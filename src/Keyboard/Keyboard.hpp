#pragma once
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>

class Keyboard {
public:
    Keyboard();

    // Processa eventos SDL (deve ser chamado no loop principal)
    void Update();

    // Para o main: fechar janela / ESC
    bool ShouldQuit() const;

    // Consulta por SDL_Keycode (usado pela VM que converte CHIP-8 -> SDL_Keycode)
    bool IsKeyPressed(SDL_Keycode key) const;

    // Retorna estado das 16 teclas do CHIP-8 (0..15)
    std::array<uint8_t,16> GetChip8Keys() const;

private:
    bool quit;
    // estados por scancode para consultas via SDL_GetScancodeFromKey
    std::array<bool, SDL_NUM_SCANCODES> keyStates;
    // estado do keypad Chip-8 (0 ou 1)
    std::array<uint8_t,16> chipKeys;

    // helper: converte SDL_Keycode -> índice Chip-8 (0..15) ou -1 se não mapeado
    static int SDLKeyToChip8Index(SDL_Keycode k);
};