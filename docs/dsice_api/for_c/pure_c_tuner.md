# 探索機構の関数群

DSICE で主要な役割を果たす，探索機構の関数群です．

## include

``` c
#include <dsice.h>
```

## オートチューナ本体の管理

### dsice_tuner_id 型

オートチューナを識別するために用意された型です．  
非負整数型のエイリアスとして定義されます．

### DSICE_CREATE()

``` c
dsice_tuner_id DSICE_CREATE(void);
```

オートチューナを作成します．

**引数 :** なし

**戻り値 :**

作成されたオートチューナを示す識別子．

### DSICE_DELETE()

``` c
void DSICE_DELETE(dsice_tuner_id tuner_id);
```

オートチューナを削除します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|削除するオートチューナを示す識別子|

**戻り値 :** なし

## 性能パラメタの指定

### DSICE_APPEND_PARAMETER()

``` c
void DSICE_APPEND_PARAMETER(dsice_tuner_id tuner_id, size_t parameter_length, double* parameter);
```

探索対象の性能パラメタを追加します．  
追加される性能パラメタが取り得る値の一覧は `double*` 型変数，つまり配列で列挙し，指定します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`parameter_length`|追加される性能パラメタが取り得る値の数|
|`parameter`|性能パラメタの取り得る値の一覧 (1 次元配列)|

**戻り値 :** なし

### DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE()

``` c
void DSICE_APPEND_PARAMETER_BY_LINEAR_SPACE(dsice_tuner_id tuner_id, double min, double max, double space_length);
```

探索対象の性能パラメタを追加します．  
追加される性能パラメタが取り得る値の一覧は，その範囲となる閉区間 [min, max] および値同士の間隔を指定することで，DSICE が自動で列挙します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`min`|性能パラメタが取り得る値の最小値|
|`max`|性能パラメタが取り得る値の最大値|
|`space_length`|性能パラメタが取り得る値同士の間隔|

**戻り値 :** なし

## アルゴリズムの設定

### DSICE_SELECT_MODE()

``` c
void DSICE_SELECT_MODE(dsice_tuner_id tuner_id, DSICE_EXECUTION_MODE mode);
```

実行する探索アルゴリズムを直接指定します．  
指定可能なアルゴリズムについては [`DSICE_EXECUTION_MODE` 列挙体](pure_c_dsice_execution_mode.md)を参照してください．

**注 :** この関数を使用しない，または [`DSICE_MODE_RECOMMENDED`](pure_c_dsice_execution_mode.md) を指定した場合に限り，チューニング開始時に自動的なモード選択が実行されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`mode`|探索アルゴリズムの識別子|`DSICE_MODE_RECOMMENDED`|

**戻り値 :** なし

### DSICE_CONFIG_DO_INITIAL_SEARCH()

``` c
void DSICE_CONFIG_DO_INITIAL_SEARCH(dsice_tuner_id tuner_id, bool do_init_search);
```

初期探索を行うように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`do_init_search`|初期点探索を行う場合 `true`|`false`|

**戻り値 :** なし

### DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER()

``` c
void DSICE_CONFIG_SPECIFY_INITIAL_PARAMETER(dsice_tuner_id tuner_id, double* initial_parameter);
```

探索初期値を指定します．

