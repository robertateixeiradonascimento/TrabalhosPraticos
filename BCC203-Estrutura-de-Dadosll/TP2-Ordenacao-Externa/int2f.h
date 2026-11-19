#ifndef INT2F_H
#define INT2F_H

#include <stdio.h>
#include "registro.h"

//parametros do metodo 1 conforme o enunciado do tp02
#define TAM_MEMORIA 10      //registros que cabem na memoria interna
#define NUM_FITAS_LADO 10   //fitas de entrada (ou de saida) a cada passada
#define NUM_FITAS_TOTAL 20  //total de fitas de armazenamento externo

//estrutura de metricas para o relatorio do tp02
typedef struct {
    long int transferencias_leitura;
    long int transferencias_escrita;
    long int comparacoes;      //so conta comparacoes entre notas (campo de ordenacao)
    double tempo_execucao;     //em segundos
} Metricas;

//Registro agora e definido em registro.h (unico para os 3 metodos)

//estrutura auxiliar para saber de qual fita o registro veio
typedef struct {
    Registro reg;
    int fita_origem;
} NoIntercalacao;

//funcoes de geracao de blocos e operacoes com arquivo
int compara_registros(Registro a, Registro b, Metricas *m);
void refaz_heap(Registro *heap, int esq, int dir, Metricas *m);
void constroi_heap(Registro *heap, int n, Metricas *m);
int ler_registro_binario(FILE *arq, Registro *reg, Metricas *m);
int geracao_blocos_substituicao(FILE *arq_entrada, FILE **fitas_saida, int num_fitas_saida, Metricas *m);
int finaliza_saida(const char *bin_final, const char *txt_saida, int imprimir_tela);

//funcoes da fase de intercalacao 2f
int compara_no_intercalacao(NoIntercalacao a, NoIntercalacao b, Metricas *m);
void refaz_heap_intercalacao(NoIntercalacao *heap, int esq, int dir, Metricas *m);
void constroi_heap_intercalacao(NoIntercalacao *heap, int n, Metricas *m);
int intercalacao_2f(FILE **fitas_entrada, FILE **fitas_saida, int num_fitas, Metricas *m);

//funcao principal do metodo 1: retorna 0 em caso de sucesso e -1 em caso de erro
//tempo de relogio em segundos (clock_gettime monotonic), usado nas medicoes
double tempoAtual(void);

int executa_metodo_1(const char *nome_arquivo_entrada, int imprimir_tela, Metricas *m);

#endif