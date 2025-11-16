
# include <Uefi.h>
# include <Library/UefiLib.h>
EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
    // SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!\n");
    Print(L"Hello, TheMiku world!\n");
    while (1);
    return  0;    
}