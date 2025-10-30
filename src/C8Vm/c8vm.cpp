// ...existing code...
#include "c8vm.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdint.h>
#include <random>

// Helper: mapeia tecla CHIP-8 -> SDL_Keycode
static SDL_Keycode chip8ToSDLKey(uint8_t key)
{
    static const SDL_Keycode keyMap[16] = {
        SDLK_x, SDLK_1, SDLK_2, SDLK_3,
        SDLK_q, SDLK_w, SDLK_e, SDLK_a,
        SDLK_s, SDLK_d, SDLK_z, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v
    };
    return keyMap[key & 0xF];
}

// ---------- Operações básicas da VM ----------

void VM::cls(uint16_t inst)
{
    // CLS (0x00E0)
    if (inst == 0x00E0)
    {
        for (int i = 0; i < 64 * 32; i++)
            this->DISPLAY[i] = 0;
    }
}

void VM::jump(uint16_t NNN)
{
    // Jump to address NNN
    this->PC = NNN;
}

void VM::callSubroutine(uint16_t NNN)
{
    // Call subroutine at NNN
    this->stack[this->SP] = this->PC;
    this->SP++;
    this->PC = NNN;
}

void VM::returnSubroutine()
{
    if (this->SP > 0) {
        // Return from subroutine
        this->SP--;
        this->PC = this->stack[this->SP];
    }
}

void VM::jumpRelative(uint16_t NNN)
{
    // Jump to address NNN + V0
    this->PC = NNN + this->V[0];
}

void VM::setIndex(uint16_t NNN)
{
    // Set index register I to NNN
    this->I = NNN;
}

// ---------- Operações sobre registradores ----------

void VM::setRegister(uint8_t X, uint8_t NN)
{
    // Set V[X] to NN
    this->V[X] = NN;
}

void VM::addImmediate(uint8_t X, uint8_t NN)
{
    // Add NN to V[X]
    this->V[X] += NN;
}

void VM::setRegisterRegister(uint8_t X, uint8_t Y)
{
    // Set V[X] to V[Y]
    this->V[X] = this->V[Y];
}

void VM::orRegisters(uint8_t X, uint8_t Y)
{
    // V[X] = V[X] OR V[Y]
    this->V[X] |= this->V[Y];
}

void VM::andRegisters(uint8_t X, uint8_t Y)
{
    // V[X] = V[X] AND V[Y]
    this->V[X] &= this->V[Y];
}

void VM::xorRegisters(uint8_t X, uint8_t Y)
{
    // V[X] = V[X] XOR V[Y]
    this->V[X] ^= this->V[Y];
}

void VM::addRegisters(uint8_t X, uint8_t Y)
{
    // V[X] = V[X] + V[Y], set VF = carry
    uint16_t sum = this->V[X] + this->V[Y];
    this->V[0xF] = (sum > 0xFF) ? 1 : 0;
    this->V[X] = sum & 0xFF;
}

void VM::subRegisters(uint8_t X, uint8_t Y)
{
    // V[X] = V[X] - V[Y], set VF = NOT borrow
    this->V[0xF] = (this->V[X] > this->V[Y]) ? 1 : 0;
    this->V[X] -= this->V[Y];
}

void VM::subRegistersReverse(uint8_t X, uint8_t Y)
{
    // V[X] = V[Y] - V[X], set VF = NOT borrow
    this->V[0xF] = (this->V[Y] > this->V[X]) ? 1 : 0;
    this->V[X] = this->V[Y] - this->V[X];
}

void VM::shiftRight(uint8_t X)
{
    // V[X] = V[X] >> 1, set VF = least significant bit before shift
    this->V[0xF] = this->V[X] & 0x1;
    this->V[X] >>= 1;
}

void VM::shiftLeft(uint8_t X)
{
    // V[X] = V[X] << 1, set VF = most significant bit before shift
    this->V[0xF] = (this->V[X] & 0x80) >> 7;
    this->V[X] <<= 1;
}

// ---------- Comparações / skips ----------

void VM::skipIfEqual(uint8_t X, uint8_t NN)
{
    // Skip next instruction if V[X] == NN
    this->PC += (this->V[X] == NN) ? 2 : 0;
}

void VM::skipIfNotEqual(uint8_t X, uint8_t NN)
{
    // Skip next instruction if V[X] != NN
    this->PC += (this->V[X] != NN) ? 2 : 0;
}

void VM::skipIfRegistersEqual(uint8_t X, uint8_t Y)
{
    // Skip next instruction if V[X] == V[Y]
    this->PC += (this->V[X] == this->V[Y]) ? 2 : 0;
}

void VM::skipIfRegistersNotEqual(uint8_t X, uint8_t Y)
{
    // Skip next instruction if V[X] != V[Y]
    this->PC += (this->V[X] != this->V[Y]) ? 2 : 0;
}

// ---------- Memória / índice / BCD / dumps ----------

void VM::addToIndex(uint8_t X)
{
    // I = I + V[X]
    this->I += this->V[X];
}

void VM::setIndexToFont(uint8_t X)
{
    // I = location of sprite for digit V[X]
    this->I = this->V[X] * 5;
}

