## これは何？
自作OSのソースコードたち

## いつごろにつくられたもの？
2025年9月～

## 感想
### 1章
https://nakaterux.hatenablog.com/entry/2025/01/11/213026
本を割いた弊害で環境構築ができなかったり、実機がないから確認できないなどで足止めを食らった。開発のハードルを下げないといつまでたってもおわらないので工夫しよう。エミュレータをいれるか？
### 2章
開発ハードルを下げるためにXサーバーを入れてQEMUを使えるようにした。
勉強のためにそのままではなくパッケージ名を「TheMikuLoaderPkg」にした。大変だったけど、紐づきとかも分かって勉強になった。

二日目は長かったなポインタについて学んだ。C言語では関数の帰り値が1つしか返せないから、引数でポインタを受け取って、変数の中身を書き換えちゃうってのがよく理解できた。そして、C言語の名でのポインタの書き方とかもわかった。ポインタの型もあるのが驚いた。
おもろいサイト（https://yotio.jp/）見つけた。はりぼてOSってやつで、本物のOSではないんだけど、凝っているサイトでこれがあるから個人サイトは楽しいんだ！インターネット最高だなという気持ちになる。
### 3章
レジスタとは何かを学んだ。いつもメインメモリと迷うんだけど、レジスタはCPUの中にあるってこととかたぶん一生忘れない。やっぱり資格勉強を頑張るよりこうやって勉強するのが自分には合っているかも。にしてもレジスタに書き込むのってメインメモリの1/2000ってすごく早いな。

## ブートローダのビルド
edk2フォルダに移動してから以下をたたく
```sh
source edksetup.sh
build
```

## kernelのビルド
edk2フォルダに移動
以下をたたいてビルド
```sh
clang++ $CPPFLAGS -O2 --target=x86_64-elf -fno-exceptions -ffreestanding -c ../kernel/main.cpp
```
以下をたたいてリンカ（一つのファイルにビルド結果をまとめる）
```sh
ld.lld $LDFLAGS --entry KernelMain -z norelro --image-base 0x100000 --static -z separate-code -o kernel.elf main.o
```

## QEMUの使い方
edk2フォルダに移動してから以下をたたく
```sh
sh ../devenv/run_qemu.sh ＜ブートローダの実行ファイル＞　＜カーネルの実行ファイル＞
```
```sh
sh ../devenv/run_qemu.sh ./Build/TheMikuLoaderX64/DEBUG_GCC5/X64/TheMikuLoaderPkg/Loader/OUTPUT/Loader.efi ../kernel/kernel.elf 
```
実行ファイルはビルド後のバイナリファイル（.efi拡張子のもの）でないと動かないので注意！