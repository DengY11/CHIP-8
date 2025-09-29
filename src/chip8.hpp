#pragma once
#include <string>

#include "display.hpp"
#include "input.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "stack.hpp"

namespace CHIP8 {
class Chip8CPU {
public:
    Chip8CPU();
    ~Chip8CPU() = default;
    void cycle();
    bool loadROM(const std::string& filename);
    void update_timers();
    bool handle_input();
    void render();

private:
    std::unique_ptr<Memory> mem;
    std::unique_ptr<Register> reg;
    std::unique_ptr<Chip8Display> display;
    std::unique_ptr<Chip8Keypad> keypad;
    std::unique_ptr<Stack> stack;
};
}  // namespace CHIP8