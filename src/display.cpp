#include "display.hpp"

#include <SDL2/SDL.h>

#include <cstring>

namespace CHIP8 {

Chip8Display::Chip8Display() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, WIDTH * 10, HEIGHT * 10,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

Chip8Display::~Chip8Display() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Chip8Display::clear() {
    memset(pixels, 0, sizeof(pixels));
}

bool Chip8Display::drawSprite(int x, int y, const uint8_t* sprite,
                              int numRows) {
    bool pixelCollision = false;
    for (int row = 0; row < numRows; ++row) {
        uint8_t spriteRow = sprite[row];
        for (int col = 0; col < 8; ++col) {
            if ((spriteRow & (0x80 >> col)) != 0) {
                int currentX = (x + col) % WIDTH;
                int currentY = (y + row) % HEIGHT;
                int pixelIndex = currentY * WIDTH + currentX;
                if (pixels[pixelIndex] == 1) {
                    pixelCollision = true;
                }
                pixels[pixelIndex] ^= 1;
            }
        }
    }
    return pixelCollision;
}

void Chip8Display::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (pixels[y * WIDTH + x] == 1) {
                SDL_Rect rect = {x * 10, y * 10, 10, 10};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

bool Chip8Display::getPixel(int x, int y) const {
    return pixels[y * WIDTH + x] == 1;
}

}  // namespace CHIP8