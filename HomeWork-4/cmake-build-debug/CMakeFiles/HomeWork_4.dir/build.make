# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
CMAKE_COMMAND = /opt/clion-2017.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2017.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/victor/githubRepos/BI-PA2/HomeWork-4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HomeWork_4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HomeWork_4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HomeWork_4.dir/flags.make

CMakeFiles/HomeWork_4.dir/main.cpp.o: CMakeFiles/HomeWork_4.dir/flags.make
CMakeFiles/HomeWork_4.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HomeWork_4.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/HomeWork_4.dir/main.cpp.o -c /home/victor/githubRepos/BI-PA2/HomeWork-4/main.cpp

CMakeFiles/HomeWork_4.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HomeWork_4.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victor/githubRepos/BI-PA2/HomeWork-4/main.cpp > CMakeFiles/HomeWork_4.dir/main.cpp.i

CMakeFiles/HomeWork_4.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HomeWork_4.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victor/githubRepos/BI-PA2/HomeWork-4/main.cpp -o CMakeFiles/HomeWork_4.dir/main.cpp.s

CMakeFiles/HomeWork_4.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/HomeWork_4.dir/main.cpp.o.requires

CMakeFiles/HomeWork_4.dir/main.cpp.o.provides: CMakeFiles/HomeWork_4.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/HomeWork_4.dir/build.make CMakeFiles/HomeWork_4.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/HomeWork_4.dir/main.cpp.o.provides

CMakeFiles/HomeWork_4.dir/main.cpp.o.provides.build: CMakeFiles/HomeWork_4.dir/main.cpp.o


# Object files for target HomeWork_4
HomeWork_4_OBJECTS = \
"CMakeFiles/HomeWork_4.dir/main.cpp.o"

# External object files for target HomeWork_4
HomeWork_4_EXTERNAL_OBJECTS =

HomeWork_4: CMakeFiles/HomeWork_4.dir/main.cpp.o
HomeWork_4: CMakeFiles/HomeWork_4.dir/build.make
HomeWork_4: CMakeFiles/HomeWork_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HomeWork_4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HomeWork_4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HomeWork_4.dir/build: HomeWork_4

.PHONY : CMakeFiles/HomeWork_4.dir/build

CMakeFiles/HomeWork_4.dir/requires: CMakeFiles/HomeWork_4.dir/main.cpp.o.requires

.PHONY : CMakeFiles/HomeWork_4.dir/requires

CMakeFiles/HomeWork_4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HomeWork_4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HomeWork_4.dir/clean

CMakeFiles/HomeWork_4.dir/depend:
	cd /home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/githubRepos/BI-PA2/HomeWork-4 /home/victor/githubRepos/BI-PA2/HomeWork-4 /home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug /home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug /home/victor/githubRepos/BI-PA2/HomeWork-4/cmake-build-debug/CMakeFiles/HomeWork_4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HomeWork_4.dir/depend
