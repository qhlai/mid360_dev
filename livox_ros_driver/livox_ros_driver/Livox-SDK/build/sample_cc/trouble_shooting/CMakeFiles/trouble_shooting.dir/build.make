# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build

# Include any dependencies generated for this target.
include sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/depend.make

# Include the progress variables for this target.
include sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/progress.make

# Include the compile flags for this target's objects.
include sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/flags.make

sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/main.cpp.o: sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/flags.make
sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/main.cpp.o: ../sample_cc/trouble_shooting/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/main.cpp.o"
	cd /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/trouble_shooting.dir/main.cpp.o -c /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/sample_cc/trouble_shooting/main.cpp

sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/trouble_shooting.dir/main.cpp.i"
	cd /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/sample_cc/trouble_shooting/main.cpp > CMakeFiles/trouble_shooting.dir/main.cpp.i

sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/trouble_shooting.dir/main.cpp.s"
	cd /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/sample_cc/trouble_shooting/main.cpp -o CMakeFiles/trouble_shooting.dir/main.cpp.s

# Object files for target trouble_shooting
trouble_shooting_OBJECTS = \
"CMakeFiles/trouble_shooting.dir/main.cpp.o"

# External object files for target trouble_shooting
trouble_shooting_EXTERNAL_OBJECTS =

sample_cc/trouble_shooting/trouble_shooting: sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/main.cpp.o
sample_cc/trouble_shooting/trouble_shooting: sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/build.make
sample_cc/trouble_shooting/trouble_shooting: sdk_core/liblivox_sdk_static.a
sample_cc/trouble_shooting/trouble_shooting: sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable trouble_shooting"
	cd /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/trouble_shooting.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/build: sample_cc/trouble_shooting/trouble_shooting

.PHONY : sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/build

sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/clean:
	cd /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting && $(CMAKE_COMMAND) -P CMakeFiles/trouble_shooting.dir/cmake_clean.cmake
.PHONY : sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/clean

sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/depend:
	cd /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/sample_cc/trouble_shooting /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting /home/lqh/ros/pc_ws/src/livox_ros_driver/livox_ros_driver/Livox-SDK/build/sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sample_cc/trouble_shooting/CMakeFiles/trouble_shooting.dir/depend

