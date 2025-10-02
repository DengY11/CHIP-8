#include "debugger_cli.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <csignal>
#include <atomic>

namespace CHIP8 {

std::atomic<bool> g_should_exit(false);

void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nReceived Ctrl+C, exiting..." << std::endl;
        g_should_exit = true;
    }
}

DebuggerCLI::DebuggerCLI(Debugger& debugger) : debugger(debugger) {
    std::signal(SIGINT, signal_handler);
}

void DebuggerCLI::run() {
    std::cout << "CHIP-8 Debugger CLI" << std::endl;
    std::cout << "Type 'help' for available commands" << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    
    std::string input;
    while (!g_should_exit) {
        printPrompt();
        if (debugger.isWindowClosed()) {
            std::cout << "SDL window closed, exiting..." << std::endl;
            break;
        }
        if (!std::getline(std::cin, input)) {
            if (std::cin.eof()) {
                std::cout << "\nReceived Ctrl+D, exiting..." << std::endl;
                break;
            }
            continue;
        }
        
        if (input.empty()) {
            continue;
        }
        
        if (!parseCommand(input)) {
            break; 
        }
    }
}

void DebuggerCLI::printPrompt() {
    std::cout << "(chip8dbg) ";
    std::cout.flush();  
}

bool DebuggerCLI::parseCommand(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> args;
    std::string token;
    
    while (iss >> token) {
        args.push_back(token);
    }
    if (args.empty()) {
        return true;
    }
    std::string command = args[0];
    if (command == "s" || command == "step") {
        handleStep(args);
    } else if (command == "c" || command == "continue") {
        handleContinue(args);
    } else if (command == "b" || command == "break") {
        handleBreakpoint(args);
    } else if (command == "d" || command == "delete") {
        handleDeleteBreakpoint(args);
    } else if (command == "clear") {
        handleClearBreakpoints(args);
    } else if (command == "r" || command == "registers") {
        handleRegisters(args);
    } else if (command == "m" || command == "memory") {
        handleMemory(args);
    } else if (command == "q" || command == "quit") {
        handleQuit(args);
        return false; // 退出
    } else if (command == "h" || command == "help") {
        handleHelp(args);
    } else {
        std::cout << "Unknown command: " << command << std::endl;
        std::cout << "Type 'help' for available commands" << std::endl;
    }
    
    return true;
}

void DebuggerCLI::handleStep(const std::vector<std::string>& args) {
    int steps = 1;
    if (args.size() > 1) {
        try {
            steps = std::stoi(args[1]);
        } catch (...) {
            std::cout << "Invalid step count: " << args[1] << std::endl;
            return;
        }
    }
    
    for (int i = 0; i < steps; i++) {
        debugger.step();
        auto regs = debugger.inspectRegister();
        std::cout << "Step " << (i + 1) << ": PC=0x" << std::hex << regs->PC << std::endl;
        
        if (debugger.isAtBreakpoint()) {
            break;
        }
    }
}

void DebuggerCLI::handleContinue(const std::vector<std::string>& args) {
    std::cout << "Continuing execution..." << std::endl;
    debugger.continueExecution();
    auto regs = debugger.inspectRegister();
    std::cout << "Stopped at PC=0x" << std::hex << regs->PC << std::endl;
}

void DebuggerCLI::handleBreakpoint(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: break <address>" << std::endl;
        return;
    }
    try {
        uint16_t addr = std::stoul(args[1], nullptr, 16);
        debugger.addBreakpoint(addr);
        std::cout << "Breakpoint set at 0x" << std::hex << addr << std::endl;
    } catch (...) {
        std::cout << "Invalid address: " << args[1] << std::endl;
    }
}

void DebuggerCLI::handleDeleteBreakpoint(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: delete <address>" << std::endl;
        return;
    }
    
    try {
        uint16_t addr = std::stoul(args[1], nullptr, 16);
        debugger.removeBreakpoint(addr);
        std::cout << "Breakpoint removed at 0x" << std::hex << addr << std::endl;
    } catch (...) {
        std::cout << "Invalid address: " << args[1] << std::endl;
    }
}

void DebuggerCLI::handleClearBreakpoints(const std::vector<std::string>& args) {
    debugger.clearBreakpoints();
    std::cout << "All breakpoints cleared" << std::endl;
}

void DebuggerCLI::handleRegisters(const std::vector<std::string>& args) {
    auto regs = debugger.inspectRegister();
    
    std::cout << "Registers:" << std::endl;
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << regs->PC << std::endl;
    std::cout << "I:  0x" << std::hex << std::setw(4) << std::setfill('0') << regs->I << std::endl;
    std::cout << "SP: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(regs->SP) << std::endl;
    std::cout << "DT: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(regs->delay_timer) << std::endl;
    std::cout << "ST: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(regs->sound_timer) << std::endl;
    
    std::cout << "V registers:" << std::endl;
    for (int i = 0; i < 16; i++) {
        std::cout << "V" << std::hex << i << ": 0x" << std::setw(2) << std::setfill('0') 
                  << static_cast<int>(regs->V[i]) << " ";
        if ((i + 1) % 4 == 0) {
            std::cout << std::endl;
        }
    }
}

void DebuggerCLI::handleMemory(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cout << "Usage: memory <start_addr> <end_addr>" << std::endl;
        return;
    }
    
    try {
        uint16_t start_addr = std::stoul(args[1], nullptr, 16);
        uint16_t end_addr = std::stoul(args[2], nullptr, 16);
        
        debugger.memoryDump(start_addr, end_addr);
    } catch (...) {
        std::cout << "Invalid address range" << std::endl;
    }
}

void DebuggerCLI::handleQuit(const std::vector<std::string>& args) {
    std::cout << "Exiting debugger" << std::endl;
}

void DebuggerCLI::handleHelp(const std::vector<std::string>& args) {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  s, step [n]        - Execute n instructions (default: 1)" << std::endl;
    std::cout << "  c, continue        - Continue execution until breakpoint" << std::endl;
    std::cout << "  b, break <addr>     - Set breakpoint at address" << std::endl;
    std::cout << "  d, delete <addr>    - Remove breakpoint at address" << std::endl;
    std::cout << "  clear               - Clear all breakpoints" << std::endl;
    std::cout << "  r, registers        - Show all registers" << std::endl;
    std::cout << "  m, memory <s> <e>  - Dump memory from start to end address" << std::endl;
    std::cout << "  q, quit             - Exit debugger" << std::endl;
    std::cout << "  h, help             - Show this help" << std::endl;
}

}  // namespace CHIP8