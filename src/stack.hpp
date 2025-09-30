#pragma once
#include <cstdint>
#include <optional>

namespace CHIP8 {
class Stack {
public:
    Stack(uint8_t& sp_ref, uint16_t* stack_array);
    ~Stack() = default;
    void reset();
    bool push(uint16_t value);
    std::optional<uint16_t> pop();

private:
    uint8_t& sp; // Reference to SP register in Register struct
    uint16_t* stack; // Pointer to stack array
    static const size_t STACK_SIZE = 16;
};
}  // namespace CHIP8