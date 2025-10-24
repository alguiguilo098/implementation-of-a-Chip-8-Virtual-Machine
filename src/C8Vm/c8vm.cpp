#include "c8vm.hpp"
#include <stdio.h>
#include <stdlib.h>
// Componentes da VM
void VM::cls(uint16_t inst)
{
    // CLS
    if (inst == 0x00E0)
    {
        for (int i = 0; i < 64 * 32; i++)
            this->DISPLAY[i] = 0;
    }
}

void VM::drawSprite(uint8_t VX, uint8_t VY, uint8_t N)
{
    V[0xF] = 0; // VF = 0 inicialmente, será usado para colisão

    for (int yLine = 0; yLine < N; yLine++)
    {
        uint8_t pixel = RAM[I + yLine]; // lê a linha do sprite da memória
        for (int xLine = 0; xLine < 8; xLine++)
        {
            // verifica se o bit está ativo
            if ((pixel & (0x80 >> xLine)) != 0)
            {
                int xPos = (VX + xLine) % 64; // tela de 64 pixels de largura
                int yPos = (VY + yLine) % 32; // tela de 32 pixels de altura
                int index = yPos * 64 + xPos;

                // detecta colisão
                if (display[index] == 1)
                    V[0xF] = 1;

                // XOR: alterna o pixel
                display[index] ^= 1;
            }
        }
    }
}

void VM::jump(uint16_t NNN)
{
    this->PC = NNN;
}

void VM::returnSubroutine()
{
    this->SP--;
    this->PC = this->stack[this->SP];
}

void VM::callSubroutine(uint16_t NNN)
{
    this->stack[this->SP] = this->PC;
    this->SP++;
    this->PC = NNN;
}

void VM::skipIfEqual(uint8_t X, uint8_t NN)
{
    this->PC += (this->V[X] == NN) ? 2 : 0;
}

void VM::skipIfNotEqual(uint8_t X, uint8_t NN)
{
    this->PC += (this->V[X] != NN) ? 2 : 0;
}

void VM::skipIfRegistersEqual(uint8_t X, uint8_t Y)
{
    this->PC += (this->V[X] == this->V[Y]) ? 2 : 0;
}

void VM::skipIfRegistersNotEqual(uint8_t X, uint8_t Y)
{
    this->PC += (this->V[X] != this->V[Y]) ? 2 : 0;
}

void VM::jumpRelative(uint16_t NNN)
{
    this->PC = NNN + this->V[0];
}
void VM::setRegister(uint8_t X, uint8_t NN)
{
    this->V[X] = NN;
}

void VM::addImmediate(uint8_t X, uint8_t NN)
{
    this->V[X] += NN;
}

void VM::setRegisterRegister(uint8_t X, uint8_t Y)
{
    this->V[X] = this->V[Y];
}

void VM::orRegisters(uint8_t X, uint8_t Y)
{
    this->V[X] |= this->V[Y];
}

void VM::andRegisters(uint8_t X, uint8_t Y)
{
    this->V[X] &= this->V[Y];
}
void VM::xorRegisters(uint8_t X, uint8_t Y)
{
    this->V[X] ^= this->V[Y];
}

void VM::addRegisters(uint8_t X, uint8_t Y)
{
    uint16_t sum = this->V[X] + this->V[Y];
    this->V[0xF] = (sum > 0xFF) ? 1 : 0; // VF = carry
    this->V[X] = sum & 0xFF;
}
void VM::subRegisters(uint8_t X, uint8_t Y)
{
    this->V[0xF] = (this->V[X] > this->V[Y]) ? 1 : 0; // VF = NOT borrow
    this->V[X] -= this->V[Y];
}

void VM::subRegistersReverse(uint8_t X, uint8_t Y)
{
    this->V[0xF] = (this->V[Y] > this->V[X]) ? 1 : 0; // VF = NOT borrow
    this->V[X] = this->V[Y] - this->V[X];
}

void VM::shiftRight(uint8_t X)
{
    this->V[0xF] = this->V[X] & 0x1; // VF = LSB
    this->V[X] >>= 1;
}

void VM::shiftLeft(uint8_t X)
{
    this->V[0xF] = (this->V[X] & 0x80) >> 7; // VF = MSB
    this->V[X] <<= 1;
}

