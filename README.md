## これは何？
自作OSのソースコードたち

## いつごろにつくられたもの？
2025年9月～

## 感想
長いのでSTUDY_LOG.mdに記載する。

## ブートローダのビルド
edk2フォルダに移動してから以下をたたく
```sh
source edksetup.sh
build
```

## kernelのビルド
edk2フォルダに移動
【ビルドコマンド】以下をたたいてビルド
```sh
clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c ../kernel/main.cpp
```
【ビルドコマンド】virtualを使うときは以下
```sh
clang++ $CPPFLAGS -O2 --target=x86_64-elf -ffreestanding -fno-exceptions -fno-rtti -c ../kernel/main.cpp
```

【リンカコマンド】以下をたたいてリンカ（一つのファイルにビルド結果をまとめる）
```sh
ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -z separate-code -o kernel.elf main.o
```
## QEMUの使い方
edk2フォルダに移動してから以下をたたく
```sh
sh ../devenv/run_qemu.sh ＜ブートローダの実行ファイル＞　＜カーネルの実行ファイル＞
```
```sh
sh ../devenv/run_qemu.sh ./Build/TheMikuLoaderX64/DEBUG_GCC5/X64/TheMikuLoader/TheMikuLoaderPkg/Loader/OUTPUT/Loader.efi ./kernel.elf
```
実行ファイルはビルド後のバイナリファイル（.efi拡張子のもの）でないと動かないので注意！

## タグのチェックアウト
以下でタグ一覧を見れる
```sh
git tag
```
以下でチェックアウトできる。
```sh
git checkout osbook_dayxxx
```