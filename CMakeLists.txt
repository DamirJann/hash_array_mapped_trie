cmake_minimum_required(VERSION 3.16.)
project(hash_array_mapped_trie)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Google_tests/lib/googletest googletests)

# MAIN TARGET
add_executable(
        hash_array_mapped_trie
        src/trie.cpp src/utils.cpp main.cpp
)

# TEST TARGET
add_executable(
        test_list
        test/test.cpp src/utils.cpp src/trie.cpp
)


target_link_libraries(
        test_list PRIVATE
        gtest gtest_main
)

#COMPILATION FLAGS
target_compile_options(
        hash_array_mapped_trie PRIVATE
        -Wall -Wextra -pedantic -Werror
)

# COMPILATION FLAGS
#target_compile_options(
#        test_list PRIVATE
#        -Wall -Wextra -pedantic -Werror
#)
