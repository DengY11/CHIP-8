#include "input.hpp"

#include <SDL.h>

namespace CHIP8 {

// Map modern keyboard keys to CHIP-8 keypad
static const SDL_Keycode keymap[16] = {
    SDLK_1, SDLK_2, SDLK_3, SDLK_4,  // 1 2 3 4
    SDLK_q, SDLK_w, SDLK_e, SDLK_r,  // Q W E R
    SDLK_a, SDLK_s, SDLK_d, SDLK_f,  // A S D F
    SDLK_z, SDLK_x, SDLK_c, SDLK_v,  // Z X C V
};

void Chip8Keypad::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            // Handle quit event if necessary
            // For now, we just exit the loop
            break;
        }

        for (int i = 0; i < 16; ++i) {
            if (event.key.keysym.sym == keymap[i]) {
                if (event.type == SDL_KEYDOWN) {
                    keys[i] = true;
                } else if (event.type == SDL_KEYUP) {
                    keys[i] = false;
                }
            }
        }
    }
}

void Chip8Keypad::setKey(uint8_t key, bool pressed) {
    if (key < 16) {
        keys[key] = pressed;
    }
}

bool Chip8Keypad::isKeyPressed(uint8_t key) const {
    if (key < 16) {
        return keys[key];
    }
    return false;
}

int Chip8Keypad::waitForKey() {
    SDL_Event event;
    while (true) {
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                for (int i = 0; i < 16; ++i) {
                    if (event.key.keysym.sym == keymap[i]) {
                        return i;
                    }
                }
            }
        }
    }
}

}  // namespace CHIP8