#include <stdio.h>
#include <stdlib.h>
#include "aluno.h"

int main() {
    int num_processos;
    if (scanf("%d", &num_processos) != 1)
     return 0;
    
    Processo *processos = (Processo*) malloc(num_processos * sizeof(Processo));
    
    for (int i = 0; i < num_processos; ++i) {
        scanf("%d %d %ld", &processos[i].tempo_aloc, &processos[i].tempo_desaloc, &processos[i].tam);
    }

    long *alocacoes = first_fit(processos, num_processos);

    for (int i = 0; i < num_processos; i++) {
        printf("Processo %d alocado na posição %ld\n", i, alocacoes[i]);
    }

    free(alocacoes);
    free(processos);

    return 0;
}