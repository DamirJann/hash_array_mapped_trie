# CMake generated Testfile for 
# Source directory: /home/damire/space/hash_array_mapped_trie/CppBenchmark
# Build directory: /home/damire/space/hash_array_mapped_trie/cmake-build-debug-coverage/CppBenchmark
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[cppbenchmark-tests]=] "cppbenchmark-tests" "--durations" "yes" "--order" "lex")
set_tests_properties([=[cppbenchmark-tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/damire/space/hash_array_mapped_trie/CppBenchmark/CMakeLists.txt;82;add_test;/home/damire/space/hash_array_mapped_trie/CppBenchmark/CMakeLists.txt;0;")
subdirs("modules")
