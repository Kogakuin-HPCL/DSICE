# Tuner (探索機構) クラス

DSICE で主要な役割を果たす，探索機構クラスです．

- [import](#import)
- [コンストラクタ](#コンストラクタ)
- [性能パラメタの指定](#性能パラメタの指定)
  - [append_parameter()](#append_parameter)
  - [append_parameter_by_linear_space()](#append_parameter_by_linear_space)
- [アルゴリズムの設定](#アルゴリズムの設定)
  - [select_mode()](#select_mode)
  - [config_do_initial_search()](#config_do_initial_search)
  - [config_specify_initial_parameter](#config_specify_initial_parameter)
  - [config_record_log()](#config_record_log)
  - [config_lower_value_is_better()](#config_lower_value_is_better)
  - [config_higher_value_is_better()](#config_higher_value_is_better)
  - [config_set_parallel_mode()](#config_set_parallel_mode)
  - [config_set_d_spline_alpha()](#config_set_d_spline_alpha)
  - [config_set_database_metric_type()](#config_set_database_metric_type)
- [性能パラメタ設定値の取得](#性能パラメタ設定値の取得)
  - [get_suggested_next()](#get_suggested_next)
  - [get_suggested_list()](#get_suggested_list)
- [性能評価値の登録](#性能評価値の登録)
  - [set_metric_value()](#set_metric_value)
  - [set_time_performance()](#set_time_performance)
  - [set_metric_values_list()](#set_metric_values_list)
- [内部状態の取得](#内部状態の取得)
  - [get_param_num()](#get_param_num)
  - [get_param_length()](#get_param_length)
  - [get_target_parameters()](#get_target_parameters)
  - [get_tentative_best_parameter()](#get_tentative_best_parameter)
  - [is_search_finished()](#is_search_finished)
- [ログや結果の出力](#ログや結果の出力)
  - [print_tuning_result()](#print_tuning_result)
  - [print_simple_loop_log()](#print_simple_loop_log)
  - [output_search_space_for_viewer()](#output_search_space_for_viewer)
  - [output_log_for_viewer()](#output_log_for_viewer)

## import

``` python
from dsice.tuner import Tuner
```

## コンストラクタ

``` python
def __init__(self):
```

**引数 :** なし

## 性能パラメタの指定

### append_parameter()

``` python
def append_parameter(self, param:list) -> None:
```

探索対象の性能パラメタを追加します．  
追加される性能パラメタが取り得る値の一覧は `list` で列挙し，指定します．

**注 :** 要素となる値は `float` 型である必要があります (内部で倍精度浮動小数点数として扱われます)．

**引数 :**

|引数|説明|
|:-:|:-:|
|`param`|性能パラメタの取り得る値の一覧 (1 次元リスト)|

**戻り値 :** なし

### append_parameter_by_linear_space()

``` python
def append_parameter_by_linear_space(self, min:float, max:float, space_length:float) -> None:
```

探索対象の性能パラメタを追加します．  
追加される性能パラメタが取り得る値の一覧は，その範囲となる閉区間 [min, max] および値同士の間隔を指定することで，DSICE が自動で列挙します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`min`|性能パラメタが取り得る値の最小値|
|`max`|性能パラメタが取り得る値の最大値|
|`space_length`|性能パラメタが取り得る値同士の間隔|

**戻り値 :** なし

## アルゴリズムの設定

### select_mode()

``` python
def select_mode(self, mode:DSICE_EXECUTION_MODE) -> None:
```

実行する探索アルゴリズムを直接指定します．  
指定可能なアルゴリズムについては [`DSICE_EXECUTION_MODE` 列挙体](py_dsice_execution_mode.md)を参照してください．

**注 :** この関数を使用しない，または `DSICE_EXECUTION_MODE.RECOMMENDED` を指定した場合に限り，チューニング開始時に自動的なモード選択が実行されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`mode`|探索アルゴリズムの識別子|`DSICE_EXECUTION_MODE.RECOMMENDED`|

**戻り値 :** なし

### config_do_initial_search()

``` python
def config_do_initial_search(self, do_init_search:bool=True) -> None:
```

初期探索を行うように指定します．

**注 :** [`config_specify_initial_parameter()`](#config_specify_initial_parameter) と同時に使用しないでください．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`do_init_search`|初期点探索を行う場合 `True`|`False`|

**戻り値 :** なし

### config_specify_initial_parameter()

``` python
def config_specify_initial_parameter(self, initial_parameter:list) -> None:
```

探索初期値を指定します．

**注 :** [`config_do_initial_search()`](#config_do_initial_search) と同時に使用しないでください．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`initial_parameter`|初期点として指定する性能パラメタ設定値|-|

**戻り値 :** なし

### config_record_log()

``` python
def config_record_log(self, record_log:bool=True) -> None:
```

ログを記録するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`record_log`|ログをとる場合 `True`|`False`|

**戻り値 :** なし

### config_lower_value_is_better()

``` python
def config_lower_value_is_better(self, lower_value_is_better:bool=True) -> None:
```

低い値を探索するように指定します．

**注 :** この関数は可読性向上のために存在します．DSICE はデフォルトで低い値を探索するため，通常この関数を呼び出す必要はありません．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`lower_value_is_better`|低い値を探索する場合 `True`|`True`|

**戻り値 :** なし

### config_higher_value_is_better()

``` python
def config_higher_value_is_better(self, higher_value_is_better:bool=True) -> None:
```

高い値を探索するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`higher_value_is_better`|高い値を探索する場合 `True`|`False`|

**戻り値 :** なし

### config_set_parallel_mode()

``` python
def config_set_parallel_mode(self, parallel_mode:bool=True) -> None:
```

並列探索アルゴリズムを使用するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`parallel_mode`|並列探索アルゴリズムを使用する場合 `True`|`False`|

**戻り値 :** なし

### config_set_d_spline_alpha()

``` python
def config_set_d_spline_alpha(self, alpha:float) -> None:
```

推定で使用する近似関数 d-Spline の滑らかさを決定する重み係数を指定します．

大きな値を設定するほど近似曲線が滑らかになり，大雑把な傾向を重視した探索となります．  
逆に小さな値を設定するほど近似曲線が実測データを重視するようになり，細かい探索を行いますが，データに誤差がある場合はその影響を強く受けるようになります．

**注 :** 設定可能な値に上限はありません (0 < `alpha` < ∞)．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`alpha`|近似関数 d-Spline の滑らかさを決定する重み係数|`0.1`|

**戻り値 :** なし

### config_set_database_metric_type()

``` python
def config_set_database_metric_type(self, metric_type:DSICE_DATABASE_METRIC_TYPE) -> None:
```

DSICE のデータベース内で，性能評価値がどのように扱われるかを指定します．  
指定可能な挙動については[`DSICE_DATABASE_METRIC_TYPE` 列挙体](py_dsice_database_metric_type.md)を参照してください

**注 :** この設定は，同一点に対する標本データが複数現れた場合 (同じパラメタ設定値で複数回性能評価を行った場合) にのみ影響します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`metric_type`|データベース内での性能評価値のふるまいを指定する識別子|`DSICE_DATABASE_METRIC_TYPE.Average`|

**戻り値 :** なし

## 性能パラメタ設定値の取得

### get_suggested_next()

``` python
def get_suggested_next(self) -> list:
```

次に性能パラメタに設定すべき値の組 (最優先実測値) を取得します．  
主に逐次探索アルゴリズム向けですが，並列探索アルゴリズムでも使用可能です．

**引数 :** なし

**戻り値 :**  
次に各性能パラメタに設定すべき値の組 (最優先実測値)，1 次元リスト．  
`list` の各要素が各性能パラメタ ([`append_parameter()`](#append_parameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### get_suggested_list()

``` python
def get_suggested_list(self) -> list:
```

性能パラメタに設定する値の組の候補一覧 (実測優先度順) を取得します．  
主に並列探索アルゴリズム向けですが，逐次探索アルゴリズムでも使用可能です．

**引数 :** なし

**戻り値 :**  
性能パラメタに設定する値の組の候補一覧 (実測優先度順)，2 次元リスト．  
各性能パラメタに設定すべき値の組 (`list`) がさらに一覧 (`[[]]`) となった構造です．

## 性能評価値の登録

### set_metric_value()

``` python
def set_metric_value(self, metric_value:float) -> None:
```

最優先実測値に対応する性能評価値を登録します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`metric_value`|最優先実測値に対応する性能評価値|

**戻り値 :** なし

### set_time_performance()

``` python
def set_time_performance(self) -> None:
```

[`get_suggested_next()`](#get_suggested_next) を呼び出してからの経過時間を，最優先実測値に対応する性能評価値として登録します．  
経過時間は自動で計測されます．

**引数 :** なし

**戻り値 :** なし

### set_metric_values_list()

``` python
def set_metric_values_list(self, metric_values_list:list) -> None:
```

[get_suggested_list()](#get_suggested_list) で提示される各実測候補に対応する性能評価値一覧を登録します．

**注 :** 性能評価値一覧は [get_suggested_list()](#get_suggested_list) で提示される実測優先度順に作成する必要があります．

**注 :** 必ずしも [get_suggested_list()](#get_suggested_list) の候補すべてに対する性能評価値を登録する必要はありません．渡された性能評価値が少ない場合，実行優先度の高い順に対応付けられ，登録されます．

**引数 :**

|引数|説明|
|:-:|:-:|
|`metric_values_list`|実測候補値に対応する性能評価値一覧|

**戻り値 :** なし

## 内部状態の取得

### get_param_num()

``` python
def get_param_num(self) -> int:
```

登録済性能パラメタの数を取得します．

**引数 :** なし

**戻り値 :**  登録済性能パラメタの数

### get_param_length()

``` python
def get_param_length(self, parameter_idx:int) -> int:
```

登録済性能パラメタのうち，指定されたパラメタが取り得る値の数を取得します．

**引数 :** 数を取得したい性能パラメタの登録インデックス ([`append_parameter()`](#append_parameter) による登録順)

**戻り値 :**  指定された性能パラメタが取り得る値の数

### get_target_parameters()

``` python
def get_target_parameters(self) -> list:
```

登録されている性能パラメタリストを取得します．

**引数 :** なし

**戻り値 :**  
登録されている性能パラメタリスト．  
`list` の各要素は各性能パラメタの取りうる値一覧 ([`append_parameter()`](#append_parameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### get_tentative_best_parameter()

``` python
def get_tentative_best_parameter(self) -> list:
```

現在の暫定最良推定設定値を取得します．

**注 :** これまでに最良の性能評価値を得た性能パラメタ設定値でないことに注意してください．この関数で取得されるのは，探索機構が推定の基準としている「基準点」です．

**引数 :** なし

**戻り値 :**  
現在の暫定最良推定設定値 (1 次元リスト)．  
`list` の各要素が各性能パラメタ ([`append_parameter()`](#append_parameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### is_search_finished()

``` python
def is_search_finished(self) -> bool:
```

探索機構が終了条件を満たしているかを取得します．

**引数 :** なし

**戻り値 :**  
終了条件を満たしていれば `True`，まだ探索途中であれば `False`

## ログや結果の出力

### print_tuning_result()

``` python
def print_tuning_result(self, file_path:str=None) -> None:
```

標準出力，またはファイルに探索結果を出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス (`None` で標準出力)|

**戻り値 :** なし

### print_simple_loop_log()

``` python
def print_simple_loop_log(self, file_path:str=None) -> None:
```

標準出力，またはファイルに，各ループの簡易的なログを出力します．

**注 :** この関数は性能評価値の登録後 (各ループの最後) に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス (`None` で標準出力)|

**戻り値 :** なし

### output_search_space_for_viewer()

``` python
def output_search_space_for_viewer(self, file_path:str='search_space.log', default_value:float=0) -> None:
```

可視化ツール DSICE Viewer 向けに探索空間データファイルを出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス|
|`default_value`|未実測点で性能評価値の代わりに利用する値|

**戻り値 :** なし

### output_log_for_viewer()

``` python
def output_log_for_viewer(self, file_path:str='dsice_sampling.log') -> None:
```

可視化ツール DSICE Viewer 向けにログファイルを出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス|

**戻り値 :** なし
