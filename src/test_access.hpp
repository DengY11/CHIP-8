#pragma once
#include <cstdint>

namespace CHIP8 {
class Chip8CPU;
class Memory;

class Chip8TestAccess {
public:
    static uint8_t getRegisterV(const Chip8CPU& cpu, int index);
    static uint16_t getRegisterI(const Chip8CPU& cpu);
    static uint16_t getPC(const Chip8CPU& cpu);
    static uint8_t getSP(const Chip8CPU& cpu);
    static uint8_t getDT(const Chip8CPU& cpu);
    static uint8_t getST(const Chip8CPU& cpu);
    static uint8_t getMemory(const Chip8CPU& cpu, uint16_t address);
    static void setMemory(Chip8CPU& cpu, uint16_t address, uint8_t value);
    static uint8_t getMemory(const Memory& mem, uint16_t address);
    static void setMemory(Memory& mem, uint16_t address, uint8_t value);
};
}  // namespace CHIP8