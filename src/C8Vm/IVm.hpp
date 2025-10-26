#include <cstdint>
#include <stdint.h>
class IVM
{

public:
    virtual void CarregarROM(char *arq_rom, uint16_t pc_inicial) = 0;
    virtual void ExecutarInstrucao() = 0;
    virtual void ImprimirRegistradores() = 0;
};
