cmake_minimum_required(VERSION 3.10)
project(chip8)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(chip8 src/main.cpp src/chip8.cpp src/memory.cpp src/display.cpp src/input.cpp)