
O **Chip-8** é uma linguagem interpretada criada por Joseph Weisbecker nos anos 70, usada em microcomputadores de 8 bits (ex.: COSMAC VIP).

---

## Estrutura do projeto

implementation-of-a-Chip-8-Virtual-Machine/
- main.cpp
- src/
  - Display/Display.cpp
  - Keyboard/Keyboard.cpp
  - C8Vm/c8vm.cpp
- games/ (ROMs)
  - PONG
  - INVADERS
  - ...
- Imagens-Teste/
  - "IBM Logo.ch8"
  - "1-chip8-logo.ch8"
- Makefile
- README.md

---

## Instalação (dependências)

No Ubuntu / Debian:
```bash
sudo apt update
sudo apt install build-essential git libsdl2-dev pkg-config
```

No Fedora:
```bash
sudo dnf install gcc make git pkgconf SDL2-devel
```

---

## Compilar e executar

Compilar com o Makefile:
```bash
# compila (alvo padrão)
make
```

Executar jogos com alvos do Makefile:
```bash
make pong       # executa games/PONG
make invaders   # executa games/INVADERS
```

Executar imagens de teste:
```bash
make ibm
make logo
```

Limpar binário:
```bash
make clean
```

Execução manual (sem Makefile):
```bash
./main ./games/PONG
./main ./games/INVADERS
./main "./Imagens-Teste/IBM Logo.ch8"
./main "./Imagens-Teste/1-chip8-logo.ch8"
```