void VM::storeBCD(uint8_t X)
{
    // Store BCD representation of V[X] in memory at I, I+1, I+2
    uint8_t value = this->V[X];
    this->RAM[this->I]     = value / 100;
    this->RAM[this->I + 1] = (value / 10) % 10;
    this->RAM[this->I + 2] = value % 10;
}

void VM::storeRegisters(uint8_t X)
{
    // Store registers V[0] to V[X] in memory starting at I
    for (uint8_t i = 0; i <= X; i++)
        this->RAM[this->I + i] = this->V[i];
}

void VM::loadRegisters(uint8_t X)
{
    // Load registers V[0] to V[X] from memory starting at I
    for (uint8_t i = 0; i <= X; i++)
        this->V[i] = this->RAM[this->I + i];
}

// ---------- Display / draw ----------

void VM::drawSprite(uint8_t X, uint8_t Y, uint8_t N)
{
    // Draw sprite at (V[X], V[Y]) with N bytes of sprite data starting at I
    uint8_t xPos = this->V[X] % 64;
    uint8_t yPos = this->V[Y] % 32;
    this->V[0xF] = 0;

    for (int row = 0; row < N; row++)
    {
        uint8_t spriteByte = this->RAM[this->I + row];
        for (int col = 0; col < 8; col++)
        {
            if ((spriteByte & (0x80 >> col)) != 0)
            {
                int pixelIndex = (xPos + col) % 64 + ((yPos + row) % 32) * 64;
                if (this->DISPLAY[pixelIndex] == 1)
                    this->V[0xF] = 1;
                this->DISPLAY[pixelIndex] ^= 1;
            }
        }
    }
}

// ---------- Teclado (SKP / SKNP) ----------

void VM::skipIfKeyPressed(uint8_t X)
{
    // Skip next instruction if key with value V[X] is pressed
    SDL_Keycode key = chip8ToSDLKey(this->V[X]);
    if (this->keyboard && this->keyboard->IsKeyPressed(key))
        this->PC += 2;
}

void VM::skipIfKeyNotPressed(uint8_t X)
{
    // Skip next instruction if key with value V[X] is not pressed
    SDL_Keycode key = chip8ToSDLKey(this->V[X]);
    if (this->keyboard && !this->keyboard->IsKeyPressed(key))
        this->PC += 2;
}

// ---------- RNG (CNN) ----------

void VM::random(uint8_t X, uint8_t NN)
{
    // Set V[X] to random byte AND NN
    static std::mt19937 rng((uint32_t)SDL_GetTicks());
    std::uniform_int_distribution<int> dist(0, 255);
    this->V[X] = (uint8_t)(dist(rng) & NN);
}

// ---------- Grupo 8 (já existente) ----------

void VM::execute8Group(uint8_t X, uint8_t Y, uint8_t N)
{
    switch (N)
    {
    case 0x0: setRegisterRegister(X, Y); break;
    case 0x1: orRegisters(X, Y); break;
    case 0x2: andRegisters(X, Y); break;
    case 0x3: xorRegisters(X, Y); break;
    case 0x4: addRegisters(X, Y); break;
    case 0x5: subRegisters(X, Y); break;
    case 0x6: shiftRight(X); break;
    case 0x7: subRegistersReverse(X, Y); break;
    case 0xE: shiftLeft(X); break;
    default:
        printf("Instrução do grupo 8 não implementada! N: 0x%X\n", N);
    }
}

// ---------- Grupo F (timers, BCD, mem ops...) ----------

void VM::GrupoF(uint8_t X, uint8_t NN)
{
    switch (NN)
    {
    case 0x07: loadDelayTimer(X); break;   // LD Vx, DT
    case 0x0A: { // LD Vx, K  -> aguarda tecla: se nenhuma tecla pressionada, retrocede PC para repetir instrução
        if (!this->keyboard) break;
        auto keys = this->keyboard->GetChip8Keys();
        bool pressed = false;
        for (uint8_t i = 0; i < 16; ++i) {
            if (keys[i]) { this->V[X] = i; pressed = true; break; }
        }
        if (!pressed) {
            // não avançar: ajustar PC para repetir a instrução na próxima iteração
            this->PC -= 2;
        }
        break;
    }
    case 0x15: setDelayTimer(X); break;    // LD DT, Vx
    case 0x18: setSoundTimer(X); break;    // LD ST, Vx
    case 0x1E: addToIndex(X); break;       // ADD I, Vx
    case 0x29: setIndexToFont(X); break;   // LD F, Vx
    case 0x33: storeBCD(X); break;         // LD B, Vx
    case 0x55: storeRegisters(X); break;   // LD [I], Vx
    case 0x65: loadRegisters(X); break;    // LD Vx, [I]
    default:
        printf("Grupo F não implementado: 0x%02X\n", NN);
    }
}

void VM::TickTimers()
{
    if (delayTimer > 0) --delayTimer;
    if (soundTimer > 0) --soundTimer;
}

