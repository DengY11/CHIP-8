#pragma once
#include <SDL.h>
#include <SDL_keycode.h>

#include <cstdint>

namespace CHIP8 {
class Chip8Display {
public:
    static const int WIDTH = 64;
    static const int HEIGHT = 32;

    Chip8Display();
    ~Chip8Display();

    void clear();

    /**
     * @brief Draws a sprite at the given coordinates.
     * 
     * @param x The x-coordinate to draw the sprite at.
     * @param y The y-coordinate to draw the sprite at.
     * @param sprite A pointer to the sprite data.
     * @param numRows The number of rows in the sprite.
     * @return True if a pixel collision occurred (a pixel was flipped from 1 to 0), false otherwise.
     */
    bool drawSprite(int x, int y, const uint8_t* sprite,
                    int numRows); 

    /**
     * @brief Renders the current state of the display to the screen using SDL.
     */
    void render();                

    /**
     * @brief Gets the state of a pixel at the given coordinates.
     * 
     * @param x The x-coordinate of the pixel.
     * @param y The y-coordinate of the pixel.
     * @return True if the pixel is on, false otherwise.
     */
    bool getPixel(int x, int y) const;

private:
    uint8_t pixels[WIDTH * HEIGHT]{};
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};
}  // namespace CHIP8
