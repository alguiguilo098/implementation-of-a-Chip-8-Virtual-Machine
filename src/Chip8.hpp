#pragma once

#include "Display.hpp"
#include "c8vm.hpp"

class Chip8{
    Display *display;
    VM *c8vm;
public:
    Chip8(Display *display,VM *c8vm);
    void load_program(const char *program_path);
    void run();
    
}