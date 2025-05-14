# DSICE

[English](README.md) | [日本語](README-ja.md)

## Introduction

DSICE は近似関数 d-Spline を用いたパラメタチューニングライブラリです．

## Supported Environment

DSICE は次のプラットフォームでの使用を想定しています．

- Linux
- Windows

ライブラリを使用可能なプログラミング言語は次のとおりです．

- C
- C++
- Python3

## Requirements

ビルドには次のものが必要です．

- CMake (version 3.14 以上)
- C++20 以上をサポートする コンパイラ
- Python3 系 (※ Python 使用時のみ)

## Installation

インストール方法についてはドキュメント内「[導入](docs/installation/installation_top.md)」を参照してください．

## Documentation

使い方については[ドキュメント](docs/documents_home.md)を参照してください．

## Release Note

過去の更新については[変更履歴](CHANGELOG-ja.md)を参照してください．

### Version 1.0.1+docs (2025-05-14)

- その他の更新
  - チュートリアルのコードに存在するバグの修正，および関連する説明の加筆
  - ドキュメントのどこにもコンパイル時のオプション `-lDSICE` の記述が無い問題への対処
- 改善検討中
  - `getSuggestedList()` が返すリストの要素数に下限がなく，探索最終版で十分な並列数を確保できない可能性
