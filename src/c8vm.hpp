#pragma once

#include <stdint.h>
#include "Interface/IVm.hpp"
class VM : IVM
{

    // Componentes da VM
    uint8_t RAM[4096];  // Memória 4KB
    uint16_t PC;        // Program Counter
    uint8_t V[16];      // Registradores de propósito geral
    uint8_t SP;         // Stack Pointer
    uint16_t I;         // Registrador de índice
    uint16_t stack[16]; // Pilha

public:
    uint8_t DISPLAY[64 * 32]; // Tela
    void cls(uint16_t inst);
    VM(uint16_t pc_inicial);
    void CarregarROM(
        char *arq_rom,
        uint16_t pc_inicial);
    void ExecutarInstrucao();
    void ImprimirRegistradores();
};
