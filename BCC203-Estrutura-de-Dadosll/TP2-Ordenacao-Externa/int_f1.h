#ifndef INT_F1_H
#define INT_F1_H

#include <stdio.h>
#include "registro.h"
#include "int2f.h"   /* reaproveita Metricas, heap de selecao por substituicao e finaliza_saida */

/* Metodo 2: intercalacao balanceada de varios caminhos com f+1 fitas
   (intercala f fitas em 1 e redistribui os blocos a cada passada).
   Parametros do enunciado do TP02 para o metodo 2:
     - memoria interna de no maximo 19 registros (TAM_MEMORIA_F1);
     - 20 fitas: 19 de entrada (F_CAMINHOS = f) + 1 de saida => NUM_FITAS_F1 = f + 1.
   (O metodo 1 tem seus proprios limites em int2f.h: TAM_MEMORIA = 10.) */
#ifndef F_CAMINHOS
#define F_CAMINHOS      19
#endif
#ifndef TAM_MEMORIA_F1
#define TAM_MEMORIA_F1  19
#endif
#define NUM_FITAS_F1    (F_CAMINHOS + 1)

/* retorna 0 em caso de sucesso e -1 em caso de erro; resultado em resultado_metodo2.txt */
int executa_metodo_2(const char *nome_arquivo_entrada, int imprimir_tela, Metricas *m);

#endif
