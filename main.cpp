#include <iostream>
#include "c8vm.hpp"
#include "Display.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo.ch8>\n";
        return 1;
    }
    VM vm(0x200);
    vm.CarregarROM(argv[1], 0x200);

    try {
        Display display;
        bool running = true;
        SDL_Event event;

        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) running = false;
            }
            vm.ExecutarInstrucao();

            // Atualiza o display com base na memória da VM
            display.UpdateFromMemory(vm.DISPLAY);
            display.Render();

            SDL_Delay(100);
        }

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
