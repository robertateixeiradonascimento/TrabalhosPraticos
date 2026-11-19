/*
 * TP2 - BCC203 (Estrutura de Dados II) - Ordenacao Externa
 *
 * Uso: ordena <metodo> <quantidade> <situacao> [-P]
 *   metodo     : 1 = intercalacao balanceada de varios caminhos (2f fitas)
 *                2 = intercalacao balanceada de varios caminhos (f+1 fitas)
 *                3 = Quicksort Externo
 *   quantidade : numero de alunos (linhas iniciais do PROVAO.TXT) a ordenar
 *   situacao   : 1 = arquivo ordenado ascendentemente pelas notas
 *                2 = arquivo ordenado descendentemente pelas notas
 *                3 = arquivo desordenado aleatoriamente (ordem original)
 *   -P         : opcional; mostra na tela os dados de entrada e o resultado
 *
 * Ao final, o programa sempre mostra o numero de leituras e escritas de registros
 * entre memoria interna e externa, o numero de comparacoes entre notas e o tempo
 * de execucao da ordenacao.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "registro.h"
#include "quicksort.h"
#include "int2f.h"
#include "int_f1.h"
#include "preparo.h"

/* arquivo binario com os n registros a ordenar (copia dos n primeiros da situacao) */
#define ARQ_TESTE "provaoOrdenar.dat"

static const char *nomeMetodo(int m) {
    return m == 1 ? "Intercalacao balanceada 2f fitas (selecao por substituicao)"
         : m == 2 ? "Intercalacao balanceada f+1 fitas (selecao por substituicao)"
         :          "Quicksort Externo";
}

static const char *nomeSituacao(int s) {
    return s == 1 ? "Ordenado ascendentemente"
         : s == 2 ? "Ordenado descendentemente"
         :          "Desordenado aleatoriamente";
}

/* mensagem de orientacao mostrada quando a linha de comando esta incorreta */
static void mostrarUso(const char *prog) {
    fprintf(stderr,
        "Uso: %s <metodo> <quantidade> <situacao> [-P]\n"
        "  <metodo>     1 = intercalacao balanceada 2f fitas\n"
        "               2 = intercalacao balanceada f+1 fitas\n"
        "               3 = Quicksort Externo\n"
        "  <quantidade> numero de alunos a ordenar (1 a 471705)\n"
        "  <situacao>   1 = ascendente, 2 = descendente, 3 = aleatorio\n"
        "  -P           (opcional) mostra os dados de entrada e o resultado na tela\n"
        "Exemplo: %s 1 2000 3 -P\n", prog, prog);
}

/* converte texto em inteiro, aceitando apenas numeros completos dentro de [min, max] */
static int lerInteiro(const char *txt, long min, long max, long *valor) {
    char *fim;
    errno = 0;
    long v = strtol(txt, &fim, 10);
    if (errno != 0 || fim == txt || *fim != '\0' || v < min || v > max) return 0;
    *valor = v;
    return 1;
}

static void imprimirResultados(int metodo, long quantidade, int situacao,
                               long long leituras, long long escritas,
                               long long comparacoes, double tempo) {
    printf("\n=================================================\n");
    printf("            RESULTADOS DO EXPERIMENTO\n");
    printf("=================================================\n");
    printf("Metodo                      : %d (%s)\n", metodo, nomeMetodo(metodo));
    printf("Quantidade de registros (n) : %ld\n", quantidade);
    printf("Situacao inicial            : %d (%s)\n", situacao, nomeSituacao(situacao));
    printf("-------------------------------------------------\n");
    printf("Transferencias de leitura   : %lld\n", leituras);
    printf("Transferencias de escrita   : %lld\n", escritas);
    printf("Comparacoes entre notas     : %lld\n", comparacoes);
    printf("Tempo de execucao           : %.6f segundos\n", tempo);
    printf("=================================================\n");
}

/* mostra os registros de um arquivo binario (usado com -P, fora das metricas) */
static void imprimirBinario(const char *nome, const char *titulo) {
    FILE *f = fopen(nome, "rb");
    if (!f) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s' para exibicao.\n", nome);
        return;
    }
    Registro r;
    printf("=== %s ===\n", titulo);
    while (fread(&r, sizeof(Registro), 1, f) == 1)
        printf("%08ld %5.1f %-2s %-50s %-30s\n", r.inscricao, r.nota, r.estado, r.cidade, r.curso);
    fclose(f);
}

