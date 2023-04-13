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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb

# Include any dependencies generated for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compiler_depend.make

# Include the progress variables for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/progress.make

# Include the compile flags for this target's objects.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/flags.make

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/flags.make
pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj: /home/tarnished-dan22/RTOS_SP2023/pico/pico-sdk/src/rp2_common/boot_stage2/compile_time_choice.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj"
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/bs2_default.dir/compile_time_choice.S.obj   -c /home/tarnished-dan22/RTOS_SP2023/pico/pico-sdk/src/rp2_common/boot_stage2/compile_time_choice.S

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing ASM source to CMakeFiles/bs2_default.dir/compile_time_choice.S.i"
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -E /home/tarnished-dan22/RTOS_SP2023/pico/pico-sdk/src/rp2_common/boot_stage2/compile_time_choice.S > CMakeFiles/bs2_default.dir/compile_time_choice.S.i

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling ASM source to assembly CMakeFiles/bs2_default.dir/compile_time_choice.S.s"
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -S /home/tarnished-dan22/RTOS_SP2023/pico/pico-sdk/src/rp2_common/boot_stage2/compile_time_choice.S -o CMakeFiles/bs2_default.dir/compile_time_choice.S.s

# Object files for target bs2_default
bs2_default_OBJECTS = \
"CMakeFiles/bs2_default.dir/compile_time_choice.S.obj"

# External object files for target bs2_default
bs2_default_EXTERNAL_OBJECTS =

pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj
pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/build.make
pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: /home/tarnished-dan22/RTOS_SP2023/pico/pico-sdk/src/rp2_common/boot_stage2/boot_stage2.ld
pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM executable bs2_default.elf"
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bs2_default.dir/link.txt --verbose=$(VERBOSE)
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-objdump -h /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf > bs2_default.dis
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-objdump -d /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf >> bs2_default.dis

# Rule to build all files generated by this target.
pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/build: pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/build

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/clean:
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 && $(CMAKE_COMMAND) -P CMakeFiles/bs2_default.dir/cmake_clean.cmake
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/clean

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/depend:
	cd /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb /home/tarnished-dan22/RTOS_SP2023/pico/pico-sdk/src/rp2_common/boot_stage2 /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2 /home/tarnished-dan22/RTOS_SP2023/pico/simple_usb/pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/depend

