#include <chrono>
#include <iostream>
#include <thread>

#include "chip8.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ROM file>" << std::endl;
        return 1;
    }
    std::string path = argv[1];
    try {
        CHIP8::Chip8CPU cpu(CHIP8::Chip8Mode::NORMAL, path);
        cpu.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}