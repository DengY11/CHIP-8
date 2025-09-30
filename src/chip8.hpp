#pragma once
#include <string>
#include <memory>

#include "display.hpp"
#include "input.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "stack.hpp"
#include "test_access.hpp"

namespace CHIP8 {
enum class Chip8Mode {
    NORMAL ,
    TEST 
};
class Chip8CPU {
public:
    Chip8CPU();
    Chip8CPU(Chip8Mode mode);
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
    uint16_t stack_array[16]; // Stack storage array
    
    friend class Chip8TestAccess;
};
}  // namespace CHIP8