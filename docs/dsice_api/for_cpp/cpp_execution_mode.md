# ExecutionMode (探索アルゴリズム) 列挙体

実行される探索アルゴリズムを表現する列挙体です．

## include

``` cpp
#include <dsice_execution_mode.hpp>
```

## 値一覧

|値|説明|
|:-:|:-:|
|`RECOMMENDED`|アルゴリズムが DSICE により自動で選択されます|
|`S_IPPE`|1 パラメタの場合限定の逐次探索アルゴリズム (参照：[論文](https://doi.org/10.1155/2014/310879))|
|`S_2017`|[2017 年の論文](https://doi.org/10.1109/IPDPSW.2017.132)で提案された逐次探索アルゴリズム|
|`S_2018`|[2018 年の論文](https://ipsj.ixsq.nii.ac.jp/records/190744)で提案された逐次探索アルゴリズム|
|`P_2024`|[SC 24 のポスター](https://sc24.supercomputing.org/proceedings/poster/poster_pages/post207.html)で提案された並列探索アルゴリズム|
