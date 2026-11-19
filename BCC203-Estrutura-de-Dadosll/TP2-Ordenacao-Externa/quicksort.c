
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "quicksort.h"
 
long long num_leituras = 0;
long long num_escritas = 0;
long long num_comparacoes = 0;
 
 
void FAVazia(TipoArea *Area) {
    Area->numElementos = 0;
}
 
int ObterNumCelOcupadas(TipoArea Area) {
    return Area.numElementos;
}
//procura o lugar certo para fazer a insercao
void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea) {
    int i = Area->numElementos - 1;
    while (i >= 0) {
        num_comparacoes++;
        //desaloca os valores maiores que a nota pro final
        if (Area->itens[i].nota > UltLido->nota) {
            Area->itens[i + 1] = Area->itens[i];
            i--;
        } else {
            break;
        }
    }
    //insere a nota no lugar certo
    Area->itens[i + 1] = *UltLido;
    Area->numElementos++;
    *NRArea = Area->numElementos;
}
//retira o menor valor da area dos pivos
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    if (Area->numElementos == 0) return;
    *R = Area->itens[0];
    for (int i = 0; i < Area->numElementos - 1; i++) {
        Area->itens[i] = Area->itens[i + 1];
    }
    Area->numElementos--;
    *NRArea = Area->numElementos;
}
//retira o maior valor da area dos pivos
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea) {
    if (Area->numElementos == 0) return;
    *R = Area->itens[Area->numElementos - 1];
    Area->numElementos--;
    *NRArea = Area->numElementos;
}
 
