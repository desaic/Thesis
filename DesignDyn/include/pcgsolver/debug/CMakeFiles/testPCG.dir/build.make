# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/desaic/workspace/DesignDyn/pcgsolver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/desaic/workspace/DesignDyn/pcgsolver/debug

# Include any dependencies generated for this target.
include CMakeFiles/testPCG.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testPCG.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testPCG.dir/flags.make

CMakeFiles/testPCG.dir/test_pcg.cpp.o: CMakeFiles/testPCG.dir/flags.make
CMakeFiles/testPCG.dir/test_pcg.cpp.o: ../test_pcg.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/desaic/workspace/DesignDyn/pcgsolver/debug/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/testPCG.dir/test_pcg.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/testPCG.dir/test_pcg.cpp.o -c /home/desaic/workspace/DesignDyn/pcgsolver/test_pcg.cpp

CMakeFiles/testPCG.dir/test_pcg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testPCG.dir/test_pcg.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/desaic/workspace/DesignDyn/pcgsolver/test_pcg.cpp > CMakeFiles/testPCG.dir/test_pcg.cpp.i

CMakeFiles/testPCG.dir/test_pcg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testPCG.dir/test_pcg.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/desaic/workspace/DesignDyn/pcgsolver/test_pcg.cpp -o CMakeFiles/testPCG.dir/test_pcg.cpp.s

CMakeFiles/testPCG.dir/test_pcg.cpp.o.requires:
.PHONY : CMakeFiles/testPCG.dir/test_pcg.cpp.o.requires

CMakeFiles/testPCG.dir/test_pcg.cpp.o.provides: CMakeFiles/testPCG.dir/test_pcg.cpp.o.requires
	$(MAKE) -f CMakeFiles/testPCG.dir/build.make CMakeFiles/testPCG.dir/test_pcg.cpp.o.provides.build
.PHONY : CMakeFiles/testPCG.dir/test_pcg.cpp.o.provides

CMakeFiles/testPCG.dir/test_pcg.cpp.o.provides.build: CMakeFiles/testPCG.dir/test_pcg.cpp.o

# Object files for target testPCG
testPCG_OBJECTS = \
"CMakeFiles/testPCG.dir/test_pcg.cpp.o"

# External object files for target testPCG
testPCG_EXTERNAL_OBJECTS =

testPCG: CMakeFiles/testPCG.dir/test_pcg.cpp.o
testPCG: CMakeFiles/testPCG.dir/build.make
testPCG: CMakeFiles/testPCG.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable testPCG"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testPCG.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testPCG.dir/build: testPCG
.PHONY : CMakeFiles/testPCG.dir/build

CMakeFiles/testPCG.dir/requires: CMakeFiles/testPCG.dir/test_pcg.cpp.o.requires
.PHONY : CMakeFiles/testPCG.dir/requires

CMakeFiles/testPCG.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testPCG.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testPCG.dir/clean

CMakeFiles/testPCG.dir/depend:
	cd /home/desaic/workspace/DesignDyn/pcgsolver/debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/desaic/workspace/DesignDyn/pcgsolver /home/desaic/workspace/DesignDyn/pcgsolver /home/desaic/workspace/DesignDyn/pcgsolver/debug /home/desaic/workspace/DesignDyn/pcgsolver/debug /home/desaic/workspace/DesignDyn/pcgsolver/debug/CMakeFiles/testPCG.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testPCG.dir/depend

