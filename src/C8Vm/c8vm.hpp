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
    // Desenha um sprite de N bytes na tela em VX, VY
    void drawSprite(uint8_t VX, uint8_t VY, uint8_t N);
    // Salta para endereço NNN
    void jump(uint16_t NNN);
    // Chama subrotina em NNN (empilha PC)
    void callSubroutine(uint16_t NNN);
    // Retorna de subrotina (desempilha PC)
    void returnSubroutine();
    // Pula próxima instrução se VX == NN
    void skipIfEqual(uint8_t X, uint8_t NN);
    // Pula próxima instrução se VX != NN
    void skipIfNotEqual(uint8_t X, uint8_t NN);
    // Pula próxima instrução se VX == VY
    void skipIfRegistersEqual(uint8_t X, uint8_t Y);
    // Pula próxima instrução se VX != VY
    void skipIfRegistersNotEqual(uint8_t X, uint8_t Y);
    // Salto relativo: PC = NNN + V0
    void jumpRelative(uint16_t NNN);
    // VX = NN
    void setRegister(uint8_t X, uint8_t NN);
    // VX += NN
    void addImmediate(uint8_t X, uint8_t NN);
    // VX = VY
    void setRegisterRegister(uint8_t X, uint8_t Y);
    // VX |= VY
    void orRegisters(uint8_t X, uint8_t Y);
    // VX &= VY
    void andRegisters(uint8_t X, uint8_t Y);
    // VX ^= VY
    void xorRegisters(uint8_t X, uint8_t Y);
    // VX += VY, VF = carry
    void addRegisters(uint8_t X, uint8_t Y);
    // VX -= VY, VF = NOT borrow
    void subRegisters(uint8_t X, uint8_t Y);
    // VX = VY - VX, VF = NOT borrow
    void subRegistersReverse(uint8_t X, uint8_t Y);
    // VX >>= 1, VF = LSB
    void shiftRight(uint8_t X);
    // VX <<= 1, VF = MSB
    void shiftLeft(uint8_t X);

    void execute8Group(uint8_t X, uint8_t Y, uint8_t N);
    void executarGrupoE(uint8_t X, uint8_t NN);
    void executarGrupoF(uint8_t X, uint8_t NN);
    
    VM(uint16_t pc_inicial);
    void CarregarROM(
        char *arq_rom,
        uint16_t pc_inicial);
    void ExecutarInstrucao();
    void ImprimirRegistradores();
};