/* Metodo 3: Quicksort Externo sobre o proprio arquivo (3 ponteiros: Li, Ei e LEs) */
static int executaMetodo3(long n, int imprimir, Metricas *m) {
    if (imprimir) imprimirBinario(ARQ_TESTE, "Dados de entrada");

    FILE *ArqLi = fopen(ARQ_TESTE, "r+b");
    FILE *ArqEi = fopen(ARQ_TESTE, "r+b");
    FILE *ArqLEs = fopen(ARQ_TESTE, "r+b");
    if (!ArqLi || !ArqEi || !ArqLEs) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s' para o Quicksort Externo.\n", ARQ_TESTE);
        if (ArqLi) fclose(ArqLi);
        if (ArqEi) fclose(ArqEi);
        if (ArqLEs) fclose(ArqLEs);
        return -1;
    }

    /* zera os contadores globais (eles tambem sao usados ao gerar crescente.bin) */
    num_leituras = num_escritas = num_comparacoes = 0;
    double inicio = tempoAtual();
    QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, (int)n);
    fclose(ArqLi);
    fclose(ArqEi);
    fclose(ArqLEs);
    m->tempo_execucao = tempoAtual() - inicio;

    m->transferencias_leitura = num_leituras;
    m->transferencias_escrita = num_escritas;
    m->comparacoes = num_comparacoes;

    if (imprimir) printf("=== Resultado ordenado ===\n");
    return finaliza_saida(ARQ_TESTE, "resultado_metodo3.txt", imprimir);
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Erro: numero de argumentos invalido.\n");
        mostrarUso(argv[0]);
        return 1;
    }

    long metodo, quantidade, situacao;
    int imprimir = 0;

    if (!lerInteiro(argv[1], 1, 3, &metodo)) {
        fprintf(stderr, "Erro: metodo '%s' invalido (use 1, 2 ou 3).\n", argv[1]);
        mostrarUso(argv[0]);
        return 1;
    }
    if (!lerInteiro(argv[2], 1, 100000000L, &quantidade)) {
        fprintf(stderr, "Erro: quantidade '%s' invalida (deve ser um inteiro positivo).\n", argv[2]);
        mostrarUso(argv[0]);
        return 1;
    }
    if (!lerInteiro(argv[3], 1, 3, &situacao)) {
        fprintf(stderr, "Erro: situacao '%s' invalida (use 1, 2 ou 3).\n", argv[3]);
        mostrarUso(argv[0]);
        return 1;
    }
    if (argc == 5) {
        if (strcmp(argv[4], "-P") == 0 || strcmp(argv[4], "-p") == 0) imprimir = 1;
        else {
            fprintf(stderr, "Erro: argumento opcional '%s' desconhecido (use -P).\n", argv[4]);
            mostrarUso(argv[0]);
            return 1;
        }
    }

    /* gera binario.bin, crescente.bin e decrescente.bin na primeira execucao */
    if (garantirArquivosBase() != 0) return 1;

    long total = contarRegistros(ARQ_BIN_BASE);
    if (total <= 0) {
        fprintf(stderr, "Erro: o arquivo base '%s' esta vazio ou inacessivel.\n", ARQ_BIN_BASE);
        return 1;
    }
    if (quantidade > total) {
        fprintf(stderr, "Erro: quantidade %ld maior que o total de alunos disponiveis (%ld).\n",
                quantidade, total);
        return 1;
    }

    /* copia os n primeiros registros da situacao escolhida para o arquivo a ordenar */
    if (prepararSituacao(ARQ_BIN_BASE, ARQ_TESTE, (int)quantidade, (int)situacao) != 0) return 1;

    Metricas m = {0, 0, 0, 0.0};
    int status;
    if (metodo == 1)      status = executa_metodo_1(ARQ_TESTE, imprimir, &m);
    else if (metodo == 2) status = executa_metodo_2(ARQ_TESTE, imprimir, &m);
    else                  status = executaMetodo3(quantidade, imprimir, &m);

    if (status != 0) {
        fprintf(stderr, "Erro: a ordenacao nao foi concluida.\n");
        return 1;
    }

    imprimirResultados((int)metodo, quantidade, (int)situacao,
                       m.transferencias_leitura, m.transferencias_escrita,
                       m.comparacoes, m.tempo_execucao);
    printf("Arquivo ordenado gravado em resultado_metodo%ld.txt\n", metodo);
    return 0;
}
