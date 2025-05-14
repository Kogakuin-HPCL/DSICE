# DSICE

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

### Version 1.0.1+docs (2025-05-14)

- Other Updates
  - Bug fixes in the code of the tutorials and additions to the corresponding explanations
  - Fixed the problem that the compiler option `-lDSICE` is not explained anywhere in the documentation.
- Under Consideration for Improvement
  - There is no lower limit on the number of elements in the list returned by `getSuggestedList()`, and this may not be sufficient to run in parallel during the latter phases.
