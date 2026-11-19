#define _POSIX_C_SOURCE 199309L
#include "int2f.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//tempo de relogio em segundos: termino menos inicio da execucao, como pede o enunciado
double tempoAtual(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

//compara dois registros priorizando o status de congelamento e depois a nota
//so conta comparacao quando as notas de fato sao comparadas
int compara_registros(Registro a, Registro b, Metricas *m) {
    if (a.congelado != b.congelado) {
        return a.congelado - b.congelado;
    }
    m->comparacoes++;
    if (a.nota > b.nota) return 1;
    if (a.nota < b.nota) return -1;
    return 0;
}

//refaz o min-heap descendo o elemento raiz para a posicao correta
void refaz_heap(Registro *heap, int esq, int dir, Metricas *m) {
    int i = esq;
    int j = i * 2 + 1;
    Registro aux = heap[i];

    while (j <= dir) {
        if (j < dir && compara_registros(heap[j], heap[j + 1], m) > 0) {
            j++;
        }
        if (compara_registros(aux, heap[j], m) <= 0) {
            break;
        }
        heap[i] = heap[j];
        i = j;
        j = i * 2 + 1;
    }
    heap[i] = aux;
}

//constroi o heap inicial de forma bottom-up
void constroi_heap(Registro *heap, int n, Metricas *m) {
    int esq = (n / 2) - 1;
    while (esq >= 0) {
        refaz_heap(heap, esq, n - 1, m);
        esq--;
    }
}

//leitura no arquivo binario contando a transferencia
int ler_registro_binario(FILE *arq, Registro *reg, Metricas *m) {
    //fread retorna 1 se conseguiu ler a struct
    if (fread(reg, sizeof(Registro), 1, arq) == 1) {
        m->transferencias_leitura++;
        return 1;
    }
    m->transferencias_leitura++;
    return 0;
}

//gera os blocos ordenados e retorna a quantidade de blocos criados
int geracao_blocos_substituicao(FILE *arq_entrada, FILE **fitas_saida, int num_fitas_saida, Metricas *m) {
    Registro heap[TAM_MEMORIA];
    int tam_heap = 0;
    int fita_atual = 0;
    int congelados = 0;
    int blocos = 0;

    //preenche a memoria com os primeiros registros
    while (tam_heap < TAM_MEMORIA && ler_registro_binario(arq_entrada, &heap[tam_heap], m)) {
        heap[tam_heap].congelado = 0;
        tam_heap++;
    }

    if (tam_heap > 0) {
        blocos = 1;
    }

    constroi_heap(heap, tam_heap, m);

    while (tam_heap > 0) {
        Registro menor = heap[0];

        //grava em binario para facilitar a leitura na intercalacao
        fwrite(&menor, sizeof(Registro), 1, fitas_saida[fita_atual]);
        m->transferencias_escrita++;

        Registro novo;
        if (ler_registro_binario(arq_entrada, &novo, m)) {
            //se a nota nova e menor que a gravada congela o registro
            m->comparacoes++;
            if (novo.nota < menor.nota) {
                novo.congelado = 1;
                congelados++;
            } else {
                novo.congelado = 0;
            }
            heap[0] = novo;
        } else {
            //arquivo original acabou substitui raiz pelo ultimo do heap
            heap[0] = heap[tam_heap - 1];
            tam_heap--;
        }

        if (tam_heap > 0) {
            refaz_heap(heap, 0, tam_heap - 1, m);
        }

        //se todos estao congelados o bloco acabou
        if (congelados == tam_heap && tam_heap > 0) {
            fita_atual = (fita_atual + 1) % num_fitas_saida;
            congelados = 0;
            blocos++;

            //descongela todos do heap
            for (int i = 0; i < tam_heap; i++) {
                heap[i].congelado = 0;
            }
            constroi_heap(heap, tam_heap, m);
        }
    }
    return blocos;
}

//transforma o binario final em txt e cuida do parametro p para impressao na tela
//retorna 0 em caso de sucesso e -1 em caso de erro
int finaliza_saida(const char *bin_final, const char *txt_saida, int imprimir_tela) {
    FILE *bin = fopen(bin_final, "rb");
    if (!bin) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s' para leitura.\n", bin_final);
        return -1;
    }
    FILE *txt = fopen(txt_saida, "w");
    if (!txt) {
        fprintf(stderr, "Erro: nao foi possivel criar '%s'.\n", txt_saida);
        fclose(bin);
        return -1;
    }
    Registro reg;

    //le registro por registro do arquivo binario ja ordenado
    while (fread(&reg, sizeof(Registro), 1, bin) == 1) {
        //formata a saida imitando o provao original para salvar no txt
        fprintf(txt, "%08ld %5.1f %-2s %-50s %-30s\n",
                reg.inscricao, reg.nota, reg.estado, reg.cidade, reg.curso);

        //se a flag p foi ativada na linha de comando joga no console
        if (imprimir_tela) {
            printf("%08ld %5.1f %-2s %-50s %-30s\n",
                   reg.inscricao, reg.nota, reg.estado, reg.cidade, reg.curso);
        }
    }

    fclose(bin);
    fclose(txt);
    return 0;
}

