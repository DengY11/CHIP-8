#include "debugger.hpp"
#include "test_access.hpp"
#include <iostream>
#include <iomanip>
#include <thread>

namespace CHIP8 {

Debugger::Debugger(Chip8CPU& cpu) : cpu(cpu) {
}

void Debugger::addBreakpoint(uint16_t addr) {
    breakpoints.insert(addr);
}

void Debugger::removeBreakpoint(uint16_t addr) {
    breakpoints.erase(addr);
}

void Debugger::clearBreakpoints() {
    breakpoints.clear();
}

bool Debugger::hasBreakpoint(uint16_t addr) const {
    return breakpoints.count(addr);
}

bool Debugger::isWindowClosed() {
    if (CHIP8::Chip8TestAccess::handle_input(cpu) == false) {
        return true;  
    }
    return false;
}

void Debugger::step() {
    uint16_t current_pc = CHIP8::Chip8TestAccess::getPC(cpu);
    if (breakpoints.count(current_pc)) {
        std::cout << "Breakpoint hit at 0x" << std::hex << current_pc << std::endl;
        return; 
    }
    
    if (!CHIP8::Chip8TestAccess::handle_input(cpu)) {
        std::cout << "SDL window closed during step" << std::endl;
        return;
    }
    
    CHIP8::Chip8TestAccess::cycle(cpu);
    CHIP8::Chip8TestAccess::update_timers(cpu);
    CHIP8::Chip8TestAccess::render(cpu);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void Debugger::continueExecution() {
    while (true) {
        if (!CHIP8::Chip8TestAccess::handle_input(cpu)) {
            std::cout << "SDL window closed during continue" << std::endl;
            break;
        }
        
        uint16_t current_pc = CHIP8::Chip8TestAccess::getPC(cpu);
        if (breakpoints.count(current_pc)) {
            std::cout << "Breakpoint hit at 0x" << std::hex << current_pc << std::endl;
            break; 
        }
        CHIP8::Chip8TestAccess::cycle(cpu);
        CHIP8::Chip8TestAccess::update_timers(cpu);
        CHIP8::Chip8TestAccess::render(cpu);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool Debugger::isAtBreakpoint() const {
    return breakpoints.count(CHIP8::Chip8TestAccess::getPC(cpu));
}

std::unique_ptr<Register> Debugger::inspectRegister() {
    auto r = std::make_unique<Register>();
    for(int i=0;i<16;++i){
        r->V[i]=CHIP8::Chip8TestAccess::getRegisterV(cpu, i);
    }
    r->I=CHIP8::Chip8TestAccess::getRegisterI(cpu);
    r->PC=CHIP8::Chip8TestAccess::getPC(cpu);
    r->SP=CHIP8::Chip8TestAccess::getSP(cpu);
    r->delay_timer=CHIP8::Chip8TestAccess::getDT(cpu);
    r->sound_timer=CHIP8::Chip8TestAccess::getST(cpu);
    return r;
}

void Debugger::memoryDump(uint16_t start_addr, uint16_t end_addr) {
    std::cout << "Memory dump from 0x" << std::hex << start_addr 
              << " to 0x" << end_addr << ":" << std::endl;
    if (end_addr < start_addr) {
        std::cout << "Error: Invalid address range" << std::endl;
        return;
    }
    const int bytes_per_line = 16;
    for (uint16_t addr = start_addr; addr <= end_addr; addr += bytes_per_line) {
        std::cout << "0x" << std::hex << std::setw(4) << std::setfill('0') << addr << ": ";
        for (int i = 0; i < bytes_per_line; i++) {
            uint16_t current_addr = addr + i;
            if (current_addr <= end_addr) {
                uint8_t value = CHIP8::Chip8TestAccess::getMemory(cpu, current_addr);
                std::cout << std::hex << std::setw(2) << std::setfill('0') 
                          << static_cast<int>(value) << " ";
            } else {
                std::cout << "   "; 
            }
        }
        std::cout << " ";
        for (int i = 0; i < bytes_per_line; i++) {
            uint16_t current_addr = addr + i;
            if (current_addr <= end_addr) {
                uint8_t value = CHIP8::Chip8TestAccess::getMemory(cpu, current_addr);
                if (value >= 32 && value <= 126) { 
                    std::cout << static_cast<char>(value);
                } else {
                    std::cout << "."; 
                }
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}
}  // namespace CHIP8
