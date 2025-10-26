cpp:
#include "Keyboard.hpp"

Keyboard::Keyboard() {
    keyStates.fill(false);
}

void Keyboard::Update() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            keyStates[e.key.keysym.scancode] = true;
        } else if (e.type == SDL_KEYUP) {
            keyStates[e.key.keysym.scancode] = false;
        }
    }
}

bool Keyboard::IsKeyPressed(SDL_Keycode key) const {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return keyStates[scancode];
}
