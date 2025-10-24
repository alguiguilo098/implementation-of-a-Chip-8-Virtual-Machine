#include "./src/Display.hpp"
#include "./src/Keyboard.hpp"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    try {
        Display display;
        Keyboard keyboard;

        int x = display.WIDTH / 2;
        int y = display.HEIGHT / 2;

        bool running = true;

        while (running) {
            keyboard.Update();

            if (keyboard.ShouldQuit() || keyboard.IsKeyPressed(SDLK_ESCAPE))
                running = false;

            //Mapeamento simplificado do teclado para o Chip-8
            //    1 2 3 C
            //    4 5 6 D
            //    7 8 9 E
            //    A 0 B F

            if (keyboard.IsKeyPressed(SDLK_1)) std::cout << "Tecla 1 pressionada" << std::endl; //1
            if (keyboard.IsKeyPressed(SDLK_2)) std::cout << "Tecla 2 pressionada" << std::endl; //2
            if (keyboard.IsKeyPressed(SDLK_3)) std::cout << "Tecla 3 pressionada" << std::endl; //3
            if (keyboard.IsKeyPressed(SDLK_4)) std::cout << "Tecla 4 pressionada" << std::endl; //C
            if (keyboard.IsKeyPressed(SDLK_q)) std::cout << "Tecla Q pressionada" << std::endl; //4
            if (keyboard.IsKeyPressed(SDLK_w)) y--;                                             //5
            if (keyboard.IsKeyPressed(SDLK_e)) std::cout << "Tecla E pressionada" << std::endl; //6
            if (keyboard.IsKeyPressed(SDLK_r)) std::cout << "Tecla R pressionada" << std::endl; //D
            if (keyboard.IsKeyPressed(SDLK_a)) x--;                                             //7
            if (keyboard.IsKeyPressed(SDLK_s)) y++;                                             //8
            if (keyboard.IsKeyPressed(SDLK_d)) x++;                                             //9
            if (keyboard.IsKeyPressed(SDLK_f)) std::cout << "Tecla F pressionada" << std::endl; //E
            if (keyboard.IsKeyPressed(SDLK_z)) std::cout << "Tecla Z pressionada" << std::endl; //A
            if (keyboard.IsKeyPressed(SDLK_x)) std::cout << "Tecla X pressionada" << std::endl; //0
            if (keyboard.IsKeyPressed(SDLK_c)) std::cout << "Tecla C pressionada" << std::endl; //B
            if (keyboard.IsKeyPressed(SDLK_v)) std::cout << "Tecla V pressionada" << std::endl; //F


            // Limites
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x >= display.WIDTH)  x = display.WIDTH - 1;
            if (y >= display.HEIGHT) y = display.HEIGHT - 1;

            // Atualiza tela
            display.Clear();
            display.DrawPixel(x, y, true);
            display.Render();

            SDL_Delay(16); // ~60 FPS
        }

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
