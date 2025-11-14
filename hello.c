typedef unsigned short CHAR16;
typedef unsigned long long EFI_STATUS;
typedef void *EFI_HANDLE;

struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef EFI_STATUS (*EFI_TEXT_STRING)(
  struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This,
  CHAR16                                   *String);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  void             *dummy;
  EFI_TEXT_STRING  OutputString;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
  char                             dummy[52];
  EFI_HANDLE                       ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *ConOut;
} EFI_SYSTEM_TABLE;

// EFI_STATUS EfiMain(EFI_HANDLE        ImageHandle,
//                    EFI_SYSTEM_TABLE  *SystemTable) {
//   SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!\n");
//   while (1);
//   return 0;
// }


// 参考：https://morinokabu.com/2025/08/25/c-programming-multiple-files-header-files-compilation/
// includeは指定されたファイルの中身を、その場所にまるごとコピー＆ペーストする命令
// ヘッダーファイル（.h）に書くもの → 「宣言」
// ソースファイル（.c）に書くもの → 「定義」
// #include <system_header.h>: 主にOSやコンパイラが提供するシステム標準のヘッダーファイル
// #include "my_header.h": 主に自作のヘッダーファイル

// # include <stdio.h>
// # include <efi.h>
// # include <efilib.h>
// C言語コンパイルの仕組み：https://tekrafter.com/entry/2025/03/29/205119#google_vignette
EFI_STATUS EfMain(EFI_HANDLE ImageHandle,EFI_SYSTEM_TABLE *SystemTable){
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello, world!\n");
    while (1);
    return  0;    
}

