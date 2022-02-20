#!/usr/bin/env bash

ls -l
cd hamt
ls -l
cmake -DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10
make

./test_list
