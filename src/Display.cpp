#include "Display.hpp"
#include <iostream>

Display::Display() : pixels(WIDTH * HEIGHT, 0) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(std::string("SDL_Init falhou: ") + SDL_GetError());

    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH * SCALE,
        HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!window)
        throw std::runtime_error(std::string("Falha ao criar janela: ") + SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        throw std::runtime_error(std::string("Falha ao criar renderer: ") + SDL_GetError());
}

Display::~Display() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::Clear() {
    std::fill(pixels.begin(), pixels.end(), 0);
}

void Display::DrawPixel(int x, int y, bool value) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    pixels[y * WIDTH + x] = value ? 1 : 0;
}

void Display::UpdateFromMemory(const uint8_t* memoryDisplay) {
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        pixels[i] = memoryDisplay[i];
}

void Display::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (pixels[y * WIDTH + x]) {
                SDL_Rect pixelRect = { x * SCALE, y * SCALE, SCALE, SCALE };
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
