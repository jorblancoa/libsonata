#!/usr/bin/env bash

# This file runs the C++ tests, as well as compiling the code with warnings on
# so that errors should be caught quicker

set -euxo pipefail

rm -rf build
mkdir build
cd build
cmake                           \
    -DCMAKE_BUILD_TYPE=Release  \
    -DEXTLIB_FROM_SUBMODULES=ON \
    -DSONATA_CXX_WARNINGS=ON    \
    ..
make -j2
make test
