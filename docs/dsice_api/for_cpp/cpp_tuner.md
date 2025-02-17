# Tuner クラス

DSICE で主要な役割を果たす，探索機構クラスです．

- [include](#include)
- [名前空間](#名前空間)
- [クラス定義](#クラス定義)
- [コンストラクタ](#コンストラクタ)
  - [基本コンストラクタ](#基本コンストラクタ)
  - [引数ありコンストラクタ](#引数ありコンストラクタ)
- [性能パラメタの指定](#性能パラメタの指定)
  - [appendParameter()](#appendparameter)
  - [appendParameterByLinearSpace()](#appendparameterbylinearspace)
- [アルゴリズムの設定](#アルゴリズムの設定)
  - [selectMode()](#selectmode)
  - [config_doInitialSearch()](#config_doinitialsearch)
  - [config_specifyInitialParameter()](#config_specifyinitialparameter)
  - [config_recordLog()](#config_recordlog)
  - [config_lowerValueIsBetter()](#config_lowervalueisbetter)
  - [config_higherValueIsBetter()](#config_highervalueisbetter)
  - [config_setParallelMode()](#config_setparallelmode)
  - [config_setDSplineAlpha()](#config_setdsplinealpha)
  - [config_setDatabaseMetricType()](#config_setdatabasemetrictype)
- [性能パラメタ設定値の取得](#性能パラメタ設定値の取得)
  - [getSuggestedNext()](#getsuggestednext)
  - [getSuggestedList()](#getsuggestedlist)
- [性能評価値の登録](#性能評価値の登録)
  - [setMetricValue()](#setmetricvalue)
  - [setTimePerformance()](#settimeperformance)
  - [setMetricValuesList()](#setmetricvalueslist)
- [内部状態の取得](#内部状態の取得)
  - [getParametersNum()](#getparametersnum)
  - [getParameterLength()](#getparameterlength)
  - [getTargetParameters()](#gettargetparameters)
  - [getTentativeBestParameter()](#gettentativebestparameter)
  - [isSearchFinished()](#issearchfinished)
- [ログや結果の出力](#ログや結果の出力)
  - [printTuningResult()](#printtuningresult)
  - [printSimpleLoopLog()](#printsimplelooplog)
  - [outputSearchSpaceForViewer()](#outputsearchspaceforviewer)
  - [outputLogForViewer()](#outputlogforviewer)

## include

``` cpp
#include <dsice_tuner.hpp>
```

## 名前空間

``` cpp
namespace dsice {}
```

## クラス定義

``` cpp
template class <typename T = double>
class Tuner<T> final {};
```

|要素|意味|
|:-:|:-:|
|型 `T`|性能パラメタの値型|

なお，型 `T` には `int`，`long long`，`std::size_t`，`float`，`double` を指定可能です．

## コンストラクタ

### 基本コンストラクタ

``` cpp
Tuner(void);
```

**引数 :** なし

### 引数ありコンストラクタ

``` cpp
Tuner(const std::vector<std::vector<T>>& parameters);
```

インスタンス生成時に全性能パラメタの取り得る値の一覧を指定します．

**注 :** 後から追加で指定することもできますが，指定箇所が分離することは可読性の観点から避けるべきです．

**引数 :**

|引数|説明|
|:-:|:-:|
|`parameters`|性能パラメタの取り得る値の一覧 (2 次元 vector)|

## 性能パラメタの指定

### appendParameter()

``` cpp
void appendParameter(const std::vector<T>& parameter);
```

探索対象の性能パラメタを追加します．  
追加される性能パラメタが取り得る値の一覧は `std::vector` 型変数で列挙し，指定します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`parameter`|性能パラメタの取り得る値の一覧 (1 次元 vector)|

**戻り値 :** なし

### appendParameterByLinearSpace()

``` cpp
void appendParameterByLinearSpace(const T& min, const T& max, const T& space_length);
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

### selectMode()

``` cpp
void selectMode(ExecutionMode execution_mode);
```

実行する探索アルゴリズムを直接指定します．  
指定可能なアルゴリズムについては [`ExecutionMode` 列挙体](./cpp_execution_mode.md)を参照してください．

**注 :** この関数を使用しない，または `ExecutionMode::RECOMMENDED` を指定した場合に限り，チューニング開始時に自動的なモード選択が実行されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`execution_mode`|探索アルゴリズムの識別子|`ExecutionMode::RECOMMENDED`|

**戻り値 :** なし

### config_doInitialSearch()

``` cpp
void config_doInitialSearch(bool do_init_search = true);
```

初期探索を行うように指定します．

**注 :** [`config_specifyInitialParameter()`](#config_specifyinitialparameter) と同時に使用しないでください．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`do_init_search`|初期点探索を行う場合 `true`|`false`|

**戻り値 :** なし

### config_specifyInitialParameter()

``` cpp
void config_specifyInitialParameter(const std::vector<T>& initial_parameter);
```

探索初期値を指定します．

**注 :** [`config_doInitialSearch()`](#config_doinitialsearch) と同時に使用しないでください．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`initial_parameter`|初期点として指定する性能パラメタ設定値|-|

**戻り値 :** なし

### config_recordLog()

``` cpp
void config_recordLog(bool record_log = true);
```

ログを記録するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`record_log`|ログをとる場合 `true`|`false`|

**戻り値 :** なし

### config_lowerValueIsBetter()

``` cpp
void config_lowerValueIsBetter(bool lower_value_is_better = true);
```

低い値を探索するように指定します．

**注 :** この関数は可読性向上のために存在します．DSICE はデフォルトで低い値を探索するため，通常この関数を呼び出す必要はありません．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`lower_value_is_better`|低い値を探索する場合 `true`|`true`|

**戻り値 :** なし

### config_higherValueIsBetter()

``` cpp
void config_higherValueIsBetter(bool higher_value_is_better = true);
```

高い値を探索するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`higher_value_is_better`|高い値を探索する場合 `true`|`false`|

**戻り値 :** なし

### config_setParallelMode()

``` cpp
void config_setParallelMode(bool parallel_mode = true);
```

並列探索アルゴリズムを使用するように指定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`parallel_mode`|並列探索アルゴリズムを使用する場合 `true`|`false`|

**戻り値 :** なし

### config_setDSplineAlpha()

``` cpp
void config_setDSplineAlpha(double alpha);
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

### config_setDatabaseMetricType()

``` cpp
void config_setDatabaseMetricType(DatabaseMetricType metric_type);
```

DSICE のデータベース内で，性能評価値がどのように扱われるかを指定します．  
指定可能な挙動については[`DatabaseMetricType` 列挙体](./cpp_execution_mode.md)を参照してください．

**注 :** この設定は，同一点に対する標本データが複数現れた場合 (同じパラメタ設定値で複数回性能評価を行った場合) にのみ影響します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`metric_type`|データベース内での性能評価値のふるまいを指定する識別子|`DatabaseMetricType::AverageDouble`|

**戻り値 :** なし

## 性能パラメタ設定値の取得

### getSuggestedNext()

``` cpp
const std::vector<T>& getSuggestedNext(void);
```

次に性能パラメタに設定すべき値の組 (最優先実測値) を取得します．  
主に逐次探索アルゴリズム向けですが，並列探索アルゴリズムでも使用可能です．

**引数 :** なし

**戻り値 :**  
次に各性能パラメタに設定すべき値の組 (最優先実測値)．  
`vector` の各要素が各性能パラメタ ([`appendParameter()`](#appendparameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### getSuggestedList()

``` cpp
const std::vector<std::vector<T>>& getSuggestedList(void);
```

性能パラメタに設定する値の組の候補一覧 (実測優先度順) を取得します．  
主に並列探索アルゴリズム向けですが，逐次探索アルゴリズムでも使用可能です．

**引数 :** なし

**戻り値 :**  
性能パラメタに設定する値の組の候補一覧 (実測優先度順)．  
各性能パラメタに設定すべき値の組 (`vector<T>`) がさらに一覧 (`vector<vector<T>>`) となった構造です．

## 性能評価値の登録

### setMetricValue()

``` cpp
void setMetricValue(double metric_value);
```

最優先実測値に対応する性能評価値を登録します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`metric_value`|最優先実測値に対応する性能評価値|

**戻り値 :** なし

### setTimePerformance()

``` cpp
void setTimePerformance(void);
```

[`getSuggestedNext()`](#getsuggestednext) を呼び出してからの経過時間を，最優先実測値に対応する性能評価値として登録します．  
経過時間は自動で計測されます．

**引数 :** なし

**戻り値 :** なし

### setMetricValuesList()

``` cpp
void setMetricValuesList(const std::vector<double>& metric_values_list);
```

[`getSuggestedList()`](#getsuggestedlist) で提示される各実測候補に対応する性能評価値一覧を登録します．

**注 :** 性能評価値一覧は [`getSuggestedList()`](#getsuggestedlist) で提示される実測優先度順に作成する必要があります．

**注 :** 必ずしも [`getSuggestedList()`](#getsuggestedlist) の候補すべてに対する性能評価値を登録する必要はありません．渡された性能評価値が少ない場合，実行優先度の高い順に対応付けられ，登録されます．

**引数 :**

|引数|説明|
|:-:|:-:|
|`metric_values_list`|実測候補値に対応する性能評価値一覧|

**戻り値 :** なし

## 内部状態の取得

### getParametersNum()

``` cpp
std::size_t getParametersNum(void) const;
```

登録済性能パラメタの数を取得します．

**引数 :** なし

**戻り値 :**

登録済性能パラメタの数．

### getParameterLength()

``` cpp
std::size_t getParameterLength(std::size_t parameter_idx) const;
```

登録済性能パラメタのうち，指定されたパラメタが取り得る値の数を取得します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`parameter_idx`|数を取得したい性能パラメタの登録インデックス ([`appendParameter()`](#appendparameter) による登録順)|

**戻り値 :**

指定された性能パラメタが取り得る値の数．

### getTargetParameters()

``` cpp
const std::vector<std::vector<T>>& getTargetParameters(void) const;
```

登録されている性能パラメタリストを取得します．

**引数 :** なし

**戻り値 :**  
登録されている性能パラメタリスト．  
`vector` の各要素は各性能パラメタの取りうる値一覧 ([`appendParameter()`](#appendparameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### getTentativeBestParameter()

``` cpp
std::vector<T> getTentativeBestParameter(void) const;
```

現在の暫定最良推定設定値を取得します．

**注 :** これまでに最良の性能評価値を得た性能パラメタ設定値でないことに注意してください．この関数で取得されるのは，探索機構が推定の基準としている「基準点」です．

**引数 :** なし

**戻り値 :**

現在の暫定最良推定設定値．  
`vector` の各要素が各性能パラメタ ([`appendParameter()`](#appendparameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

### isSearchFinished()

``` cpp
bool isSearchFinished(void) const;
```

探索機構が終了条件を満たしているかを取得します．

**引数 :** なし

**戻り値 :**  
終了条件を満たしていれば `true`，まだ探索途中であれば `false`

## ログや結果の出力

### printTuningResult()

``` cpp
void printTuningResult(std::string file_path = "") const;
```

標準出力，またはファイルに探索結果を出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス (空文字 `""` で標準出力)|

**戻り値 :** なし

### printSimpleLoopLog()

``` cpp
void printSimpleLoopLog(std::string file_path = "") const;
```

標準出力，またはファイルに，各ループの簡易的なログを出力します．

**注 :** この関数は性能評価値の登録後 (各ループの最後) に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス (空文字 `""` で標準出力)|

**戻り値 :** なし

### outputSearchSpaceForViewer()

``` cpp
void outputSearchSpaceForViewer(std::string file_path = "search_space.log", double default_value = 0) const;
```

可視化ツール DSICE Viewer 向けに探索空間データファイルを出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス|
|`default_value`|未実測点で性能評価値の代わりに利用する値|

**戻り値 :** なし

### outputLogForViewer()

``` cpp
void outputLogForViewer(std::string file_path = "dsice_sampling.log") const;
```

可視化ツール DSICE Viewer 向けにログファイルを出力します．

**注 :** この関数はプログラムの最後など，チューニング領域の終了後に呼び出してください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`file_path`|出力先ファイルパス|

**戻り値 :** なし
