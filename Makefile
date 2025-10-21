all:
   g++ main.cpp Display.cpp c8vm.cpp -o chip8 $(pkg-config --cflags --libs sdl2)

clean:
   rm -f chip8