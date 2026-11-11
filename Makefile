# Compilador e opções de compilação
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99

# Nome do executável final
TARGET = tp_exec

# Identifica automaticamente todos os arquivos .c da pasta
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Regra padrão (compila o projeto completo)
all: $(TARGET)

# Gera o executável final a partir dos arquivos .o
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compila cada arquivo .c individualmente em um arquivo objeto .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executa o programa compilado
run: $(TARGET)
	./$(TARGET)

# Limpa os arquivos binários e objetos gerados
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean