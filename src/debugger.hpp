#pragma once
#include <iostream>
#include <string>
#include "chip8.hpp"
#include "test_access.hpp"


namespace CHIP8{

class Debugger{
public:
    Debugger(Chip8CPU& cpu);
    ~Debugger() = default;
    void breakpoint(uint16_t addr);
    void step();
    void inspectRegister();
    void memoryDump(uint16_t start_addr, uint16_t end_addr);
private:
    Chip8CPU& cpu;

};


}