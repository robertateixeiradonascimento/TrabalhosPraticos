#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdio.h>

#define TAMAREA 10
#define TRUE 1
#define FALSE 0

#include "registro.h"   /* TipoRegistro agora vem daqui */

typedef struct {
    TipoRegistro itens[TAMAREA];
    int numElementos;
} TipoArea;

extern long long num_leituras;
extern long long num_escritas;
extern long long num_comparacoes;

void FAVazia(TipoArea *Area);
int ObterNumCelOcupadas(TipoArea Area);
void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea);
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea);
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea);

void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer);
void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es);
void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei);

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea *Area, int Esq, int Dir, int *i, int *j);
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir);

int prepararSituacao(const char *arqBinOrig, const char *arqBinTeste, int n, int situacao);
void imprimirArquivoBinario(const char *nomeArqBin, int n);

void quickSortCrescente(TipoRegistro *v, int esq, int dir);
void quickSortDecrescente(TipoRegistro *v, int esq, int dir);

#endif