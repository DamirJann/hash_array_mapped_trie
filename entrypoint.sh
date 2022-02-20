#!/usr/bin/env bash
cd "$GITHUB_WORKSPACE" || exit 1

ln -sfn ${GTEST_DIR} ./test/gtest
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=clang-10 -DCMAKE_CXX_COMPILER=clang++-10 ../
make

pwd
for f in $(find test_list -type f -executable); do
  echo "running test suite " $f
  ./$f
done