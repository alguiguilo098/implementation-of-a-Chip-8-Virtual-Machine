#include "c8vm.hpp"
#include <stdio.h>
#include <stdlib.h>
// Componentes da VM
void VM::cls(uint16_t inst){
    // CLS
    if (inst == 0x00E0){
        for (int i = 0; i < 64 * 32; i++) this->DISPLAY[i] = 0;
    }
}

void VM::drawSprite(uint8_t VX, uint8_t VY, uint8_t N)
{
    
}

// Inicializa a VM
VM::VM(uint16_t pc_inicial){
    
    this->PC = pc_inicial;
};

//  Carrega o arquivo ROM na memória da VM
void VM::CarregarROM(char *arq_rom, uint16_t pc_inicial)
{
    FILE *rom = fopen(arq_rom, "rb");
    fseek(rom, 0, SEEK_END);
    int tam_rom = ftell(rom);

    rewind(rom);
    fread(&this->RAM[pc_inicial], 1, tam_rom, rom);
    fclose(rom);

};

// Executa uma instrução da VM
void VM::ExecutarInstrucao()
{
    uint16_t inst = (this->RAM[this->PC] << 8) | this->RAM[this->PC + 1];

    printf("Instrução: 0x%04X\n", inst);
    this->PC += 2;

    uint8_t grupo = inst >> 12;
    uint8_t X = (inst & 0x0F00) >> 8;
    uint8_t Y = (inst & 0x00F0) >> 4;
    uint8_t N = inst & 0x000F;
    uint8_t NN = inst & 0x00FF;
    uint8_t NNN = inst & 0x0FFF;

    switch (grupo){
    case 0x0:
        this->cls(inst);
        break;

    case 0x1:
        this->V[X] = NN;
        break;
    case 0x2:
        break;
    case 0x3:
        break;
    case 0x4:
        break;
    case 0x5:
        break;
    case 0x6:
        break;
    case 0x7:
        break;
    case 0x8:
        break;
    case 0x9:
        break;
    case 0xA:
        break;
    case 0xB:
        break;
    case 0xC:
        break;
    case 0xD:
        break;
    case 0xE:
        break;
    case 0xF:
        break;

    default:
        printf("Grupo não implementado! Instrução: 0x%04X\n",inst);
    }
};

void VM::ImprimirRegistradores(){

    printf("PC: 0x%04X I: 0x%04X SP: 0x%02X\n", this->PC, this->I, this->SP);
    for (int i = 0; i < 16; i++) printf("V[%X]: 0x%02X ", i, this->V[i]);
    printf("\n");
};