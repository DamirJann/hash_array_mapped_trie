#!/usr/bin/env bash
cd "$GITHUB_WORKSPACE" || exit 1
cd lib
make -j googletest
cd ..
cd test
cmake  -DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10 .
make
./unit_test
