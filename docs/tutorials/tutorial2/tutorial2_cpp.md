# チュートリアル 2 - DSICEの設定

DSICE の設定方法を学びます．

## 0. はじめに

DSICE の設定はインスタンス作成，および性能パラメタの登録が完了した後に行ってください．また，そのインスタンスで最初に `getSuggestedNext()` を呼び出したタイミングで設定項目が反映されるため，それよりも前に行う必要があります．

具体的には，[チュートリアル 1](../tutorial1/tutorial1_top.md) のコード例では次の位置です．

``` cpp
Tuner<double> tuner;
tuner.appendParameter(param1);
tuner.appendParameter(param2);

//
// ここに設定を記述
//

for (int i = 0; i < 100; i++) {
    std::vector<double> suggested = tuner.getSuggestedNext();
```

なお，設定内容によっては一部の設定が無視される場合があります．

## 1. 高い方が良いか低い方が良いか

DSICE はデフォルトで**低い性能値を引き出す設定値を探します**．これは DSICE のメインターゲットが元々，プログラムの実行時間短縮であったことに由来します．

しかし，性能には高い方が良いものもあります (例 : 正答率)．そのような性能を対象とする場合は，デフォルトのままではより悪いものを探してしまうことになるわけです．  
もちろん，DSICE はより高い値を探すように設定可能です．[`config_higherValueIsBetter()`](../../dsice_api/for_cpp/cpp_tuner.md#config_highervalueisbetter) を呼び出してください．

``` cpp
tuner.config_higherValueIsBetter();
```

なお，デフォルト設定なので不要ですが，低い値を探す場合には次のように明示することも可能です．

``` cpp
tuner.config_lowerValueIsBetter();
```

## 2. アルゴリズムの種類を直接指定する

DSICE のアルゴリズムは，デフォルトでは推奨アルゴリズムが選択されます．推奨アルゴリズムは多くの対象に有効なように考えられていますが，一部のプログラムでは必ずしも最適でない場合もあります．  
また，推奨アルゴリズムは DSICE のバージョンによって変わる場合があります．バージョンごとに挙動を変えたくない場合や過去の挙動を再現したい場合など，自動選択を解除したいということもあります．

DSICE で使用可能なアルゴリズムは，[`ExecutionMode`](../../dsice_api/for_cpp/cpp_execution_mode.md) 列挙体で定義されています．  
列挙子が `S_` ではじまるものは逐次探索向けアルゴリズム，`P_` ではじまるものは並列探索向けアルゴリズムです (並列探索については[チュートリアル 3](../tutorial3/tutorial3_top.md) を参照してください)．

次のように `selectMode()` で，[`ExecutionMode`](../../dsice_api/for_cpp/cpp_execution_mode.md) 列挙体を指定することが可能です．

``` cpp
tuner.selectMode(ExecutionMode::S_2018);
```

## 3. 探索初期点に関する設定

DSICE の探索 (一部アルゴリズムを除く) は，デフォルトでは探索空間の中心点 (各性能パラメタが取りうる値一覧でインデックスが真ん中の値) から開始されます．これは偏りのない状態から開始するという意味では正解ですが，より初期点として適している点があるかもしれません．例えば極端な話ですが，最良点 (最良性能を引き出すことができる値) からスタートすれば，最初から確実に最良点を見つけることができます．

そのような極端な例でなくとも，事前にある程度傾向が分かっているのであれば，その周辺をできるだけ集中的に調べた方が良い点を見つけやすいでしょう．  
DSICE では，探索の初期基準点を直接指定することができます．[`config_specifyInitialParameter()`](../../dsice_api/for_cpp/cpp_tuner.md#config_specifyinitialparameter) に初期値の一覧を渡してください．

``` cpp
tuner.config_specifyInitialParameter({5.0, 6.5});
```

しかしもちろん，何も傾向が分からない状態で探索する場合もあるでしょう．デフォルト通りに中心点から開始することもできますが，それでもできるだけ適切な初期点から開始したいものです．  
このような場合，DSICE は初期点探索を行うことができます．初期点探索は通常の探索を行う前にいくつかの試行を行い，初期点として適切そうな点を探す機能です．初期点探索で試行される点は極力偏りがないように，かつその後の探索がスムーズに進むように選択されます．  
初期点探索を行う場合は次のように，[`config_doInitialSearch()`](../../dsice_api/for_cpp/cpp_tuner.md#config_doinitialsearch) を呼び出します．

``` cpp
tuner.config_doInitialSearch();
```

初期点に関する設定の説明は以上ですが，これらの設定には注意点が 3 つあります．  

1 つめ，[`config_specifyInitialParameter()`](../../dsice_api/for_cpp/cpp_tuner.md#config_specifyinitialparameter) と [`config_doInitialSearch()`](../../dsice_api/for_cpp/cpp_tuner.md#config_doinitialsearch) は同時に片方しか使えません．  
2 つめ，初期点探索における試行対象の選択にはランダム性があります．初期点探索を行う場合，実行するごとに挙動が異なる可能性がある点に注意してください．  
3 つめ，**初期点探索を行う場合はその分試行回数が増える可能性があります**．総試行回数を抑えることも初期点探索の目的ですが，その初期点探索自体でも試行していることには注意が必要です．ユーザプログラムに初期点探索が有効である保証はありません．

## 4. データの誤差レベルを考慮する

DSICE は繰り返し試行を行いますが，試行によって得られる結果 (性能評価値) には誤差が含まれる場合があります．誤差が含まれるということは DSICE は本来の結果とは異なる情報を与えられるわけですから，推定を正しく行うのが困難になります．誤差が大きければ大きいほど，その悪影響も強くなってしまうでしょう．

実はデフォルトの状態では，DSICE は与えられた結果をかなり強く信頼します．つまり，大きな誤差が含まれていた場合はその影響を強く受けます．  

誤差が大きいのであれば，個々のデータを信頼するよりも全体的な傾向を大雑把に見た方が精度がチューニングの精度が良い可能性があります．  
[`config_setDSplineAlpha()`](../../dsice_api/for_cpp/cpp_tuner.md#config_setdsplinealpha) で内部の近似関数における重み係数を設定することができます．設定可能な値は正の浮動小数点数で，値が大きいほど滑らか (傾向重視) になります．  
デフォルト値は `0.1` ですが，この値は単純な割合 (0～1) ではありません．誤差の影響が強い場合には `10000` などの非常に大きな値を設定すると改善することがあります．

``` cpp
tuner.config_setDSplineAlpha(10000);
```
