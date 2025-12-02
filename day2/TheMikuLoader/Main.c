
# include <Uefi.h>
# include <Library/UefiLib.h>
# include <Library/UefiBootServicesTableLib.h>
# include <Library/PrintLib.h>
# include <Protocol/LoadedImage.h>
# include <Protocol/SimpleFileSystem.h>
# include <Protocol/DiskIo2.h>
# include <Protocol/BlockIo.h>
# include <Guid/FileInfo.h>
// Hello, world!
// EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
//     Print(L"Hello, TheMiku world!\n");
//     while (1);
//     return  0;    
// }

struct MemoryMap{
    UINTN buffer_size;
    VOID* buffer;
    UINTN map_size;
    UINTN map_key;
    UINTN descriptor_size;
    UINT32 descriptor_version;
};

EFI_STATUS GetMemoryMap(struct MemoryMap* map){
    if(map->buffer == NULL){
        return EFI_BUFFER_TOO_SMALL;
    }
    map->map_size = map->buffer_size;
    return gBS->GetMemoryMap(
        &map->map_size,
        (EFI_MEMORY_DESCRIPTOR*)map->buffer,
        &map->map_key,
        &map->descriptor_size,
        &map->descriptor_version
    );
}

const CHAR16* GetMemoryTypeUnicode(EFI_MEMORY_TYPE type){
    switch(type){
        case EfiReservedMemoryType: return L"EfiResercedMemoryType";
        case EfiLoaderCode: return L"EfiloaderCode";
        case EfiLoaderData: return L"EfiLoaderData";
        case EfiBootServicesCode: return L"EfiBootServicesCode";
        case EfiBootServicesData: return L"EfiBootServiceDate";
        case EfiRuntimeServicesCode: return L"EfiRuntimeServicesCode";
        case EfiRuntimeServicesData: return L"EfiRuntimeSevicesData";
        case EfiConventionalMemory: return L"EficonvetionalMemory"; //空き容量
        case EfiUnusableMemory: return L"EfiUnusableMemoruy";
        case EfiACPIMemoryNVS:return L"EfiACPIMemoriy"; // ACPIは電源に関する制御のプロトコル（参考：https://wa3.i-3-i.info/word14319.html）
        case EfiMemoryMappedIO: return L"EfiMemoryMappedIO";
        case EfiPalCode: return L"EfiPalCode";
        case EfiPersistentMemory: return L"EfiPersistentMemory"; // Persistent=持続的
        case EfiMaxMemoryType: return L"EfiMaxMemoryType";
        default: return L"InvalidMemoryType";
    }
}

EFI_STATUS SaveMemoryMap(struct MemoryMap* map, EFI_FILE_PROTOCOL* file){
    CHAR8 buf[256];
    UINTN len;

    CHAR8* header = "Indec, Type, Type(name), PhysicalStart, NumberOfPages, Attribute\n";
    len = AsciiStrLen(header);
    file->Write(file, &len, header);

    Print(L"map->buffer = %08lx, map->map_sixe = %08lx\n", map->buffer, map->map_size);

    EFI_PHYSICAL_ADDRESS iter;
    int i;
    for(iter = (EFI_PHYSICAL_ADDRESS)map->buffer, i = 0;
        iter < (EFI_PHYSICAL_ADDRESS)map->buffer + map->map_size;
        iter += map->descriptor_size, i++){
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)iter;
        // メモリディスクリプタの値を文字列にする（discriptor＝ファイルの通り道の目印）
        len = AsciiSPrint(
            buf ,sizeof(buf),
            "%u, %x, %-ls, %08lx, %lx, %lx\n",
            i, desc->Type, GetMemoryTypeUnicode(desc->Type),
            desc->PhysicalStart,desc->NumberOfPages,
            desc->Attribute & 0xFFFFFlu);
            // 文字列を書き込む
        file->Write(file, &len,buf) ;
    }
    return EFI_SUCCESS;
}

EFI_STATUS OpenRootDir(EFI_HANDLE image_handle, EFI_FILE_PROTOCOL** root){
    EFI_LOADED_IMAGE_PROTOCOL* loaded_image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fs;

    gBS->OpenProtocol(
        image_handle,
        &gEfiLoadedImageProtocolGuid,
        (VOID**)&loaded_image,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    
    gBS->OpenProtocol(
        loaded_image->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID**)&fs,
        image_handle,
        NULL,
        EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL
    );
    fs->OpenVolume(fs,root);
    return EFI_SUCCESS;
}
EFI_STATUS EFIAPI UefiMain(EFI_HANDLE image_handle,EFI_SYSTEM_TABLE* system_table){
    Print(L"Hello, TheMiku world!\n");
    CHAR8 memmap_buf[4096 * 4];
    struct MemoryMap memmap = {sizeof(memmap_buf),memmap_buf,0,0,0,0};
    GetMemoryMap(&memmap);

    EFI_FILE_PROTOCOL* root_dir;
    OpenRootDir(image_handle,&root_dir);
    EFI_FILE_PROTOCOL* memmap_file;
    root_dir->Open(
        root_dir,&memmap_file,L"\\memmap",EFI_FILE_MODE_READ|EFI_FILE_MODE_WRITE|EFI_FILE_MODE_CREATE,0
    );

    SaveMemoryMap(&memmap,memmap_file);
    memmap_file->Close(memmap_file);

    // カーネルファイルを読み込み専用で開く
    EFI_FILE_PROTOCOL* kernel_file;
    root_dir->Open(root_dir, &kernel_file, L"\\kernel.elf", EFI_FILE_MODE_READ, 0);

    // メモリを確保するために、カーネルファイル全体の大きさを取得する
    UINTN file_info_size = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 12 ;
    UINT8 file_info_buffer[file_info_size];
    kernel_file->GetInfo(kernel_file, &gEfiFileInfoGuid,&file_info_size, file_info_buffer);
    //// 
    EFI_FILE_INFO* file_info = (EFI_FILE_INFO*)file_info_buffer; // ポインタ
    UINTN kernel_file_size = file_info->FileSize;

    // カーネルファイルのメモリ領域を確保する
    EFI_PHYSICAL_ADDRESS kernel_base_addr = 0x100000; // 指定したアドレスから確保しないと壊れるため、指定
    gBS->AllocatePages(
        AllocateAddress, EfiLoaderData,
        (kernel_file_size = 0xfff) / 0x1000,&kernel_base_addr);
    // カーネルファイルを読み込む
    kernel_file->Read(kernel_file, &kernel_file_size, (VOID*)kernel_base_addr);
    Print(L"Kernel; 0x%0lx (%lu bytes)\n",kernel_base_addr,kernel_file_size);

    // すでに動いているブートサービスを停止させる
    EFI_STATUS status;
    status = gBS->ExitBootServices(image_handle,memmap.map_key);
    //// 1度は失敗するから何度かやる
    if(EFI_ERROR(status)){
        status = GetMemoryMap(&memmap);
        if(EFI_ERROR(status)){
            Print(L"failed to get memory map: %r\n" , status);
            while (1);        
        }
        status = gBS->ExitBootServices(image_handle,memmap.map_key);
        if(EFI_ERROR(status)){
            Print(L"Could not exit boot service: %r\n",status);
            while(1);
        }
    }

    // カーネルを起動する
    UINT64 entry_addr =*(UINT64*)(kernel_base_addr + 24); // EFL形式の仕様により24バイトのオフセット

    typedef void EntryPointType(void);
    EntryPointType* entry_point = (EntryPointType*)entry_addr; // <---KernelMain()の実体のアドレスになるはず
    entry_point();

    Print(L"All done\n");
    while (1);
    return  0;
}