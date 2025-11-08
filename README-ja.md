# DSICE

[![C++](https://img.shields.io/badge/C%2B%2B-20%2B-blue)](https://isocpp.org/)
[![C](https://img.shields.io/badge/C-ISO%20C-blue)](https://www.open-std.org/JTC1/SC22/WG14/)
[![Python](https://img.shields.io/badge/Python-3.x-blue)](https://www.python.org)
[![Version](https://img.shields.io/github/v/tag/Kogakuin-HPCL/DSICE?sort=semver&label=Version)](./CHANGELOG.md)
[![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.14-blue)](https://cmake.org/)
[![License](https://img.shields.io/github/license/Kogakuin-HPCL/DSICE)](./LICENSE)

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

### Version 1.0.1+docs.license (2025-11-08)

- その他の更新
  - ライセンスファイルの追加
  - README へのバッジ追加
- 改善検討中
  - `getSuggestedList()` が返すリストの要素数に下限がなく，探索最終版で十分な並列数を確保できない可能性 [継続]
