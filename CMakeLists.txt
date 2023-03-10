cmake_minimum_required(VERSION 3.24)
project(c-patterns C)
enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(C_FLAGS "-Wall -Wuninitialized -Wmissing-include-dirs -Wextra -Wconversion -Werror -Wfatal-errors -Wformat")

# Path configuration

set(BIN_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Add library to the project

add_library(resource_manager STATIC
        src/resource_manager/resource_manager.c
        src/resource_manager/resource_manager.h
        src/resource_manager/rm_mem.c
        src/resource_manager/rm_mem.h)

# Add executable to the project

add_executable(pattern1 src/pattern1.c)
add_executable(pattern2 src/pattern2.c)
add_executable(pattern3 src/pattern3.c)
add_executable(pattern4 src/pattern4.c src/pattern4.h)
add_executable(pattern5 src/pattern5.c src/pattern5/s_alloc.c src/pattern5/s_alloc.h src/pattern5/common.h)

# Set properties for all executables

set_target_properties(
        pattern1 pattern2 pattern3 pattern4 pattern5
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIRECTORY}
                   EXCLUDE_FROM_ALL off)

# Create the list of programs to execute in order to run the tests suite

add_test(test_program1  ${BIN_DIRECTORY}/pattern1)
add_test(test_program2  ${BIN_DIRECTORY}/pattern2)
add_test(test_program3  ${BIN_DIRECTORY}/pattern3)
add_test(test_program4  ${BIN_DIRECTORY}/pattern4)
add_test(test_program5  ${BIN_DIRECTORY}/pattern5)

