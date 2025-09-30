#include "stack.hpp"
#include <cstring>

namespace CHIP8 {

Stack::Stack(uint8_t& sp_ref, uint16_t* stack_array) : sp(sp_ref), stack(stack_array) {
    reset();
}

void Stack::reset() {
    sp = 0;
    memset(stack, 0, STACK_SIZE * sizeof(uint16_t));
}

bool Stack::push(uint16_t value) {
    if (sp >= STACK_SIZE) {
        return false;
    }
    stack[sp++] = value;
    return true;
}

std::optional<uint16_t> Stack::pop() {
    if (sp <= 0) {
        return std::nullopt;
    }
    return stack[--sp];
}

}  // namespace CHIP8