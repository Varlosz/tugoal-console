# Makefile - Projeto TUGOAL

# Nome do ficheiro final
TARGET = tugoal.exe

# Compilador (gcc) e Opções (-Wall mostra avisos)
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra

# Ficheiros do código
SRC = src/menu.c src/equipas.c src/jogos.c

# 1. Comando principal (escreve 'make')
all:
	@echo "A compilar..."
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "Feito! Podes correr o programa."

# 2. Correr o programa (escreve 'make run')
run: all
	./$(TARGET)

# 3. Limpar (escreve 'make clean')
clean:
	del $(TARGET)