# DatabaseMetricType 列挙体

DSICE のデータベースに同じ性能パラメタ設定値の情報を複数回登録した際，データベースがどのようにふるまうかを表現します．

## include

``` cpp
#include <dsice_database_metric_type.hpp>
```

## 値一覧

|値|説明|
|:-:|:-:|
|`OverwrittenDouble`|値は上書きされ，最新の値のみ使用します|
|`AverageDouble`|過去に登録された値の平均値を使用します|
