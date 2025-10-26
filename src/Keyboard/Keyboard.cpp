#include "Keyboard.hpp"

Keyboard::Keyboard() : quit(false) {
    keyStates.fill(false);
    chipKeys.fill(0);
}

int Keyboard::SDLKeyToChip8Index(SDL_Keycode k)
{
    // Mapeamento padrão usado no projeto:
    // CHIP-8 keypad:
    // 1 2 3 C
    // 4 5 6 D
    // 7 8 9 E
    // A 0 B F
    //
    // Mapemos para teclas físicas:
    // 1 2 3 4
    // Q W E R
    // A S D F
    // Z X C V
    switch (k) {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;
        case SDLK_q: return 0x4;
        case SDLK_w: return 0x5;
        case SDLK_e: return 0x6;
        case SDLK_r: return 0xD;
        case SDLK_a: return 0x7;
        case SDLK_s: return 0x8;
        case SDLK_d: return 0x9;
        case SDLK_f: return 0xE;
        case SDLK_z: return 0xA;
        case SDLK_x: return 0x0;
        case SDLK_c: return 0xB;
        case SDLK_v: return 0xF;
        default: return -1;
    }
}

void Keyboard::Update() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
            bool pressed = (e.type == SDL_KEYDOWN);
            // atualiza estado por scancode (usado por IsKeyPressed(SDL_Keycode))
            SDL_Scancode sc = e.key.keysym.scancode;
            if (sc >= 0 && sc < SDL_NUM_SCANCODES) keyStates[sc] = pressed;

            // atualiza estado do keypad Chip-8, se a tecla for mapeada
            SDL_Keycode kc = e.key.keysym.sym;
            int idx = SDLKeyToChip8Index(kc);
            if (idx >= 0 && idx < 16) chipKeys[idx] = pressed ? 1 : 0;

            // ESC também pode sinalizar quit (opcional)
            if (kc == SDLK_ESCAPE && pressed) quit = true;
        }
    }
}

bool Keyboard::ShouldQuit() const {
    return quit;
}

bool Keyboard::IsKeyPressed(SDL_Keycode key) const {
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    if (scancode >= 0 && scancode < SDL_NUM_SCANCODES)
        return keyStates[scancode];
    return false;
}

std::array<uint8_t,16> Keyboard::GetChip8Keys() const {
    return chipKeys;
}