//compara os nos baseados no status de congelado e depois na nota
//so conta comparacao quando as notas de fato sao comparadas
int compara_no_intercalacao(NoIntercalacao a, NoIntercalacao b, Metricas *m) {
    if (a.reg.congelado != b.reg.congelado) {
        return a.reg.congelado - b.reg.congelado;
    }
    m->comparacoes++;
    if (a.reg.nota > b.reg.nota) return 1;
    if (a.reg.nota < b.reg.nota) return -1;
    return 0;
}

//refaz o heap descendo a raiz para a intercalacao
void refaz_heap_intercalacao(NoIntercalacao *heap, int esq, int dir, Metricas *m) {
    int i = esq;
    int j = i * 2 + 1;
    NoIntercalacao aux = heap[i];

    while (j <= dir) {
        if (j < dir && compara_no_intercalacao(heap[j], heap[j + 1], m) > 0) {
            j++;
        }
        if (compara_no_intercalacao(aux, heap[j], m) <= 0) {
            break;
        }
        heap[i] = heap[j];
        i = j;
        j = i * 2 + 1;
    }
    heap[i] = aux;
}

//constroi o heap inicial da intercalacao
void constroi_heap_intercalacao(NoIntercalacao *heap, int n, Metricas *m) {
    int esq = (n / 2) - 1;
    while (esq >= 0) {
        refaz_heap_intercalacao(heap, esq, n - 1, m);
        esq--;
    }
}

//laco base da intercalacao retornando a quantidade de blocos resultantes
int intercalacao_2f(FILE **fitas_entrada, FILE **fitas_saida, int num_fitas, Metricas *m) {
    NoIntercalacao heap[NUM_FITAS_LADO];
    int tam_heap = 0;
    int congelados = 0;
    int fita_atual_saida = 0;
    int blocos = 0;

    //le o primeiro registro de cada uma das fitas de entrada
    for (int i = 0; i < num_fitas; i++) {
        Registro reg;
        if (ler_registro_binario(fitas_entrada[i], &reg, m)) {
            reg.congelado = 0;
            heap[tam_heap].reg = reg;
            heap[tam_heap].fita_origem = i;
            tam_heap++;
        }
    }

    if (tam_heap > 0) {
        blocos = 1;
    }

    //constroi o heap inicial de ate 10 posicoes
    constroi_heap_intercalacao(heap, tam_heap, m);

    //processo de mesclagem enquanto houver dados no heap
    while (tam_heap > 0) {
        //pega o menor elemento e grava na fita de saida
        NoIntercalacao menor = heap[0];
        fwrite(&menor.reg, sizeof(Registro), 1, fitas_saida[fita_atual_saida]);
        m->transferencias_escrita++;

        //tenta ler o proximo registro da mesma fita de onde o menor veio
        Registro novo_reg;
        if (ler_registro_binario(fitas_entrada[menor.fita_origem], &novo_reg, m)) {
            //se a nota for menor que a gravada acabou o bloco dessa fita
            m->comparacoes++;
            if (novo_reg.nota < menor.reg.nota) {
                novo_reg.congelado = 1;
                congelados++;
            } else {
                novo_reg.congelado = 0;
            }

            heap[0].reg = novo_reg;
            heap[0].fita_origem = menor.fita_origem;
        } else {
            //fita esgotou pega o ultimo do heap e joga pra raiz
            heap[0] = heap[tam_heap - 1];
            tam_heap--;
        }

        if (tam_heap > 0) {
            refaz_heap_intercalacao(heap, 0, tam_heap - 1, m);
        }

        //se todos os caminhos congelaram o bloco atual de intercalacao acabou
        if (congelados == tam_heap && tam_heap > 0) {
            //muda a fita de saida para o proximo bloco
            fita_atual_saida = (fita_atual_saida + 1) % num_fitas;
            congelados = 0;
            blocos++;

            //descongela todos do heap para iniciar o novo bloco
            for (int i = 0; i < tam_heap; i++) {
                heap[i].reg.congelado = 0;
            }
            constroi_heap_intercalacao(heap, tam_heap, m);
        }
    }
    return blocos;
}

