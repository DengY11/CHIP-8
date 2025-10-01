#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "chip8.hpp"
#include "debugger.hpp"
#include "debugger_cli.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " <ROM file> [--debug]" << std::endl;
        std::cerr << "  --debug: Start in debug mode" << std::endl;
        return 1;
    }
    std::string path = argv[1];
    bool debug_mode = (argc == 3 && std::string(argv[2]) == "--debug");
    try {
        CHIP8::Chip8CPU cpu(CHIP8::Chip8Mode::NORMAL, path);
        if (debug_mode) {
            std::cout << "Starting CHIP-8 Debugger" << std::endl;
            std::cout << "ROM loaded: " << path << std::endl;
            CHIP8::Debugger debugger(cpu);
            CHIP8::DebuggerCLI cli(debugger);
            cli.run();
        } else {
            cpu.run();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}