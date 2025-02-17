# Windows でのビルドとインストール

## 必須要件

ビルドおよびインストールには次のものが必要です．

- CMake (version 3.14 以上)
- C++20 をサポートする コンパイラ
  - Visual Studio 2019 以上
- Python3 (Python を使う場合)

## ビルド用ファイルの生成

### CUI (PowerShell) を使用したビルド用ファイルの生成する

1. [GitHub の公開リポジトリ](https://github.com/Kogakuin-HPCL/DSICE)をクローン，またはダウンロードし，dsice リポジトリ内に入ります．

    ``` powershell
    git clone https://github.com/Kogakuin-HPCL/DSICE.git
    cd dsice
    ```

2. ビルド用のフォルダを作成し，入ります．

    ``` powershell
    New-Item build -ItemType Directory
    cd build
    ```

3. CMake の設定を行います．詳細な設定項目は [CMake 公式ドキュメント](https://cmake.org/cmake/help/latest/index.html) を参照してください．

    まず，次のコマンドを実行します．

    ``` powershell
    cmake ..
    ```

    上のコマンドを実行すると，デフォルトの設定内容が表示されます．

    ``` powershell
    --     Project Name =         DSICE
    --     Version =              1.0.0
    --     Description =          The automatic parameter tuning library.
    --     CMAKE_VERSION =        3.31.0-rc2
    --     CMAKE_CXX_COMPILER =   C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.42.34433/bin/Hostx64/x64/cl.exe
    --     CMAKE_INSTALL_PREFIX = C:/Program Files (x86)/DSICE
    --     CMAKE_BUILD_TYPE =
    --     CMAKE_INSTALL_LIBDIR =
    ```

    このままで問題なければ[コアライブラリのビルド](#コアライブラリのビルド)に進んでください．  
    インストール先を指定する場合は，`-DCMAKE_INSTALL_PREFIX` で指定できます．

    ``` powershell
    cmake .. -DCMAKE_INSTALL_PREFIX="../install_dst"
    ```

### GUI (CMake GUI) を使用したビルド用ファイルの生成

## コアライブラリのビルド

### CUI (PowerShell) を使用したビルド

1. 先ほどの `build` フォルダ内でビルドを実行します．

    ``` powershell
    cmake --build . --config Release
    ```

2. エラーが表示されずに終了すれば完了です．

### GUI (Visual Studio) を使用したビルド

## C/C++ 向けインストール

続いて，C/C++ 向けのインストールを行います．C/C++ で DSICE を使用しない場合はこの手順は不要です．

### CUI (PowerShell) を使用した C/C++ 向けインストール

1. 先ほどの `build` ディレクトリの中で，次のコマンドを実行します (※ PowerShell を管理者権限で起動する必要がある可能性があります)．

    ``` bash
    cmake --install .
    ```

### GUI (Visual Studio) を使用した C/C++ 向けインストール

## Python 向けパッケージのビルドとインストール

Python 向けパッケージのビルドとインストールは CUI (PowerShell) で行います．  
Python で DSICE を使用しない場合はこの手順は不要です．

1. 先ほどの `build` ディレクトリから，リポジトリルート直下の `python` ディレクトリに移動します．

    ``` bash
    cd ../python
    ```

2. Python パッケージをビルドします．

    ``` bash
    python3 -m build
    ```

    ※ ここで `No module named build` が出る場合は `build` モジュールを入れてから再実行します．

    ``` bash
    python3 -m pip install build
    python3 -m build
    ```

3. ビルドされたパッケージをインストールします．パッケージ名は Python や DSICE のバージョンごとに異なるため注意してください．

    ``` bash
    python3 -m pip install dist/dsice-1.0.0-py3-none-any.whl
    ```

以上で Python 向けパッケージのビルドとインストールは完了です．
