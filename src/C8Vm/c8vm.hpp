#pragma once
#include "IVm.hpp"
#include <cstdint>

// caminhos relativos a este header (src/C8Vm -> ../Display, ../Keyboard)
#include "../Display/Display.hpp"
#include "../Keyboard/Keyboard.hpp"

class VM {
public:
    // buffer público para acesso direto do main (pode mudar para getter se preferir)
    uint8_t DISPLAY[64 * 32];

    // ponteiros para componentes
    Display* display;
    Keyboard* keyboard;

    VM(uint16_t pc_inicial = 0x200, Display* disp = nullptr, Keyboard* keyb = nullptr);

    // Inicialização / ROM / execução
    void CarregarROM(const char* arq_rom, uint16_t pc_inicial, Display* disp, Keyboard* keyb);
    void ExecutarInstrucao();
    void ImprimirRegistradores();
    void run();
    void TickTimers();

    // Instruções / helpers
    void cls(uint16_t inst);
    void drawSprite(uint8_t X, uint8_t Y, uint8_t N);
    void skipIfKeyPressed(uint8_t X);
    void skipIfKeyNotPressed(uint8_t X);

    void loadDelayTimer(uint8_t X);
    void setDelayTimer(uint8_t X);
    void setSoundTimer(uint8_t X);
    void addToIndex(uint8_t X);
    void setIndexToFont(uint8_t X);
    void storeBCD(uint8_t X);
    void storeRegisters(uint8_t X);
    void loadRegisters(uint8_t X);

    void jump(uint16_t NNN);
    void callSubroutine(uint16_t NNN);
    void returnSubroutine();

    void skipIfEqual(uint8_t X, uint8_t NN);
    void skipIfNotEqual(uint8_t X, uint8_t NN);
    void skipIfRegistersEqual(uint8_t X, uint8_t Y);
    void skipIfRegistersNotEqual(uint8_t X, uint8_t Y);

    void jumpRelative(uint16_t NNN);
    void setIndex(uint16_t NNN);

    void setRegister(uint8_t X, uint8_t NN);
    void addImmediate(uint8_t X, uint8_t NN);
    void setRegisterRegister(uint8_t X, uint8_t Y);
    void orRegisters(uint8_t X, uint8_t Y);
    void andRegisters(uint8_t X, uint8_t Y);
    void xorRegisters(uint8_t X, uint8_t Y);
    void addRegisters(uint8_t X, uint8_t Y);
    void subRegisters(uint8_t X, uint8_t Y);
    void subRegistersReverse(uint8_t X, uint8_t Y);
    void shiftRight(uint8_t X);
    void shiftLeft(uint8_t X);
    void execute8Group(uint8_t X, uint8_t Y, uint8_t N);

    void GrupoE(uint8_t X, uint8_t NN); // EX9E / EXA1
    void GrupoF(uint8_t X, uint8_t NN); // FX07 / FX15 / FX1E / etc.

    void random(uint8_t X, uint8_t NN);

private:
    uint8_t RAM[4096];
    uint16_t PC;
    uint8_t V[16];
    uint8_t SP;
    uint16_t I;
    uint16_t stack[16];
    uint8_t delayTimer;
    uint8_t soundTimer;
};