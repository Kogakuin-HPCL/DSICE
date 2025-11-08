# DSICE

[![C++](https://img.shields.io/badge/C%2B%2B-20%2B-blue)](https://isocpp.org/)
[![C](https://img.shields.io/badge/C-ISO%20C-blue)](https://www.open-std.org/JTC1/SC22/WG14/)
[![Python](https://img.shields.io/badge/Python-3.x-blue)](https://www.python.org)
[![Version](https://img.shields.io/github/v/tag/Kogakuin-HPCL/DSICE?sort=semver&label=Version)](./CHANGELOG.md)
[![CMake](https://img.shields.io/badge/CMake-%E2%89%A53.14-blue)](https://cmake.org/)
[![License](https://img.shields.io/github/license/Kogakuin-HPCL/DSICE)](./LICENSE)

[English](./README.md) | [日本語](./README-ja.md)

## Introduction

DSICE is a parameter tuning library using the fitting function "d-Spline."

## Supported Environment

DSICE is intended for use on the following platforms.

- Linux
- Windows

The programming languages in which the library is available are as follows.

- C
- C++
- Python3

## Requirements

The following are required for the build.

- CMake (version 3.14 and more)
- The C++ compilers (supporting C++20 and more)
- Python3 (Note: if you use Python)

## Installation

For installation, refer to the "[導入](docs/installation/installation_top.md)" in the documentation.  
Note: the documentation is now available in Japanese only.

## Documentation

Please refer to the [documentation](docs/documents_home.md) for usage instructions.  
Note: the documentation is now available in Japanese only.

## Release Note

Refer to the [change log](./CHANGELOG.md) for past updates.

### Version 1.0.1+docs.license (2025-11-08)

- Other Updates
  - Added a license file to clarify project terms.
  - Added status badges to the README for better visibility.
- Under Consideration for Improvement
  - There is no lower limit on the number of elements in the list returned by `getSuggestedList()`, and this may not be sufficient to run in parallel during the latter phases. [Ongoing]
