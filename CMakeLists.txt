cmake_minimum_required(VERSION 3.16)
project(hash_array_mapped_trie)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS -pthread)

build_)
# MAIN TARGET
add_executable(
        hamt include/visualize.h main.cpp src/utils.cpp
)