// ---------- Grupo E (key ops: SKP / SKNP) ----------
void VM::GrupoE(uint8_t X, uint8_t NN)
{
    switch (NN)
    {
    case 0x9E: skipIfKeyPressed(X); break;    // SKP Vx
    case 0xA1: skipIfKeyNotPressed(X); break; // SKNP Vx
    default:
        printf("Grupo E não implementado: 0x%02X\n", NN);
    }
}

// ---------- Timers ----------

void VM::loadDelayTimer(uint8_t X) { this->V[X] = this->delayTimer; }
void VM::setDelayTimer(uint8_t X)  { this->delayTimer = this->V[X]; }
void VM::setSoundTimer(uint8_t X)  { this->soundTimer = this->V[X]; }

// ---------- Inicialização / ROM / execução ----------

VM::VM(uint16_t pc_inicial, Display *disp, Keyboard *keyb)
{
    PC = pc_inicial;
    display = disp;
    keyboard = keyb;
    I = 0;
    SP = 0;
    for (int i = 0; i < 16; i++) V[i] = 0;
    for (int i = 0; i < 4096; i++) RAM[i] = 0;
    for (int i = 0; i < 64 * 32; i++) DISPLAY[i] = 0;
    for (int i = 0; i < 16; i++) stack[i] = 0;
    delayTimer = 0;
    soundTimer = 0;
}
// Carrega uma ROM na memória da VM
void VM::CarregarROM(const char *arq_rom, uint16_t pc_inicial, Display *disp, Keyboard *keyb)
{
    this->display = disp;
    this->keyboard = keyb;
    FILE *rom = fopen(arq_rom, "rb");
    if (!rom) {
        printf("Erro ao abrir ROM: %s\n", arq_rom);
        return;
    }
    fseek(rom, 0, SEEK_END);
    long tam_rom = ftell(rom);
    rewind(rom);
    if (tam_rom > 0 && (pc_inicial + tam_rom) <= 4096) {
        fread(&this->RAM[pc_inicial], 1, tam_rom, rom);
        printf("ROM carregada em 0x%03X, tamanho: %ld bytes. Primeiro bytes: %02X %02X %02X\n",
            pc_inicial, tam_rom, RAM[pc_inicial], RAM[pc_inicial+1], RAM[pc_inicial+2]);
    } else {
        printf("ROM muito grande ou tamanho inválido: %ld\n", tam_rom);
    }
    fclose(rom);
}

// Executa uma instrução da VM
void VM::ExecutarInstrucao()
{
    uint16_t inst = (RAM[PC] << 8) | RAM[PC + 1];
    printf("Instrução: 0x%04X\n", inst);

    // decodifica campos
    uint8_t grupo = inst >> 12;
    uint8_t X = (inst & 0x0F00) >> 8;
    uint8_t Y = (inst & 0x00F0) >> 4;
    uint8_t N = inst & 0x000F;
    uint8_t NN = inst & 0x00FF;
    uint16_t NNN = inst & 0x0FFF;

    // por padrão avança PC em 2, algumas instruções alteram PC diretamente
    this->PC += 2;

    switch (grupo)
    {
    case 0x0:
        cls(inst); // 00E0 / 00EE
        // se for RET (00EE) tratar na cls? aqui tratamos 00EE com retorno direto:
        if (inst == 0x00EE) returnSubroutine();
        break;
    case 0x1: jump(NNN); break;               // 1NNN
    case 0x2: callSubroutine(NNN); break;     // 2NNN
    case 0x3: skipIfEqual(X, NN); break;      // 3XNN
    case 0x4: skipIfNotEqual(X, NN); break;   // 4XNN
    case 0x5: skipIfRegistersEqual(X, Y); break; // 5XY0
    case 0x6: setRegister(X, NN); break;      // 6XNN
    case 0x7: addImmediate(X, NN); break;     // 7XNN
    case 0x8: execute8Group(X, Y, N); break;  // 8XYN
    case 0x9: skipIfRegistersNotEqual(X, Y); break; // 9XY0
    case 0xA: setIndex(NNN); break;           // ANNN
    case 0xB: jumpRelative(NNN); break;       // BNNN
    case 0xC: random(X, NN); break;           // CXNN
    case 0xD: drawSprite(X, Y, N); break;     // DXYN
    case 0xE: GrupoE(X, NN); break;           // EX9E / EXA1
    case 0xF: GrupoF(X, NN); break;           // FX07, FX15, FX1E...
    default:
        printf("Grupo não implementado! Instrução: 0x%04X\n", inst);
    }

    // NOTA: timers devem ser decrementados externamente a ~60Hz; pode implementar aqui se desejar.
}

void VM::ImprimirRegistradores()
{
    // Imprime o estado dos registradores da VM
    printf("PC: 0x%04X I: 0x%04X SP: 0x%02X\n", this->PC, this->I, this->SP);
    for (int i = 0; i < 16; i++)
        printf("V[%X]: 0x%02X ", i, this->V[i]);
    printf("\n");
}

void VM::run()
{
    while (true)
    {
        //
        ExecutarInstrucao();
        ImprimirRegistradores();
    }
}
