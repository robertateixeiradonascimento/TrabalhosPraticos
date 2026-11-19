/*
 * Metodo 2: intercalacao balanceada de varios caminhos com f+1 fitas,
 * usando selecao por substituicao na geracao dos blocos ordenados.
 *
 * Parametros (int_f1.h): memoria interna de 19 registros e 20 fitas,
 * sendo f = 19 fitas de entrada e 1 fita de saida a cada etapa.
 *
 * Funcionamento:
 *  1) Geracao dos blocos: selecao por substituicao com um heap de 19 registros.
 *     Os blocos sao distribuidos alternadamente (round-robin) nas fitas 0..18.
 *  2) Intercalacao: a cada passada, o i-esimo bloco de cada uma das 19 fitas de
 *     entrada e intercalado em um unico bloco, gravado na fita de saida (fita 19).
 *  3) Redistribuicao: se a fita de saida ficou com mais de um bloco, seus blocos
 *     sao copiados de volta, alternadamente, para as 19 fitas de entrada, e uma
 *     nova passada de intercalacao comeca. Essa copia e a diferenca em relacao ao
 *     metodo 2f: ela gasta leituras e escritas extras, em troca de usar f fitas de
 *     entrada (19 caminhos) em vez de f/2.
 *
 * Nas fitas, o campo "marcado" do registro indica FIM DE BLOCO (1 = ultimo registro
 * do bloco). Assim as fronteiras ficam gravadas no proprio registro, sem nenhuma
 * leitura ou escrita extra.
 */
#include "int_f1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FITA_SAIDA F_CAMINHOS   /* indice da unica fita de saida (a 20a fita) */

/* no do heap de intercalacao: registro + fita de onde ele veio */
typedef struct {
    Registro reg;   /* reg.marcado = marca de fim de bloco */
    int fita;
} NoF1;

/* compara as notas de dois nos (conta uma comparacao entre chaves) */
static int compara_no_f1(const NoF1 *a, const NoF1 *b, Metricas *m) {
    m->comparacoes++;
    if (a->reg.nota > b->reg.nota) return 1;
    if (a->reg.nota < b->reg.nota) return -1;
    return 0;
}

/* desce o elemento da posicao esq ate a posicao correta do min-heap */
static void refaz_heap_f1(NoF1 *heap, int esq, int dir, Metricas *m) {
    int i = esq;
    int j = i * 2 + 1;
    NoF1 aux = heap[i];
    while (j <= dir) {
        if (j < dir && compara_no_f1(&heap[j], &heap[j + 1], m) > 0) j++;
        if (compara_no_f1(&aux, &heap[j], m) <= 0) break;
        heap[i] = heap[j];
        i = j;
        j = i * 2 + 1;
    }
    heap[i] = aux;
}

/* constroi o min-heap de baixo para cima */
static void constroi_heap_f1(NoF1 *heap, int n, Metricas *m) {
    for (int esq = n / 2 - 1; esq >= 0; esq--) refaz_heap_f1(heap, esq, n - 1, m);
}

static void nome_fita(char *buf, int i) {
    sprintf(buf, "fita_f1_%d.bin", i);
}

/* fecha e apaga todas as fitas temporarias */
static void fecha_e_remove(FILE **fitas, int n) {
    char nome[40];
    for (int i = 0; i < n; i++) {
        if (fitas[i]) fclose(fitas[i]);
        fitas[i] = NULL;
        nome_fita(nome, i);
        remove(nome);
    }
}

/* esvazia uma fita (reabre truncando). Retorna 0 em sucesso e -1 em erro */
static int trunca_fita(FILE **fitas, int i) {
    char nome[40];
    nome_fita(nome, i);
    FILE *r = freopen(nome, "wb+", fitas[i]);
    fitas[i] = r;
    if (!r) {
        fprintf(stderr, "Erro: nao foi possivel reabrir a fita temporaria '%s'.\n", nome);
        return -1;
    }
    return 0;
}

