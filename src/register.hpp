#pragma once
#include <cstdint>
namespace CHIP8 {

struct Register {
    Register();
    ~Register() = default;
    void reset();
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint8_t SP;
    uint8_t delay_timer;
    uint8_t sound_timer;
};

}  // namespace CHIP8