# DSICE_DATABASE_METRIC_TYPE 列挙体

DSICE のデータベースに同じ性能パラメタ設定値の情報を複数回登録した際，データベースがどのようにふるまうかを表現します．

ふるまいの種類は以下の通りです．

|値|説明|
|:-:|:-:|
|`DSICE_METRIC_TYPE_OVERWRITTEN`|値は上書きされ，最新の値のみ使用します|
|`DSICE_METRIC_TYPE_AVERAGE`|過去に登録された値の平均値を使用します|
