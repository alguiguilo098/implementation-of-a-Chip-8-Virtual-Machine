#include "./src/Display/Display.hpp"
#include "./src/Keyboard/Keyboard.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "./src/C8Vm/c8vm.hpp"

// ---------- Main ----------
int main(int argc, char** argv) {
    try {
        Display display;
        Keyboard keyboard;

        // inicializa VM com display e teclado
        VM chip8_vm(0x200, &display, &keyboard);

        // carrega ROM se fornecida como argumento
        if (argc > 1) {
            chip8_vm.CarregarROM(argv[1], 0x200, &display, &keyboard);
            std::cout << "ROM carregada: " << argv[1] << std::endl;
        } else {
            std::cout << "Nenhuma ROM fornecida. Rode: ./chip8 Imagens-Teste/<arquivo.ch8>" << std::endl;
        }

        const int cyclesPerFrame = 8; 
        bool running = true;

        while (running) {
            // atualiza eventos do teclado
            keyboard.Update();
            if (keyboard.ShouldQuit() || keyboard.IsKeyPressed(SDLK_ESCAPE))
                break;

            // emula N instruções por frame
            for (int i = 0; i < cyclesPerFrame; ++i)
                chip8_vm.ExecutarInstrucao();

            // decrementa timers (~60Hz)
            chip8_vm.TickTimers();
            
            // renderiza buffer da VM (apenas o que a ROM desenhar)
            display.Clear();
            for (int py = 0; py < display.HEIGHT; ++py) {
                for (int px = 0; px < display.WIDTH; ++px) {
                    int idx = px + py * display.WIDTH;
                    if (chip8_vm.DISPLAY[idx]) display.DrawPixel(px, py, true);
                }
            }
            display.Render();

            SDL_Delay(16); // ~60 FPS
        }

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
