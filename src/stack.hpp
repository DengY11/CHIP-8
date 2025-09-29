#pragma once
#include <cstdint>
#include <optional>

namespace CHIP8 {

static constexpr uint8_t STACK_SIZE = 16;

class Stack {
public:
    Stack();
    void reset();
    bool push(uint16_t value);
    std::optional<uint16_t> pop();

private:
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
};

}  // namespace CHIP8