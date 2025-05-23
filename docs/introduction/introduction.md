# はじめに

この章では，DSICE の概要を紹介します．

## DSICE の概要

DSICE は，ソフトウェア自動チューニングを実現するためのライブラリ，およびツールです．

## ターゲット環境

### OS

DSICE は次の OS での使用を想定しています．

- Linux
- Windows

### プログラミング言語

DSICE は次のプログラミング言語で使用可能です．

- C
- C++
- Python3

## 使用上の制約 (DSICE における予約語)

DSICE では `dsice` または `DSICE` からはじまる文字列が予約語として使用されています．特に，名前空間による住み分けが困難な場合 (純粋 C や**一時ファイルの名称**など) に多用されています．  
DSICE を使う際は変数やファイルなどに対し，`dsice` または `DSICE` からはじまる名前を使用しないでください．使用した場合，予期しない動作を招く可能性があります．
