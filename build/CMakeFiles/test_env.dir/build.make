# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Produce verbose output by default.
VERBOSE = 1

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /apps/mingo/bin/cmake

# The command to remove a file.
RM = /apps/mingo/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mingo/workspace/mingo_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mingo/workspace/mingo_server/build

# Include any dependencies generated for this target.
include CMakeFiles/test_env.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_env.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_env.dir/flags.make

CMakeFiles/test_env.dir/tests/test_env.cpp.o: CMakeFiles/test_env.dir/flags.make
CMakeFiles/test_env.dir/tests/test_env.cpp.o: ../tests/test_env.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mingo/workspace/mingo_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_env.dir/tests/test_env.cpp.o"
	/apps/mingo/bin/g++  $(CXX_DEFINES) -D__FILE__=\"tests/test_env.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_env.dir/tests/test_env.cpp.o -c /home/mingo/workspace/mingo_server/tests/test_env.cpp

CMakeFiles/test_env.dir/tests/test_env.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_env.dir/tests/test_env.cpp.i"
	/apps/mingo/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_env.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mingo/workspace/mingo_server/tests/test_env.cpp > CMakeFiles/test_env.dir/tests/test_env.cpp.i

CMakeFiles/test_env.dir/tests/test_env.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_env.dir/tests/test_env.cpp.s"
	/apps/mingo/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_env.cpp\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mingo/workspace/mingo_server/tests/test_env.cpp -o CMakeFiles/test_env.dir/tests/test_env.cpp.s

# Object files for target test_env
test_env_OBJECTS = \
"CMakeFiles/test_env.dir/tests/test_env.cpp.o"

# External object files for target test_env
test_env_EXTERNAL_OBJECTS =

../bin/test_env: CMakeFiles/test_env.dir/tests/test_env.cpp.o
../bin/test_env: CMakeFiles/test_env.dir/build.make
../bin/test_env: ../lib/libmingo.so
../bin/test_env: CMakeFiles/test_env.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mingo/workspace/mingo_server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_env"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_env.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_env.dir/build: ../bin/test_env

.PHONY : CMakeFiles/test_env.dir/build

CMakeFiles/test_env.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_env.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_env.dir/clean

CMakeFiles/test_env.dir/depend:
	cd /home/mingo/workspace/mingo_server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mingo/workspace/mingo_server /home/mingo/workspace/mingo_server /home/mingo/workspace/mingo_server/build /home/mingo/workspace/mingo_server/build /home/mingo/workspace/mingo_server/build/CMakeFiles/test_env.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_env.dir/depend
