#include "register.hpp"

#include <cstring>

namespace CHIP8 {

Register::Register() {
    reset();
}

void Register::reset() {
    memset(V, 0, sizeof V);
    I = 0;
    PC = 0x200;
    SP = 0;
    delay_timer = 0;
    sound_timer = 0;
}

}  // namespace CHIP8
