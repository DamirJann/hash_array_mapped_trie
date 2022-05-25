#!/usr/bin/env bash

cd "$GITHUB_WORKSPACE" || exit 1
cd test

cmake  -DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10 .
make

<<<<<<< HEAD
./run_tests



=======
./unit_test
>>>>>>> 259317fe4d8279848742824dc553a976fea57d2b
