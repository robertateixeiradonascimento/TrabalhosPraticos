# TP2 - BCC203 - Ordenacao Externa
# make          compila o programa "ordena"
# make clean    apaga objetos, executavel e arquivos de resultado
# make cleanall tambem apaga os binarios gerados a partir do PROVAO.TXT
CC = gcc
CFLAGS = -Wall -Wextra -O2
OBJS = main.o quicksort.o int2f.o int_f1.o preparo.o

ordena: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o: main.c registro.h quicksort.h int2f.h int_f1.h preparo.h
int_f1.o: int_f1.c int_f1.h int2f.h registro.h
quicksort.o: quicksort.c quicksort.h registro.h
int2f.o: int2f.c int2f.h registro.h
preparo.o: preparo.c preparo.h quicksort.h registro.h

clean:
	rm -f *.o ordena pesquisa provaoOrdenar.dat resultado_metodo*.txt fita_*.bin

cleanall: clean
	rm -f binario.bin crescente.bin decrescente.bin

.PHONY: clean cleanall