/* grava um registro na fita contando a transferencia de escrita */
static void grava(FILE *fita, Registro *r, Metricas *m) {
    fwrite(r, sizeof(Registro), 1, fita);
    m->transferencias_escrita++;
}

/* Fase 1: selecao por substituicao com memoria de TAM_MEMORIA_F1 registros.
   Grava os blocos alternadamente nas f fitas de entrada e preenche blocos_fita[].
   Retorna o total de blocos gerados. */
static int gera_blocos_f1(FILE *entrada, FILE **fitas, int f, int *blocos_fita, Metricas *m) {
    Registro heap[TAM_MEMORIA_F1];
    int tam = 0, marcados = 0, total = 0, fita_atual = 0;
    for (int i = 0; i < f; i++) blocos_fita[i] = 0;

    /* enche a memoria interna */
    while (tam < TAM_MEMORIA_F1 && ler_registro_binario(entrada, &heap[tam], m)) {
        heap[tam].marcado = 0;
        tam++;
    }
    if (tam == 0) return 0;
    constroi_heap(heap, tam, m);
    blocos_fita[fita_atual]++;
    total = 1;

    while (tam > 0) {
        Registro menor = heap[0];
        Registro novo;
        if (ler_registro_binario(entrada, &novo, m)) {
            /* registro menor que o que acabou de sair pertence ao proximo bloco */
            m->comparacoes++;
            if (novo.nota < menor.nota) { novo.marcado = 1; marcados++; }
            else novo.marcado = 0;
            heap[0] = novo;
        } else {
            heap[0] = heap[tam - 1];   /* entrada acabou: a memoria vai esvaziando */
            tam--;
        }
        if (tam > 0) refaz_heap(heap, 0, tam - 1, m);

        /* o bloco termina quando todos os registros na memoria estao marcados */
        int fim_bloco = (tam == 0) || (marcados == tam);
        menor.marcado = fim_bloco;
        grava(fitas[fita_atual], &menor, m);

        if (fim_bloco && tam > 0) {
            for (int i = 0; i < tam; i++) heap[i].marcado = 0;
            marcados = 0;
            constroi_heap(heap, tam, m);
            fita_atual = (fita_atual + 1) % f;   /* proximo bloco vai para a proxima fita */
            blocos_fita[fita_atual]++;
            total++;
        }
    }
    return total;
}

/* Intercala UM bloco de cada fita participante em um unico bloco na fita de saida. */
static void intercala_um_bloco(FILE **fitas, const int *part, int np, FILE *saida, Metricas *m) {
    NoF1 heap[F_CAMINHOS];
    int tam = 0;
    for (int k = 0; k < np; k++) {
        Registro r;
        if (ler_registro_binario(fitas[part[k]], &r, m)) {
            heap[tam].reg = r;
            heap[tam].fita = part[k];
            tam++;
        }
    }
    constroi_heap_f1(heap, tam, m);

    while (tam > 0) {
        NoF1 menor = heap[0];
        Registro prox;
        if (!menor.reg.marcado && ler_registro_binario(fitas[menor.fita], &prox, m)) {
            heap[0].reg = prox;           /* a fita ainda tem registros neste bloco */
        } else {
            heap[0] = heap[tam - 1];      /* o bloco desta fita acabou */
            tam--;
        }
        if (tam > 0) refaz_heap_f1(heap, 0, tam - 1, m);

        Registro saida_reg = menor.reg;
        saida_reg.marcado = (tam == 0);   /* ultimo registro do bloco intercalado */
        grava(saida, &saida_reg, m);
    }
}

/* Redistribui os blocos da fita de saida, alternadamente, nas f fitas de entrada.
   Retorna 0 em sucesso e -1 em erro. */