void VM::execute8Group(uint8_t X, uint8_t Y, uint8_t N)
{
    switch (N)
    {
    case 0x0:
        setRegisterRegister(X, Y);
        break;
    case 0x1:
        orRegisters(X, Y);
        break;
    case 0x2:
        andRegisters(X, Y);
        break;
    case 0x3:
        xorRegisters(X, Y);
        break;
    case 0x4:
        addRegisters(X, Y);
        break;
    case 0x5:
        subRegisters(X, Y);
        break;
    case 0x6:
        shiftRight(X);
        break;
    case 0x7:
        subRegistersReverse(X, Y);
        break;
    case 0xE:
        shiftLeft(X);
        break;
    default:
        printf("Instrução do grupo 8 não implementada! N: 0x%X\n", N);
    }
}

void VM::executarGrupoE(uint8_t X, uint8_t NN)
{
    switch (NN)
    {
    case 0x9E:
        // Pula próxima instrução se tecla em VX estiver pressionada
        break;
    case 0xA1:
        // Pula próxima instrução se tecla em VX não estiver pressionada
        break;
    default:
        printf("Instrução do grupo E não implementada! NN: 0x%X\n", NN);
    }
}
void VM::executarGrupoF(uint8_t X, uint8_t NN){
    switch (NN)
    {
    case 0x07:
        setRegister(X, delayTimer); // FX07
        break;
    case 0x0A:
        waitForKeyPress(X); // FX0A
        break;
    case 0x15:
        setDelayTimer(V[X]); // FX15
        break;
    case 0x18:
        setSoundTimer(V[X]); // FX18
        break;
    case 0x1E:
        addToI(V[X]); // FX1E
        break;
    case 0x29:
        setIToSprite(V[X]); // FX29
        break;
    case 0x33:
        storeBCD(V[X]); // FX33
        break;
    case 0x55:
        storeRegisters(X); // FX55
        break;
    case 0x65:
        loadRegisters(X); // FX65
        break;
    default:
        printf("Instrução FX%02X não implementada!\n", NN);
    }
}
// Inicializa a VM
VM::VM(uint16_t pc_inicial)
{

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
    uint16_t inst = (RAM[PC] << 8) | RAM[PC + 1];
    printf("Instrução: 0x%04X\n", inst);
    PC += 2;

    uint8_t grupo = inst >> 12;
    uint8_t X = (inst & 0x0F00) >> 8;
    uint8_t Y = (inst & 0x00F0) >> 4;
    uint8_t N = inst & 0x000F;
    uint8_t NN = inst & 0x00FF;
    uint16_t NNN = inst & 0x0FFF;

    switch (grupo)
    {
    case 0x0:
        cls(inst); // limpa tela ou retorna de subrotina
        break;

    case 0x1:
        jump(NNN); // 1NNN
        break;

    case 0x2:
        callSubroutine(NNN); // 2NNN
        break;

    case 0x3:
        skipIfEqual(X, NN); // 3XNN
        break;

    case 0x4:
        skipIfNotEqual(X, NN); // 4XNN
        break;

    case 0x5:
        skipIfRegistersEqual(X, Y); // 5XY0
        break;

    case 0x6:
        setRegister(X, NN); // 6XNN
        break;

    case 0x7:
        addImmediate(X, NN); // 7XNN
        break;

    case 0x8:
        execute8Group(X, Y, N); // 8XY? (chama outra função)
        break;

    case 0x9:
        skipIfRegistersNotEqual(X, Y); // 9XY0
        break;

    case 0xA:
        setI(NNN); // ANNN
        break;

    case 0xB:
        jumpRelative(NNN); // BNNN
        break;

    case 0xC:
        random(X, NN); // CXNN
        break;

    case 0xD:
        drawSprite(V[X], V[Y], N); // DXYN
        break;

    case 0xE:
        executeEGroup(X, NN); // EX9E, EXA1
        break;

    case 0xF:
        executeFGroup(X, NN); // FX??
        break;

    default:
        printf("Grupo não implementado! Instrução: 0x%04X\n", inst);
    }
}

void VM::ImprimirRegistradores()
{

    printf("PC: 0x%04X I: 0x%04X SP: 0x%02X\n", this->PC, this->I, this->SP);
    for (int i = 0; i < 16; i++)
        printf("V[%X]: 0x%02X ", i, this->V[i]);
    printf("\n");
};