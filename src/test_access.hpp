#pragma once
#include <cstdint>
#include <string>

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

    static void setRegisterV(Chip8CPU& cpu, int index, uint8_t value);
    static void setRegisterI(Chip8CPU& cpu, uint16_t value);
    static void setPC(Chip8CPU& cpu, uint16_t value);
    static void setSP(Chip8CPU& cpu, uint8_t value);
    static void setDT(Chip8CPU& cpu, uint8_t value);
    static void setST(Chip8CPU& cpu, uint8_t value);
    static void setMemory(Chip8CPU& cpu, uint16_t address, uint8_t value);

    static void cycle(Chip8CPU& cpu);
    static bool loadROM(Chip8CPU& cpu, const std::string& filename);
    static void update_timers(Chip8CPU& cpu);
    static bool handle_input(Chip8CPU& cpu);
    static void render(Chip8CPU& cpu);
};
}  // namespace CHIP8