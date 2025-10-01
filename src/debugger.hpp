#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <unordered_set>

#include "chip8.hpp"
#include "register.hpp"

namespace CHIP8 {

class Debugger {
public:
    Debugger(Chip8CPU& cpu);
    ~Debugger() = default;
    
    void addBreakpoint(uint16_t addr);
    void removeBreakpoint(uint16_t addr);
    void clearBreakpoints();
    bool hasBreakpoint(uint16_t addr) const;
    
    void step();
    void continueExecution();
    bool isAtBreakpoint() const;
    
    std::unique_ptr<Register> inspectRegister();
    void memoryDump(uint16_t start_addr, uint16_t end_addr);

private:
    Chip8CPU& cpu;
    std::unordered_set<uint16_t> breakpoints;
};

}  // namespace CHIP8