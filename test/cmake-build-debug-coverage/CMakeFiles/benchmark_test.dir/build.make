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
CMAKE_COMMAND = /snap/clion/190/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/190/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/damire/space/hash_array_mapped_trie/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage

# Include any dependencies generated for this target.
include CMakeFiles/benchmark_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/benchmark_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/benchmark_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/benchmark_test.dir/flags.make

CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o: CMakeFiles/benchmark_test.dir/flags.make
CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o: ../benchmark_test.cpp
CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o: CMakeFiles/benchmark_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o -MF CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o.d -o CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o -c /home/damire/space/hash_array_mapped_trie/test/benchmark_test.cpp

CMakeFiles/benchmark_test.dir/benchmark_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmark_test.dir/benchmark_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/damire/space/hash_array_mapped_trie/test/benchmark_test.cpp > CMakeFiles/benchmark_test.dir/benchmark_test.cpp.i

CMakeFiles/benchmark_test.dir/benchmark_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmark_test.dir/benchmark_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/damire/space/hash_array_mapped_trie/test/benchmark_test.cpp -o CMakeFiles/benchmark_test.dir/benchmark_test.cpp.s

CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o: CMakeFiles/benchmark_test.dir/flags.make
CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o: /home/damire/space/hash_array_mapped_trie/src/utils.cpp
CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o: CMakeFiles/benchmark_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o -MF CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o.d -o CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o -c /home/damire/space/hash_array_mapped_trie/src/utils.cpp

CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/damire/space/hash_array_mapped_trie/src/utils.cpp > CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.i

CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/damire/space/hash_array_mapped_trie/src/utils.cpp -o CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.s

# Object files for target benchmark_test
benchmark_test_OBJECTS = \
"CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o" \
"CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o"

# External object files for target benchmark_test
benchmark_test_EXTERNAL_OBJECTS =

benchmark_test: CMakeFiles/benchmark_test.dir/benchmark_test.cpp.o
benchmark_test: CMakeFiles/benchmark_test.dir/home/damire/space/hash_array_mapped_trie/src/utils.cpp.o
benchmark_test: CMakeFiles/benchmark_test.dir/build.make
benchmark_test: CMakeFiles/benchmark_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable benchmark_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/benchmark_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/benchmark_test.dir/build: benchmark_test
.PHONY : CMakeFiles/benchmark_test.dir/build

CMakeFiles/benchmark_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/benchmark_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/benchmark_test.dir/clean

CMakeFiles/benchmark_test.dir/depend:
	cd /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/damire/space/hash_array_mapped_trie/test /home/damire/space/hash_array_mapped_trie/test /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug-coverage/CMakeFiles/benchmark_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/benchmark_test.dir/depend

