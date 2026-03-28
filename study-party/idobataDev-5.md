# 井戸端dev OS　vol.5
### 並列と並行
マルチタスク＝並行：進捗が複数出ること（結果論）
並列（特例的に）：ほんとにどっちも進める

### async await
asyncの範囲をawaitで区切るのです。awaitは区切り文字なのです。await前がpnedingなら次に進まない。radyなら進める。
Feature型は必ずpneding or radyを返すpoll関数を持つので、必ずpnedingなのかradyなのかを得られるのでこのように区切ることができる。
```Rust
async{
    let fn1 = fn1().await;
    let fn2 = fn2().await;
    let fn3 = fn3().await; 
}
```
完全協調的：自分から譲る。Rustなど
プリエンプティブ：いつでもぶった切られる  ⇒ await のような明示的な譲りポイントが不要　Goなど
pizzacatさんおすすめ：https://tmandry.gitlab.io/blog/posts/optimizing-await-1/
LT資料　　　　　　　 ：https://susisu.hatenablog.com/entry/2025/05/28/133921