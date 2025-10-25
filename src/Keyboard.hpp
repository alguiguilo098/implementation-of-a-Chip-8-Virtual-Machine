#pragma once
#include <SDL2/SDL.h>
#include <array>

class Keyboard {
public:
    Keyboard();

    void Update();

    bool IsKeyPressed(SDL_Keycode key) const;

    bool ShouldQuit() const { return quit; }

private:
    std::array<bool, SDL_NUM_SCANCODES> keyStates{};
    bool quit = false;
};
