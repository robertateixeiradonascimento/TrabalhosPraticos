#ifndef ALUNO_H
#define ALUNO_H


typedef struct {
    int tempo_aloc;
    int tempo_desaloc;
    long tam;
} Processo;

typedef struct {
    int tempo;
    int tipo; 
    int processo;
} Evento;

typedef struct {
    long inicio;
    long tam;
} Intervalo;

typedef struct No {
    Intervalo bloco;
    struct No *proximo;
} No;

void ordena_eventos_merge(Evento *v, int l, int r);
void ordena_eventos(Evento *eventos, int l , int m, int r);
long *first_fit(Processo *processos, int num_processos);
long alocar_processo(No **memoria, long tamanho);
void desalocar_processo(No **memoria, long inicio, long tamanho);
void juntar_intervalos(No *memoria);
void liberaMemoria(No *memoria);

#endif