cmake_minimum_required(VERSION 3.10)
project(chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(chip8 src/main.cpp src/chip8.cpp src/memory.cpp src/display.cpp src/input.cpp src/register.cpp src/stack.cpp)

target_link_libraries(chip8 ${SDL2_LIBRARIES})