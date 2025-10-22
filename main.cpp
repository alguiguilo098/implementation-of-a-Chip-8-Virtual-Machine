#include "./src/Display.hpp"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    try {
        Display display;

        display.Clear(); // Limpa tudo antes de desenhar

        // Desenha a moldura (borda)
        for (int x = 0; x < display.HEIGHT; ++x) {
            display.DrawPixel(x, 0, true);                       // Topo
            display.DrawPixel(x, display.HEIGHT - 1, true);     // Base
        }

        for (int y = 0; y < display.HEIGHT; ++y) {
            display.DrawPixel(0, y, true);                       // Esquerda
            display.DrawPixel(display.HEIGHT - 1, y, true);      // Direita
        }

        // Atualiza o renderizador
        display.Render();

        // Mantém a janela aberta até o usuário fechar
        bool running = true;
        SDL_Event e;

        while (running) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    running = false;
                } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }

            SDL_Delay(16); // evita uso excessivo de CPU (~60 FPS)
        }

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