//le os valores a partir da direita do arquivo
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer) {
    //o ponteiro tem que ser reposicionado para a posicao certa a ser lida
    //ja que apos cada leitura o ponteira "pula" pra frente e a leitura caminha para tras
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    num_leituras++;
    (*Ls)--;
    *OndeLer = FALSE;//variavel de controle para que a proxima leitura seja feita plea esquerda
}
//le os valores a partir da esquerda do arquivo
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer) {
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    num_leituras++;
    (*Li)++;
    *OndeLer = TRUE;//variavel de controle para que a proxima leitura seja feita pela direita
}
//escreve o valor na subparticao da direita
//elementos maiores que o pivo
void EscreveMax(FILE **ArqLEs, TipoRegistro R, int *Es) {
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    num_escritas++;
    (*Es)--;
}
//escreve o valor na subparticao da esquerda
//elementos menores que o pivo
void EscreveMin(FILE **ArqEi, TipoRegistro R, int *Ei) {
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    num_escritas++;
    (*Ei)++;
}
 
 
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea *Area, int Esq, int Dir, int *i, int *j) {
    //Ls e Es comecam no fim do arquivo(direita) e caminham para a esquerda
    //Li e Ei comecam do inicio (esquerda) e caminham para a direita
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq;
    int NRArea = 0;//quantidade de registros na area interna
    float Linf = -FLT_MAX, Lsup = FLT_MAX;//limites inferiores e superiores
    short OndeLer = TRUE;//variavel de controle
    TipoRegistro UltLido, R;
    //os 3 FILE* apontam para o mesmo arquivo, cada um com seu buffer: descarrega/descarta
    //os buffers para nao ler dado desatualizado de uma particao anterior
    fflush(*ArqLi); fflush(*ArqEi); fflush(*ArqLEs);
    //posiciona os ponteiros de leitura e escrita no inicio do subarquivo
    fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(TipoRegistro), SEEK_SET);
 
    *i = Esq - 1;
    *j = Dir + 1;
    //continua enquanto ainda houver registros para serrem lidos entre Li e Ls
    while (Ls >= Li) {
        //preenche a area de pivos ate atingir tamanho - 1
        if (NRArea < TAMAREA - 1) {
            if (OndeLer) LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            else LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            InserirArea(Area, &UltLido, &NRArea);
            continue;
        }
        //prioriza a leitura do lado onde o ponteiro de escrita alcancou o de leitura
        //isso evita perder dados
        //caso contrario, segue a alternancia normal
        if (Ls == Es) LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else if (Li == Ei) LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        else if (OndeLer) LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        else LeInf(ArqLi, &UltLido, &Li, &OndeLer);
 
        num_comparacoes++;
        //se for maior que o limite superior da area de pivos
        //vai direto para o subparticao da direita
        if (UltLido.nota > Lsup) {
            *j = Es;//atualiza o inicio da subparticao direita
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }
 
        num_comparacoes++;
        //se for menor que o limite inferior da area de pivos
        //vai direto para a subparticao da esquerda
        if (UltLido.nota < Linf) {
            *i = Ei;//atualiza o fim subparticao da esquerda
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }
        //se o registro ficou dentro do intervalo
        //o registro e inserido na area dos pivos
        InserirArea(Area, &UltLido, &NRArea);
      
        //a retirada da area de pivos e feita para deixar balanceada as subparticoes
        //se a subparticao esquerda(Ei - Esq) for menor que a direita (Dir - Es)
        //retira o menor, do contrario retira o maior
        if (Ei - Esq < Dir - Es) {
            RetiraMin(Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
        } else {
            RetiraMax(Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
        }
    }
    //os elementos que sobram na area apos os registros do arquivo ja terem sido lidos
    //sao gravados no espaco livre central, ficando na suas posicoes definitivas
    while (Ei <= Es) {
        RetiraMin(Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
    fflush(*ArqLi); fflush(*ArqEi); fflush(*ArqLEs);
}
 
void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir) {
    int i, j;
    TipoArea Area;
    //caso base
    if (Dir - Esq < 1) return;
 
    FAVazia(&Area);
    //inicializa a area de pivos na memoria
    Particao(ArqLi, ArqEi, ArqLEs, &Area, Esq, Dir, &i, &j);
 
    if (i - Esq < Dir - j) {
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    } else {
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }
}
 
 
/* copia os n primeiros registros de arqBinOrig (binario.bin = PROVAO.TXT na ordem original)
   para o arquivo que sera ordenado e, conforme a situacao, deixa essa AMOSTRA:
     1 = ordenada ascendentemente, 2 = ordenada descendentemente, 3 = ordem original.
   Retorna 0 em caso de sucesso e -1 em caso de erro. */
static int copiarPrimeiros(const char *origem, const char *destino, int n) {
    FILE *fIn = fopen(origem, "rb");
    if (!fIn) { fprintf(stderr, "Erro: nao foi possivel abrir '%s'.\n", origem); return -1; }
    FILE *fOut = fopen(destino, "wb");
    if (!fOut) { fprintf(stderr, "Erro: nao foi possivel criar '%s'.\n", destino); fclose(fIn); return -1; }
    static TipoRegistro lote[1024];
    int restantes = n;
    while (restantes > 0) {
        int pedir = restantes < 1024 ? restantes : 1024;
        int lidos = (int)fread(lote, sizeof(TipoRegistro), pedir, fIn);
        if (lidos <= 0) break;
        if ((int)fwrite(lote, sizeof(TipoRegistro), lidos, fOut) != lidos) break;
        restantes -= lidos;
    }
    fclose(fIn); fclose(fOut);
    if (restantes > 0) { fprintf(stderr, "Erro: '%s' tem menos de %d registros.\n", origem, n); return -1; }
    return 0;
}
 
static int ordenarArquivoCrescente(const char *nome, int n) {
    FILE *a = fopen(nome, "r+b"), *b = fopen(nome, "r+b"), *c = fopen(nome, "r+b");
    if (!a || !b || !c) {
        if (a) fclose(a); if (b) fclose(b); if (c) fclose(c);
        return -1;
    }
    QuicksortExterno(&a, &b, &c, 1, n);
    fclose(a); fclose(b); fclose(c);
    return 0;
}
 
int prepararSituacao(const char *arqBinOrig, const char *arqBinTeste, int n, int situacao) {
    if (copiarPrimeiros(arqBinOrig, arqBinTeste, n) != 0) return -1;
    if (situacao == 3) return 0;
 
    if (ordenarArquivoCrescente(arqBinTeste, n) != 0) return -1;   /* amostra em ordem crescente */
    if (situacao == 1) return 0;
 
    /* situacao 2: inverte o arquivo ja ordenado */
    const char *tmp = "provaoTmp.dat";
    FILE *in = fopen(arqBinTeste, "rb"), *out = fopen(tmp, "wb");
    if (!in || !out) { if (in) fclose(in); if (out) fclose(out); return -1; }
    TipoRegistro r;
    for (int i = n - 1; i >= 0; i--) {
        fseek(in, (long)i * sizeof(TipoRegistro), SEEK_SET);
        if (fread(&r, sizeof(TipoRegistro), 1, in) != 1) break;
        fwrite(&r, sizeof(TipoRegistro), 1, out);
    }
    fclose(in); fclose(out);
    remove(arqBinTeste);
    return rename(tmp, arqBinTeste);
}
 
void imprimirArquivoBinario(const char *nomeArqBin, int n) {
    FILE *f = fopen(nomeArqBin, "rb");
    if (!f) return;
    TipoRegistro r;
    int i = 0;
    printf("\n--- Conteudo do Arquivo (%d registros) ---\n", n);
    while (i < n && fread(&r, sizeof(TipoRegistro), 1, f)) {
        printf("Inscricao: %8ld | Nota: %5.1f | UF: %s | Cidade: %-30s | Curso: %s\n",
               r.inscricao, r.nota, r.estado, r.cidade, r.curso);
        i++;
    }
    printf("---------------------------------------------\n\n");
    fclose(f);
}
 
 