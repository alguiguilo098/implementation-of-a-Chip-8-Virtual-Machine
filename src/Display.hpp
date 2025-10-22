#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <vector>
#include <stdexcept>
#include "../Interface/IDisplay.hpp"

class Display:IDisplay {
public:
    int WIDTH = 64;
    int HEIGHT = 32;
    int SCALE = 10;

    Display();
    ~Display();

    void Clear();
    void DrawPixel(int x, int y, bool value);
    void Render();
    void UpdateFromMemory(const uint8_t* memoryDisplay);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<uint8_t> pixels;
};

#endif
