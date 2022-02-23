#!/usr/bin/env bash

cd "$GITHUB_WORKSPACE" || exit 1
cd test

cmake  -DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10 .
make

./run_tests
