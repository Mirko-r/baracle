cmake_minimum_required(VERSION 3.0)

project(baracle)

set(SOURCES ../src/baracle.c)

add_executable(baracle ${SOURCES})

target_compile_options(baracle PUBLIC -Wall -Wextra -pedantic -std=c99)
