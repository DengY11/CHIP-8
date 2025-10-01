cmake_minimum_required(VERSION 3.14)
project(chip8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Main executable
add_executable(chip8 src/main.cpp src/chip8.cpp src/memory.cpp src/display.cpp src/input.cpp src/register.cpp src/stack.cpp src/test_access.cpp src/debugger.cpp src/debugger_cli.cpp)
target_link_libraries(chip8 ${SDL2_LIBRARIES})

# Google Test integration - using system installation
find_package(GTest REQUIRED)

# Test executable
add_executable(test_chip8 test/test_opcodes.cpp src/chip8.cpp src/memory.cpp src/display.cpp src/input.cpp src/register.cpp src/stack.cpp src/test_access.cpp src/debugger.cpp)
target_include_directories(test_chip8 PRIVATE src)
target_link_libraries(test_chip8 GTest::gtest_main ${SDL2_LIBRARIES})

# Add test
add_test(NAME Chip8Tests COMMAND test_chip8)