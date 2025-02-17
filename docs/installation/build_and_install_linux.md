# Linux でのビルドとインストール

## 必須要件

ビルドおよびインストールには次のものが必要です．

- CMake (version 3.14 以上)
- C++20 をサポートする コンパイラ
  - GCC version 9.1 以上
  - Intel oneAPI 2021 以上
  - NVIDIA HPC SDK 21 以上
- Python3 (Python を使う場合)

## コアライブラリのビルド

1. [GitHub の公開リポジトリ](https://github.com/Kogakuin-HPCL/DSICE)をクローン，またはダウンロードし，dsice リポジトリ内に入ります．

    ``` bash
    git clone https://github.com/Kogakuin-HPCL/DSICE.git
    cd dsice
    ```

2. ビルド用のディレクトリを作成し，入ります．

    ``` bash
    mkdir build
    cd build
    ```

3. CMake の設定を行います．詳細な設定項目は [CMake 公式ドキュメント](https://cmake.org/cmake/help/latest/index.html) を参照してください．

    まず，次のコマンドを実行します．

    ``` bash
    cmake ..
    ```

    上のコマンドを実行すると，デフォルトの設定内容が表示されます．

    ``` bash
    -- Information :
    --     Project Name =         DSICE
    --     Version =              1.0.0
    --     Description =          The automatic parameter tuning library.
    --     CMAKE_VERSION =        3.22.1
    --     CMAKE_CXX_COMPILER =   /usr/bin/c++
    --     CMAKE_INSTALL_PREFIX = /usr/local
    --     CMAKE_BUILD_TYPE =
    --     CMAKE_INSTALL_LIBDIR =
    ```

    `CMAKE_CXX_COMPILER` に表示されたコンパイラが C++20 をサポートしているコンパイラであること，`CMAKE_INSTALL_PREFIX` が最終的にインストールされるべきディレクトリであることを確認してください．このままで問題なければ手順 4 に進んでください．  
    別のコンパイラを指定する場合は `-DCMAKE_CXX_COMPILER` で指定できます．

    ``` bash
    cmake ..  -DCMAKE_CXX_COMPILER=g++
    ```

    また，インストール先を指定する場合は `-DCMAKE_INSTALL_PREFIX` で指定できます．

    ``` bash
    cmake .. -DCMAKE_INSTALL_PREFIX=../install_dst
    ```

4. 設定が終わったらビルドを行います．

    ``` bash
    cmake --build .
    ```

5. エラーが表示されずに終了すれば完了です．

## C/C++ 向けインストール

続いて，C/C++ 向けのインストールを行います．C/C++ で DSICE を使用しない場合はこの手順は不要です．

1. 先ほどの `build` ディレクトリの中で，次のコマンドを実行します．

    ``` bash
    cmake --install .
    ```

    ※ `Permission denied` のエラーが出る場合は管理者権限が必要です．次のように `sudo` をつけて実行します．

    ``` bash
    sudo cmake --install .
    ```

    ※ 管理者権限が必要ないディレクトリにインストールするためには[コアライブラリのビルド](#コアライブラリのビルド)の手順 3 に戻り，`-DCMAKE_INSTALL_PREFIX` の設定を行ってください．

以上で，C/C++ 向けインストールは完了です．

なお，インストール先によっては環境変数の設定が必要な可能性があります．  
インストール時，`build` ディレクトリに `setup_env.sh` が生成されますので，必要に応じて実行してください (環境変数の設定はログインするたびに必要です)．

``` bash
source setup_env.sh
```

## Python 向けパッケージのビルドとインストール

Python 向けのインストールを行います．Python で DSICE を使用しない場合はこの手順は不要です．

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

3. ビルドされたパッケージの名前を確認します．次のコマンドの結果から，`.whl` という拡張子が付いたファイルを探してください．パッケージ名は Python や DSICE のバージョンごとに異なります．

    ``` bash
    ls dist
    ```

4. ビルドされたパッケージをインストールします．以下のコマンドの `dsice-1.0.0-py3-none-any.whl` の部分は，先ほどの手順 3 で名前を確認した `.whl` ファイルに置き換えてください．

    ``` bash
    python3 -m pip install dist/dsice-1.0.0-py3-none-any.whl
    ```

以上で Python 向けパッケージのビルドとインストールは完了です．
