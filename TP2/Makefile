# ------------------------------------- #
# Nome: Vitor Augusto Reis Azevedo
# Algoritmos 1 - 2024/2 - TW
# Trabalho Prático 2
# ------------------------------------- #

# Compilador
CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Wformat-security -Wconversion -Werror -I$(INC_DIR)

# Arquivo final
TARGET = tp2

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Objetos para compilação
OBJS = $(OBJ_DIR)/tp2.o $(OBJ_DIR)/Grafo.o

## REGRAS DE COMPILAÇÃO
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

all: $(TARGET)

run: $(TARGET)
	clear
	./$(TARGET) < testCase.txt

clear:
	rm $(OBJ_DIR)/* $(TARGET)

valgrind: $(TARGET)
	clear
	valgrind -s --leak-check=yes ./$(TARGET) 1 2