#include <iostream>
#include "src/C8Vm/c8vm.hpp"

int main(int argc, char const *argv[])
{
    VM vm(0x200);
    vm.CarregarROM(argv[1], 0x200);
    for (size_t i = 0; i < 20; i++) vm.ExecutarInstrucao();
    vm.ImprimirRegistradores();
    return 0;
}
