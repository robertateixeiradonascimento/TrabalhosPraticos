#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preparo.h"
#include "quicksort.h"

static int existe(const char *nome) {
    FILE *f = fopen(nome, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}

long contarRegistros(const char *nomeArqBin) {
    FILE *f = fopen(nomeArqBin, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long bytes = ftell(f);
    fclose(f);
    return bytes / (long)sizeof(Registro);
}

/* (era o arquivo.c) provao.txt -> binario.bin */
static int textoParaBinario(void) {
    /* aceita o nome original (PROVAO.TXT) ou em minusculas (provao.txt) */
    FILE *arquivo = fopen("PROVAO.TXT", "r");
    if (!arquivo) arquivo = fopen("provao.txt", "r");
    if (!arquivo) { fprintf(stderr, "Erro: arquivo PROVAO.TXT nao encontrado na pasta do programa.\n"); return -1; }
    FILE *binario = fopen(ARQ_BIN_BASE, "wb");
    if (!binario) { printf("Erro ao criar o arquivo %s!\n", ARQ_BIN_BASE); fclose(arquivo); return -1; }

    char linha[200], temp[60];
    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strlen(linha) < 70) continue;
        Registro reg;
        memset(&reg, 0, sizeof(Registro));

        strncpy(temp, linha + 0, 8);  temp[8] = '\0';  reg.inscricao = atol(temp);
        strncpy(temp, linha + 9, 5);  temp[5] = '\0';  reg.nota = (float)atof(temp);
        strncpy(reg.estado, linha + 15, 2);   reg.estado[2] = '\0';
        strncpy(reg.cidade, linha + 18, 50);  reg.cidade[50] = '\0';
        strncpy(reg.curso,  linha + 69, 30);  reg.curso[30] = '\0';

        fwrite(&reg, sizeof(Registro), 1, binario);
    }
    fclose(arquivo);
    fclose(binario);
    return 0;
}

/* (era o crescenteEdecrecente.c) binario.bin -> crescente.bin (Quicksort Externo) -> decrescente.bin */
static int gerarCrescenteDecrescente(void) {
    FILE *orig = fopen(ARQ_BIN_BASE, "rb");
    FILE *cres = fopen("crescente.bin", "wb");
    if (!orig || !cres) { printf("Erro ao criar crescente.bin\n"); if (orig) fclose(orig); if (cres) fclose(cres); return -1; }

    Registro r;
    int total = 0;
    while (fread(&r, sizeof(Registro), 1, orig) == 1) { fwrite(&r, sizeof(Registro), 1, cres); total++; }
    fclose(orig);
    fclose(cres);

    FILE *ArqLi  = fopen("crescente.bin", "r+b");
    FILE *ArqEi  = fopen("crescente.bin", "r+b");
    FILE *ArqLEs = fopen("crescente.bin", "r+b");
    if (!ArqLi || !ArqEi || !ArqLEs) { printf("Erro ao abrir ponteiros para o Quicksort Externo!\n"); return -1; }
    QuicksortExterno(&ArqLi, &ArqEi, &ArqLEs, 1, total);
    fclose(ArqLi);
    fclose(ArqEi);
    fclose(ArqLEs);

    FILE *c = fopen("crescente.bin", "rb");
    FILE *d = fopen("decrescente.bin", "wb");
    if (!c || !d) { printf("Erro ao criar decrescente.bin\n"); if (c) fclose(c); if (d) fclose(d); return -1; }
    for (int i = total - 1; i >= 0; i--) {
        fseek(c, (long)i * sizeof(Registro), SEEK_SET);
        if (fread(&r, sizeof(Registro), 1, c) == 1) fwrite(&r, sizeof(Registro), 1, d);
    }
    fclose(d);
    fclose(c);
    return 0;
}

int garantirArquivosBase(void) {
    if (!existe(ARQ_BIN_BASE)) {
        printf("Gerando %s a partir de PROVAO.TXT...\n", ARQ_BIN_BASE);
        if (textoParaBinario() != 0) return -1;
    }
    if (!existe("crescente.bin") || !existe("decrescente.bin")) {
        printf("Gerando crescente.bin e decrescente.bin (so na primeira execucao)...\n");
        if (gerarCrescenteDecrescente() != 0) return -1;
    }
    return 0;
}
