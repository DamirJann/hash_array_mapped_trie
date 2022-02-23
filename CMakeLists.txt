cmake_minimum_required(VERSION 3.16)
project(hash_array_mapped_trie)

set(CMAKE_CXX_STANDARD 14)

# MAIN TARGET
add_executable(
        hamt src/trie.cpp main.cpp src/utils.cpp src/visualize.cpp
)

# COMPILATION FLAGS
#target_compile_options(
#        hamt PRIVATE
#        -Wall -Wextra -pedantic -Werror
#)