//fecha e remove as fitas temporarias do hd (ignora as que nao foram abertas)
static void fecha_e_remove_fitas(FILE **fitas, int n) {
    char nome_fita[32];
    for (int i = 0; i < n; i++) {
        sprintf(nome_fita, "fita_%d.bin", i);
        if (fitas[i]) fclose(fitas[i]);
        fitas[i] = NULL;
        remove(nome_fita);
    }
}

//mostra na tela os registros de entrada (usado com a opcao -P), sem contar nas metricas
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

//funcao principal que gerencia as 20 fitas do metodo 1
//retorna 0 em caso de sucesso e -1 em caso de erro
int executa_metodo_1(const char *nome_arquivo_entrada, int imprimir_tela, Metricas *m) {
    //abre o arquivo binario base com a quantidade de alunos ja cortada
    FILE *arq_entrada = fopen(nome_arquivo_entrada, "rb");
    if (!arq_entrada) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo de entrada '%s'.\n", nome_arquivo_entrada);
        return -1;
    }

    //com -P mostra os dados que serao ordenados
    if (imprimir_tela) {
        imprime_entrada(arq_entrada);
    }

    //o tempo comeca a contar depois da exibicao opcional da entrada
    double inicio = tempoAtual();

    FILE *fitas[NUM_FITAS_TOTAL];
    char nome_fita[32];

    //cria as 20 fitas temporarias em modo de leitura e escrita binaria
    for (int i = 0; i < NUM_FITAS_TOTAL; i++) {
        fitas[i] = NULL;
    }
    for (int i = 0; i < NUM_FITAS_TOTAL; i++) {
        sprintf(nome_fita, "fita_%d.bin", i);
        fitas[i] = fopen(nome_fita, "wb+");
        if (!fitas[i]) {
            fprintf(stderr, "Erro: nao foi possivel criar a fita temporaria '%s'.\n", nome_fita);
            fecha_e_remove_fitas(fitas, NUM_FITAS_TOTAL);
            fclose(arq_entrada);
            return -1;
        }
    }

    //ponteiros para facilitar a inversao de entrada e saida
    FILE **fitas_entrada = &fitas[0];
    FILE **fitas_saida = &fitas[NUM_FITAS_LADO];

    //fase 1 geracao dos blocos iniciais e contagem de quantos foram gerados
    int blocos_ativos = geracao_blocos_substituicao(arq_entrada, fitas_saida, NUM_FITAS_LADO, m);
    fclose(arq_entrada);

    //rebobina as fitas de saida para que possam ser lidas na intercalacao
    for (int i = 0; i < NUM_FITAS_LADO; i++) {
        rewind(fitas_saida[i]);
    }

    //fase 2 intercalacao alternando as metades ate sobrar apenas 1 bloco
    while (blocos_ativos > 1) {
        //a saida da fase anterior vira a entrada da fase atual
        FILE **temp = fitas_entrada;
        fitas_entrada = fitas_saida;
        fitas_saida = temp;

        //limpa as novas fitas de saida para sobrescrever pelo nome real
        for (int i = 0; i < NUM_FITAS_LADO; i++) {
            int indice_real = (fitas_saida == &fitas[0]) ? i : i + NUM_FITAS_LADO;
            sprintf(nome_fita, "fita_%d.bin", indice_real);
            FILE *reaberta = freopen(nome_fita, "wb+", fitas_saida[i]);
            if (!reaberta) {
                //o freopen ja fechou a fita original
                fitas_saida[i] = NULL;
                fprintf(stderr, "Erro: nao foi possivel reabrir a fita temporaria '%s'.\n", nome_fita);
                fecha_e_remove_fitas(fitas, NUM_FITAS_TOTAL);
                return -1;
            }
            fitas_saida[i] = reaberta;
        }

        //intercala e atualiza a quantidade de blocos ativos
        blocos_ativos = intercalacao_2f(fitas_entrada, fitas_saida, NUM_FITAS_LADO, m);

        //rebobina as fitas de saida recem gravadas para a proxima iteracao
        for (int i = 0; i < NUM_FITAS_LADO; i++) {
            rewind(fitas_saida[i]);
        }
    }

    //fim da ordenacao: tempo medido sem contar a gravacao do txt de resultado
    m->tempo_execucao = tempoAtual() - inicio;

    //descobre onde o arquivo final ordenado parou para gerar o txt
    int indice_final = (fitas_saida == &fitas[0]) ? 0 : NUM_FITAS_LADO;
    sprintf(nome_fita, "fita_%d.bin", indice_final);
    int status = finaliza_saida(nome_fita, "resultado_metodo1.txt", imprimir_tela);

    //fecha e remove as 20 fitas temporarias do hd
    fecha_e_remove_fitas(fitas, NUM_FITAS_TOTAL);
    return status;
}