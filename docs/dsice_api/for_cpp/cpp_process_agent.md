# ProcessAgent クラス

ユーザプログラムが DSICE 本体と別で実行されている場合に，DSICE との間で性能パラメタ設定値や性能評価値を受け渡すサポートを行うクラスです．

## include

``` cpp
#include <dsice_process_agent.hpp>
```

## 名前空間

``` cpp
namespace dsice {}
```

## クラス定義

``` cpp
template <typename T = double>
class ProcessAgent final {};
```

|要素|意味|
|:-:|:-:|
|型 `T`|性能パラメタの値型|

なお，型 `T` には `int`，`long long`，`std::size_t`，`float`，`double` を指定可能です．

## コンストラクタ

性能パラメタ設定値をファイルから受け取るか，コマンドライン引数から受け取るかによって，使用するコンストラクタが異なります．

### ファイル経由の場合のコンストラクタ

``` cpp
ProcessAgent(void);
```

**引数 :** なし

### コマンドライン引数経由の場合のコンストラクタ

``` cpp
ProcessAgent(int argc, char** argv);
```

**引数 :**

|引数|説明|
|:-:|:-:|
|`argc`|コマンドライン引数の数 (`main` 関数の第一引数)|
|`argv`|コマンドライン引数の中身 (`main` 関数の第二引数)|

## 性能パラメタ設定値の取得

### getParameter()

``` cpp
const std::vector<T>& getParameter(void) const;
```

性能パラメタ設定値を取得します．

**引数 :** なし

**戻り値 :**

次に各性能パラメタに設定すべき値の組．  
`vector` の各要素が各性能パラメタ (DSICE の [`appendParameter()`](./cpp_tuner.md#appendparameter) による登録順) に対応しており，要素数は性能パラメタの個数です．

## 性能評価値の登録

### recordPerformanceValue()

``` cpp
void recordPerformanceValue(double performance_value) const;
```

性能評価値を登録します．

**引数 :**

|引数|説明|
|:-:|:-:|
|`performance_value`|性能評価値|

**戻り値 :** なし

## 内部状態の取得

### getProcessId()

``` cpp
std::size_t getProcessId(void) const;
```

[`ProcessManager`](./cpp_process_manager.md) クラスがプロセス管理時に使用している識別子を取得します．

**引数 :** なし

**戻り値 :**

プロセス識別子
