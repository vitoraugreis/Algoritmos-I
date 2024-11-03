# ------------------------------------- #
# Nome: Vitor Augusto Reis Azevedo
# Algoritmos 1 - 2024/2 - TW
# Trabalho Prático 1
# ------------------------------------- #

# Compilador
CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Wformat-security -Wconversion -Werror -I$(INC_DIR)

# Arquivo final
TARGET = tp1

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Objetos para compilação
OBJS = $(OBJ_DIR)/tp1.o $(OBJ_DIR)/Grafo.o

## REGRAS DE COMPILAÇÃO
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $^

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

all: $(TARGET)

run: $(TARGET)
	clear
	./$(TARGET)

clear:
	rm $(OBJ_DIR)/* tp1

valgrind: $(TARGET)
	clear
	valgrind -s --leak-check=yes ./$(TARGET) 1 2