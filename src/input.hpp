#pragma once
#include <cstdint>

namespace CHIP8 {
class Chip8Keypad {
public:
    void handleInput();
    void setKey(uint8_t key, bool pressed);
    bool isKeyPressed(uint8_t key) const;
    int waitForKey();

private:
    bool keys[16]{};
};
}  // namespace CHIP8