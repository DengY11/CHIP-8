#include "stack.hpp"

#include <cstring>

namespace CHIP8 {

Stack::Stack() : sp(0) {
    memset(stack, 0, sizeof stack);
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