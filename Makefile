CXX = g++
SRC = main.cpp src/Display/Display.cpp src/Keyboard/Keyboard.cpp src/C8Vm/c8vm.cpp
OUT = main
CXXFLAGS = $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2)
GAMES_DIR = ./games
IMGS_DIR = ./Imagens-Teste

#lvo padrão (só compila)
all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LDFLAGS)

#ogos
pong: all
	./$(OUT) $(GAMES_DIR)/PONG

invaders: all
	./$(OUT) $(GAMES_DIR)/INVADERS

#Imagens de teste
ibm: all
	./$(OUT) $(IMGS_DIR)/IBM\ Logo.ch8

logo: all
	./$(OUT) $(IMGS_DIR)/1-chip8-logo.ch8

#Limpar
clean:
	rm -f $(OUT)
