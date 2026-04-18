# 井戸端dev OS　vol.6
### Staticかつmutable
mutable：可変 ⇒ javaでいうとClass
imutable：不変 ⇒ Javaでいうとプリミティブ型

マーカートレイト：スレッドをまたぐと危険だとわかるためのマーク。

### プロセスとスレッド
プロセス（Process）
* OSが起動する独立したプログラムの実行単位
* それぞれが独自のメモリ空間を持つ
* プロセス同士はメモリを共有しない（隔離されている）
* 起動・切り替えのコストが高い
* 例：ChromeとVSCodeは別々のプロセス

スレッド（Thread）
* プロセスの内部で動く実行単位
* 同じプロセス内のスレッドはメモリを共有する
* 起動・切り替えのコストが低い（軽量）
* 例：Chromeの各タブは同じプロセス内の別スレッド

## ミューテックス（Mutex）
DBの排他ロックのようなもの。（共有ロックのように読み込みだけとかもダメ）
共有ロック（読むのはOKで書くのはだめ）みたいなミューテックスを実装することもできるけどふつうに全部を禁止するミューテックスを使う。
セマフォ：何人アクセスできるかを制限できる。ミューテックスはバイナリセマフォという特殊なセマフォとみなすことができる

MutexGurd：Mutexのアクセス権を取得できた（英語でscaop lock）ことを保持する

## スピンロック
ロックが取れるまでループすること

## アトミック
Atomic＝原子性

そういえば、DBのACID特性で出てきたよね。
A → 原子性  (Atomicity)　全部成功するか失敗するかの二択だよ。中途半端はダメ。
C → 一貫性  (Consistency)
I → 独立性  (Isolation)
D → 永続性  (Durability)

アトミック操作
　CPUが1命令で完結させる割り込み不可能な操作

アトミックブール
　読み書きをアトミックに行える変数？？←謎残る

## イテレータとジェネレータ
イテレータ　：順番にアクセスする仕組み
https://saycon.co.jp/archives/neta/iterator
```java
List<String> fruits = Arrays.asList("りんご", "バナナ", "みかん");

Iterator<String> iterator = fruits.iterator();

while (iterator.hasNext()) {
    String fruit = iterator.next();
    System.out.println(fruit);
}
```
ジェネレータ：イテレータを簡単に作る仕組み

## マルチタス
協調的マルチタスクと非協調的マルチタスク
非協調的マルチタスクのほうが頑健性が高い。変な実装があっても強制的にほかの処理に分配する。

頑健性（がけんせい）＝ロバスト≈強い

## relaxed
relaxedはOSでよく出てくるなぁ。。。
Relaxedとは、アトミック性だけ保証しャッシュ同期は保証しないという意味

* Sequentially Consistent（seq_cst）  ← 最も厳格・最も遅い
* Acquire / Release
* Relaxed                             ← 最も緩い・最も速い

## ユーザーランド