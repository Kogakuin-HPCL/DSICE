# ProcessManager クラス

ユーザプログラムをプロセスで起動する機構作成のサポートを行うクラスです．

## include

``` cpp
#include <dsice_process_manager.hpp>
```

## 名前空間

``` cpp
namespace dsice {}
```

## クラス定義

``` cpp
template <typename T = double>
class ProcessManager final {};
```

|要素|意味|
|:-:|:-:|
|型 `T`|性能パラメタの値型|

なお，型 `T` には `int`，`long long`，`std::size_t`，`float`，`double` を指定可能です．

## コンストラクタ

``` cpp
ProcessManager(void);
```

**引数 :** なし

## 起動プロセスの指定

### setJobScript()

``` cpp
void setJobScript(SuperComputer machine, const std::string& script_path);
```

スーパーコンピュータでの実行と，実行するプログラムを指定します．  
指定可能なスーパーコンピュータについては [`SuperComputer` 列挙体](./cpp_super_computer.md)を参照してください．

**注 :** [`SuperComputer::PLAIN`](./cpp_super_computer.md) はこの関数の引数にすることができません．

**引数 :**

|引数|説明|
|:-:|:-:|
|`machine`|実行環境のスーパーコンピュータ|
|`script_path`|投入するジョブスクリプトまでのパス|

**戻り値 :** なし

### setLaunchCommand()

``` cpp
void setLaunchCommand(const std::string& command, std::string options = "");
void setLaunchCommand(const std::string& command, const std::vector<std::string>& options);
```

一般的なマシンでの実行と，実行するプログラムを指定します．

**注 :** コマンドを実行する場合は，環境変数でコマンドまでのパスが通っている必要があります．

**引数 :**

|引数|説明|
|:-:|:-:|
|`command`|実行するプログラムまでのパス，または実行するコマンド|
|`options`|プログラムに渡すコマンドライン引数，またはコマンドのオプション|

**戻り値 :** なし

## 管理処理の設定

### setCopiedDirectory()

``` cpp
void setCopiedDirectory(std::string directory_path);
```

プロセス実行の結果が次の実行 (性能) に影響を与えないよう，プロセス起動ごとにワーキングディレクトリをコピーし，保護します．

**注 :** 指定ディレクトリ以下の階層に `ProcessManager` 自身が動いているディレクトリ階層を含まないでください．`ProcessManager` 自身がコピーされることは想定していません．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`directory_path`|プロセスを起動するたびにコピーするワーキングディレクトリのパス|なし (コピーしない)|

**戻り値 :** なし

### config_switchWaitLimit()

``` cpp
void config_switchWaitLimit(bool limit_on = true);
```

プロセスの起動とプロセスの実行に対して制限時間を設け，時間経過でプロセスを強制終了するかを設定します．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`limit_on`|制限時間経過でプロセスを強制終了する場合 `true`|`false`|

**戻り値 :** なし

### setWatchDurationOfLaunch()

``` cpp
void setWatchDurationOfLaunch(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
void setWatchDurationOfLaunch(std::size_t hours, std::size_t minutes, std::size_t seconds);
```

プロセス起動時，プロセスが起動したかどうかを確認する頻度を指定します．  
[`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定しない場合，この値は無視されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`hours`|プロセス起動確認頻度の「時間」|`0`|
|`minutes`|プロセス起動確認頻度の「分」|`0`|
|`seconds`|プロセス起動確認頻度の「秒」|`2`|

**戻り値 :** なし

### setWaitDurationOfLaunch()

``` cpp
void setWaitDurationOfLaunch(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
void setWaitDurationOfLaunch(std::size_t hours, std::size_t minutes, std::size_t seconds);
```

プロセス起動時，プロセスが強制終了されるまでの制限時間を設定します．  
[`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定しない場合，この値は無視されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`hours`|プロセス起動確認頻度の「時間」|`0`|
|`minutes`|プロセス起動確認頻度の「分」|`0`|
|`seconds`|プロセス起動確認頻度の「秒」|`30`|

**戻り値 :** なし

### setWatchDurationOfRunning()

``` cpp
void setWatchDurationOfRunning(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
void setWatchDurationOfRunning(std::size_t hours, std::size_t minutes, std::size_t seconds);
```

プロセス実行時，プロセスが終了したかどうかを確認する頻度を指定します．  
[`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定しない場合，この値は無視されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`hours`|プロセス起動確認頻度の「時間」|`0`|
|`minutes`|プロセス起動確認頻度の「分」|`1`|
|`seconds`|プロセス起動確認頻度の「秒」|`0`|

**戻り値 :** なし

### setWaitDurationOfRunning()

``` cpp
void setWaitDurationOfRunning(std::chrono::hours hours, std::chrono::minutes minutes, std::chrono::seconds seconds);
void setWaitDurationOfRunning(std::size_t hours, std::size_t minutes, std::size_t seconds);
```

プロセス実行時，プロセスが強制終了されるまでの制限時間を設定します．  
[`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定しない場合，この値は無視されます．

**引数 :**

|引数|説明|設定前初期値|
|:-:|:-:|:-:|
|`hours`|プロセス起動確認頻度の「時間」|`0`|
|`minutes`|プロセス起動確認頻度の「分」|`30`|
|`seconds`|プロセス起動確認頻度の「秒」|`0`|

**戻り値 :** なし

## プロセスの管理

### launchProcess()

``` cpp
bool launchProcess(std::vector<T> parameter, bool with_file = true);
```

[`setJobScript()`](#setjobscript) で設定したジョブスクリプトを投入，または [`setLaunchCommand()`](#setlaunchcommand) で設定したプログラムやコマンドを起動し，性能パラメタ設定値を渡します．

**注 :** スーパーコンピュータにジョブ投入をする場合，第 2 引数 `with_file` には値を設定しないでください．

**引数 :**

|引数|説明|
|:-:|:-:|
|`parameter`|ジョブやプログラム，コマンドに渡す性能パラメタ設定値|
|`with_file`|性能パラメタ設定値を一時ファイルで渡す場合 `true`，コマンドライン引数で渡す場合 `false`|

**戻り値 :**

問題なく起動されれば `true`，強制終了された場合 `false`．  
[`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定しない場合，戻り値は常に `true` です．

### waitAll()

``` cpp
bool waitAll(void) const;
```

[`launchProcess()`](#launchprocess) で起動したすべてのプロセスが終了するまで待機します．  
ただし [`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定した場合，制限時間経過時に実行中のプロセスは強制終了されます．

**引数 :** なし

**戻り値 :**

すべてのプロセスが正常終了すれば `true`，制限時間で強制終了されたプロセスがあれば `false`．  
[`config_switchWaitLimit()`](#config_switchwaitlimit) で制限時間を使うように設定しない場合，戻り値は常に `true` です．

### aggregateResults()

``` cpp
std::vector<double> aggregateResults(void);
```

前回 [`aggregateResults()`](#aggregateresults) を呼び出したとき以降に起動されたプロセスの結果を確認し，性能評価値を収集します．

**引数 :** なし

**戻り値 :**

収集した性能評価値の一覧．  
性能評価値は，プロセスの起動順で格納されます．  
また，制限時間などで結果を得られなかったプロセスについては，代わりに `NaN` (`std::numeric_limits<double>::quiet_NaN()`) が格納されます．

## その他

### resetEnvironment()

``` cpp
void resetEnvironment(void) const;
```

プロセス起動時の一時ファイルやコピーされたディレクトリなどを削除します．

**引数 :** なし

**戻り値 :** なし
