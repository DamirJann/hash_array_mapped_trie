# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/damire/.local/lib/python3.8/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/damire/.local/lib/python3.8/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/damire/space/hash_array_mapped_trie/lib/benchmark

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build

# Include any dependencies generated for this target.
include test/CMakeFiles/benchmark_gtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/benchmark_gtest.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/benchmark_gtest.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/benchmark_gtest.dir/flags.make

test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o: test/CMakeFiles/benchmark_gtest.dir/flags.make
test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o: /home/damire/space/hash_array_mapped_trie/lib/benchmark/test/benchmark_gtest.cc
test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o: test/CMakeFiles/benchmark_gtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o"
	cd /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o -MF CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o.d -o CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o -c /home/damire/space/hash_array_mapped_trie/lib/benchmark/test/benchmark_gtest.cc

test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.i"
	cd /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/damire/space/hash_array_mapped_trie/lib/benchmark/test/benchmark_gtest.cc > CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.i

test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.s"
	cd /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/damire/space/hash_array_mapped_trie/lib/benchmark/test/benchmark_gtest.cc -o CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.s

# Object files for target benchmark_gtest
benchmark_gtest_OBJECTS = \
"CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o"

# External object files for target benchmark_gtest
benchmark_gtest_EXTERNAL_OBJECTS =

test/benchmark_gtest: test/CMakeFiles/benchmark_gtest.dir/benchmark_gtest.cc.o
test/benchmark_gtest: test/CMakeFiles/benchmark_gtest.dir/build.make
test/benchmark_gtest: src/libbenchmark.a
test/benchmark_gtest: lib/libgmock_main.a
test/benchmark_gtest: lib/libgmock.a
test/benchmark_gtest: lib/libgtest.a
test/benchmark_gtest: test/CMakeFiles/benchmark_gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable benchmark_gtest"
	cd /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/benchmark_gtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/benchmark_gtest.dir/build: test/benchmark_gtest
.PHONY : test/CMakeFiles/benchmark_gtest.dir/build

test/CMakeFiles/benchmark_gtest.dir/clean:
	cd /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test && $(CMAKE_COMMAND) -P CMakeFiles/benchmark_gtest.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/benchmark_gtest.dir/clean

test/CMakeFiles/benchmark_gtest.dir/depend:
	cd /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/damire/space/hash_array_mapped_trie/lib/benchmark /home/damire/space/hash_array_mapped_trie/lib/benchmark/test /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test /home/damire/space/hash_array_mapped_trie/lib/benchmark-release/build/test/CMakeFiles/benchmark_gtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/benchmark_gtest.dir/depend