static int redistribui(FILE **fitas, int f, int *blocos_fita, int total, Metricas *m) {
    for (int i = 0; i < f; i++) {
        if (trunca_fita(fitas, i) != 0) return -1;
        blocos_fita[i] = 0;
    }
    rewind(fitas[FITA_SAIDA]);
    Registro r;
    int fita = 0;
    for (int b = 0; b < total; b++) {
        blocos_fita[fita]++;
        /* copia o bloco registro a registro ate a marca de fim de bloco */
        while (ler_registro_binario(fitas[FITA_SAIDA], &r, m)) {
            grava(fitas[fita], &r, m);
            if (r.marcado) break;
        }
        fita = (fita + 1) % f;
    }
    for (int i = 0; i < f; i++) { fflush(fitas[i]); rewind(fitas[i]); }
    return trunca_fita(fitas, FITA_SAIDA);
}

/* mostra os registros de entrada (opcao -P), sem contar nas metricas */
static void imprime_entrada(FILE *arq) {
    Registro reg;
    printf("=== Dados de entrada ===\n");
    while (fread(&reg, sizeof(Registro), 1, arq) == 1) {
        printf("%08ld %5.1f %-2s %-50s %-30s\n",
               reg.inscricao, reg.nota, reg.estado, reg.cidade, reg.curso);
    }
    printf("=== Resultado ordenado ===\n");
    rewind(arq);
}

int executa_metodo_2(const char *nome_arquivo_entrada, int imprimir_tela, Metricas *m) {
    const int f = F_CAMINHOS;

    FILE *entrada = fopen(nome_arquivo_entrada, "rb");
    if (!entrada) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo de entrada '%s'.\n", nome_arquivo_entrada);
        return -1;
    }
    if (imprimir_tela) imprime_entrada(entrada);

    double inicio = tempoAtual();

    /* cria as f+1 fitas temporarias */
    FILE *fitas[NUM_FITAS_F1];
    char nome[40];
    for (int i = 0; i < NUM_FITAS_F1; i++) fitas[i] = NULL;
    for (int i = 0; i < NUM_FITAS_F1; i++) {
        nome_fita(nome, i);
        fitas[i] = fopen(nome, "wb+");
        if (!fitas[i]) {
            fprintf(stderr, "Erro: nao foi possivel criar a fita temporaria '%s'.\n", nome);
            fecha_e_remove(fitas, NUM_FITAS_F1);
            fclose(entrada);
            return -1;
        }
    }

    int blocos[F_CAMINHOS];   /* blocos ainda nao intercalados em cada fita de entrada */

    /* Fase 1: geracao dos blocos ordenados */
    int total = gera_blocos_f1(entrada, fitas, f, blocos, m);
    fclose(entrada);
    for (int i = 0; i < f; i++) { fflush(fitas[i]); rewind(fitas[i]); }

    /* com um unico bloco o arquivo ja esta ordenado na fita 0 */
    int fita_final = 0;

    /* Fase 2: passadas de intercalacao (f fitas -> 1 fita) + redistribuicao */
    while (total > 1) {
        int novos = 0;
        for (;;) {
            /* participam desta intercalacao as fitas que ainda tem bloco */
            int part[F_CAMINHOS], np = 0;
            for (int i = 0; i < f; i++)
                if (blocos[i] > 0) { part[np++] = i; blocos[i]--; }
            if (np == 0) break;
            intercala_um_bloco(fitas, part, np, fitas[FITA_SAIDA], m);
            novos++;
        }
        fflush(fitas[FITA_SAIDA]);
        total = novos;
        if (total == 1) { fita_final = FITA_SAIDA; break; }

        /* mais de um bloco: volta a distribuir os blocos nas f fitas de entrada */
        if (redistribui(fitas, f, blocos, total, m) != 0) {
            fecha_e_remove(fitas, NUM_FITAS_F1);
            return -1;
        }
    }

    m->tempo_execucao = tempoAtual() - inicio;

    int status;
    if (total == 0) {
        FILE *vazio = fopen("resultado_metodo2.txt", "w");   /* entrada vazia */
        status = vazio ? 0 : -1;
        if (vazio) fclose(vazio);
    } else {
        fflush(fitas[fita_final]);
        nome_fita(nome, fita_final);
        status = finaliza_saida(nome, "resultado_metodo2.txt", imprimir_tela);
    }
    fecha_e_remove(fitas, NUM_FITAS_F1);
    return status;
}