**注 :** [`DSICE_CONFIG_DO_INITIAL_SEARCH()`](#dsice_config_do_initial_search) と同時に使用しないでください．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`initial_parameter`|初期点として指定する性能パラメタ設定値|-|

**戻り値 :** なし

### DSICE_CONFIG_RECORD_LOG()

``` c
void DSICE_CONFIG_RECORD_LOG(dsice_tuner_id tuner_id, bool record_log);
```

ログを記録するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`record_log`|ログをとる場合 `true`|`false`|

**戻り値 :** なし

### DSICE_CONFIG_LOWER_VALUE_IS_BETTER()

``` c
void DSICE_CONFIG_LOWER_VALUE_IS_BETTER(dsice_tuner_id tuner_id, bool lower_value_is_better);
```

低い値を探索するように指定します．

**注 :** この関数は可読性向上のために存在します．DSICE はデフォルトで低い値を探索するため，通常この関数を呼び出す必要はありません．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`lower_value_is_better`|低い値を探索する場合 `true`|`true`|

**戻り値 :** なし

### DSICE_CONFIG_HIGHER_VALUE_IS_BETTER()

``` c
void DSICE_CONFIG_HIGHER_VALUE_IS_BETTER(dsice_tuner_id tuner_id, bool higher_value_is_better);
```

高い値を探索するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`higher_value_is_better`|高い値を探索する場合 `true`|`false`|

**戻り値 :** なし

### DSICE_CONFIG_SET_PARALLEL_MODE()

``` c
void DSICE_CONFIG_SET_PARALLEL_MODE(dsice_tuner_id tuner_id, bool parallel_mode);
```

並列探索アルゴリズムを使用するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`parallel_mode`|並列探索アルゴリズムを使用する場合 `true`|`false`|

**戻り値 :** なし

### DSICE_CONFIG_SET_DSPLINE_ALPHA()

``` c
void DSICE_CONFIG_SET_DSPLINE_ALPHA(dsice_tuner_id tuner_id, double alpha);
```

推定で使用する近似関数 d-Spline の滑らかさを決定する重み係数を指定します．

大きな値を設定するほど近似曲線が滑らかになり，大雑把な傾向を重視した探索となります．  
逆に小さな値を設定するほど近似曲線が実測データを重視するようになり，細かい探索を行いますが，データに誤差がある場合はその影響を強く受けるようになります．

**注 :** 設定可能な値に上限はありません (0 < `alpha` < ∞)．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`alpha`|近似関数 d-Spline の滑らかさを決定する重み係数|`0.1`|

**戻り値 :** なし

### DSICE_CONFIG_SET_DATABASE_METRIC_TYPE()

``` c
void DSICE_CONFIG_SET_DATABASE_METRIC_TYPE(dsice_tuner_id tuner_id, DSICE_DATABASE_METRIC_TYPE metric_type);
```

DSICE のデータベース内で，性能評価値がどのように扱われるかを指定します．  
指定可能な挙動については[`DSICE_DATABASE_METRIC_TYPE` 列挙体](pure_c_dsice_database_metric_type.md)を参照してください．

**注 :** この設定は，同一点に対する標本データが複数現れた場合 (同じパラメタ設定値で複数回性能評価を行った場合) にのみ影響します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|-|
|`metric_type`|データベース内での性能評価値のふるまいを指定する識別子|`DSICE_METRIC_TYPE_AVERAGE`|

**戻り値 :** なし

## 性能パラメタ設定値の取得

### DSICE_BEGIN()

``` c
const double* DSICE_BEGIN(dsice_tuner_id tuner_id);
```

次に性能パラメタに設定すべき値の組 (最優先実測値) を取得します．  
主に逐次探索アルゴリズム向けですが，並列探索アルゴリズムでも使用可能です．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :**

次に各性能パラメタに設定すべき値の組 (最優先実測値)．  
配列の各要素が各性能パラメタ ([`DSICE_APPEND_PARAMETER()`](#dsice_append_parameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### DSICE_BEGIN_PARALLEL()

``` c
const double** DSICE_BEGIN_PARALLEL(dsice_tuner_id tuner_id);
```

性能パラメタに設定する値の組の候補一覧 (実測優先度順) を取得します．  
主に並列探索アルゴリズム向けですが，逐次探索アルゴリズムでも使用可能です．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :**

性能パラメタに設定する値の組の候補一覧 (実測優先度順)．  
各性能パラメタに設定すべき値の組 (`double*`) がさらに一覧 (`double**`) となった構造です．

## 性能評価値の登録

### DSICE_END()

``` c
void DSICE_END(dsice_tuner_id tuner_id, double performance_value);
```

最優先実測値に対応する性能評価値を登録します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`performance_value`|最優先実測値に対応する性能評価値|

**戻り値 :** なし

### DSICE_END_TIME()

``` c
void DSICE_END_TIME(dsice_tuner_id tuner_id);
```

[`DSICE_BEGIN()`](#dsice_begin) を呼び出してからの経過時間を，最優先実測値に対応する性能評価値として登録します．  
経過時間は自動で計測されます．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :** なし

### DSICE_END_PARALLEL()

``` c
void DSICE_END_PARALLEL(dsice_tuner_id tuner_id, size_t value_num, const double* performance_values);
```

[`DSICE_BEGIN_PARALLEL()`](#dsice_begin_parallel) で提示される各実測候補に対応する性能評価値一覧を登録します．

**注 :** 性能評価値一覧は [`DSICE_BEGIN_PARALLEL()`](#dsice_begin_parallel) で提示される実測優先度順に作成する必要があります．

**注 :** 必ずしも [`DSICE_BEGIN_PARALLEL()`](#dsice_begin_parallel) の候補すべてに対する性能評価値を登録する必要はありません．渡された性能評価値が少ない場合，実行優先度の高い順に対応付けられ，登録されます．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`performance_values`|実測候補値に対応する性能評価値一覧|

**戻り値 :** なし

## 内部状態の取得

### DSICE_GET_PARAM_NUM()

``` c
size_t DSICE_GET_PARAM_NUM(dsice_tuner_id tuner_id);
```

登録済性能パラメタの数を取得します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :**

登録済性能パラメタの数．

### DSICE_GET_PARAM_LENGTH()

``` c
size_t DSICE_GET_PARAM_LENGTH(dsice_tuner_id tuner_id, size_t parameter_idx);
```

登録済性能パラメタのうち，指定されたパラメタが取り得る値の数を取得します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`parameter_idx`|数を取得したい性能パラメタの登録インデックス ([`DSICE_APPEND_PARAMETER()`](#dsice_append_parameter) による登録順)|

**戻り値 :**

指定された性能パラメタが取り得る値の数．

### DSICE_GET_TARGET_PARAMETERS()

``` c
const double** DSICE_GET_TARGET_PARAMETERS(dsice_tuner_id tuner_id);
```

現在の暫定最良推定設定値を取得します．

**注 :** これまでに最良の性能評価値を得た性能パラメタ設定値でないことに注意してください．この関数で取得されるのは，探索機構が推定の基準としている「基準点」です．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :**

登録されている性能パラメタリスト (2 次元配列)．  
配列の各要素となる 1 次元配列は各性能パラメタの取りうる値一覧 ([`DSICE_APPEND_PARAMETER()`](#dsice_append_parameter) による登録順) に対応しており，1 次元配列の数は性能パラメタの個数です．

### DSICE_GET_TENTATIVE_BEST_PARAMETER()

``` c
const double* DSICE_GET_TENTATIVE_BEST_PARAMETER(dsice_tuner_id tuner_id);
```

現在の暫定最良推定設定値を取得します．

**注 :** これまでに最良の性能評価値を得た性能パラメタ設定値でないことに注意してください．この関数で取得されるのは，探索機構が推定の基準としている「基準点」です．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :**

現在の暫定最良推定設定値．  
配列の各要素が各性能パラメタ ([`DSICE_APPEND_PARAMETER()`](#dsice_append_parameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### DSICE_IS_SEARCH_FINISHED()

``` c
bool DSICE_IS_SEARCH_FINISHED(dsice_tuner_id tuner_id);
```

探索機構が終了条件を満たしているかを取得します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :**

終了条件を満たしていれば `true`，まだ探索途中であれば `false`

## ログや結果の出力

### DSICE_PRINT_TUNING_RESULT_STD()

``` c
void DSICE_PRINT_TUNING_RESULT_STD(dsice_tuner_id tuner_id);
```

標準出力に探索結果を出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :** なし

### DSICE_PRINT_TUNING_RESULT_FILE()

``` c
void DSICE_PRINT_TUNING_RESULT_FILE(dsice_tuner_id tuner_id, const char* file_path);
```

ファイルに探索結果を出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`file_path`|出力先ファイルパス|

**戻り値 :** なし

### DSICE_PRINT_SIMPLE_LOOP_LOG_STD()

``` c
void DSICE_PRINT_SIMPLE_LOOP_LOG_STD(dsice_tuner_id tuner_id);
```

標準出力に各ループの簡易的なログを出力します．

**注 :** この関数は性能評価値の登録後 (各ループの最後) に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|

**戻り値 :** なし

### DSICE_PRINT_SIMPLE_LOOP_LOG_FILE()

``` c
void DSICE_PRINT_SIMPLE_LOOP_LOG_FILE(dsice_tuner_id tuner_id, const char* file_path);
```

ファイルに各ループの簡易的なログを出力します．

**注 :** この関数は性能評価値の登録後 (各ループの最後) に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`file_path`|出力先ファイルパス|

**戻り値 :** なし

### DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER()

``` c
void DSICE_OUTPUT_SEARCH_SPACE_FOR_VIEWER(dsice_tuner_id tuner_id, const char* file_path, double default_value);
```

可視化ツール DSICE Viewer 向けに探索空間データファイルを出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`file_path`|出力先ファイルパス|
|`default_value`|未実測点で性能評価値の代わりに利用する値|

### DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER()

``` c
void DSICE_OUTPUT_TUNER_LOG_FOR_VIEWER(dsice_tuner_id tuner_id, const char* file_path);
```

可視化ツール DSICE Viewer 向けにログファイルを出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`tuner_id`|対象とするオートチューナを示す識別子|
|`file_path`|出力先ファイルパス|

**戻り値 :** なし
