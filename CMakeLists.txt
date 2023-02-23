cmake_minimum_required(VERSION 3.24)
project(c-patterns C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)
set(C_FLAGS "-Wall -Wuninitialized -Wmissing-include-dirs -Wextra -Wconversion -Werror -Wfatal-errors -Wformat")

add_executable(pattern1 src/pattern1.c)
set_target_properties(
        pattern1
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin EXCLUDE_FROM_ALL off)

add_executable(pattern2 src/pattern2.c)
set_target_properties(
        pattern2
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin EXCLUDE_FROM_ALL off)
