#include <stdlib.h>
#include "aluno.h"

#define ALLOCATION_EVENT 0
#define DEALLOCATION_EVENT 1

void ordena_eventos(Evento *eventos, int l , int m, int r){
    int leftSize = m - l + 1;
    int rightSize = r - m;
    Evento *leftVector = (Evento*) malloc(sizeof(Evento) * leftSize);
    Evento *rightVector = (Evento*) malloc(sizeof(Evento) * rightSize);
    
    for(int i = 0; i < leftSize; i++){
        leftVector[i] = eventos[i + l];
    }
    for(int j = 0; j < rightSize; j++){
        rightVector[j] = eventos[j + m + 1];
    }
    int k = l;
    int i = 0;
    int j = 0;
    int trocar;
    while(i < leftSize && j < rightSize){
        trocar = 0;
        if(leftVector[i].tempo < rightVector[j].tempo){
            trocar = 1;
        }
        else if(leftVector[i].tempo == rightVector[j].tempo){
            if(leftVector[i].tipo == 1 && rightVector[j].tipo == 0){
                trocar = 1;
            } else if (leftVector[i].tipo == rightVector[j].tipo) {
                trocar = 1; 
            }
        }
        
        if (trocar) {
            eventos[k] = leftVector[i];
            i++;
            k++;
        } else {
            eventos[k] = rightVector[j];
            j++;
            k++;
        }
    }
    
    while (i < leftSize) {
        eventos[k] = leftVector[i];
        i++;
        k++;
    }
    while (j < rightSize) {
        eventos[k] = rightVector[j];
        j++;
        k++;
    }
    free(leftVector);
    free(rightVector);
}

void ordena_eventos_merge(Evento *v, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        ordena_eventos_merge(v, l, m);
        ordena_eventos_merge(v, m + 1, r);
        ordena_eventos(v, l, m, r);
    }
}

long alocar_processo(No **memoria, long tamanho) {//** para que eu possa alterar a variavel memoria original
    No *atual = *memoria;
    No *anterior = NULL;
    long posicaoAlocada;
    
    while (atual != NULL) {
        if (atual->bloco.tam >= tamanho) {
            posicaoAlocada = atual->bloco.inicio;
            atual->bloco.inicio += tamanho;
            atual->bloco.tam -= tamanho;

            if (atual->bloco.tam == 0) {
                if (anterior == NULL) {
                    *memoria = atual->proximo;//se o bloco ficou sem tamanho, atualizamos a cabeca para apontar para o proximo no da fila
                } else {
                    anterior->proximo = atual->proximo;//Se o bloco ocupado estava no meio ou no fim da lista (ou seja, anterior não é nulo), nós fazemos o bloco de trás (anterior) pular o bloco atual e apontar diretamente para o bloco da frente (atual->proximo)
                    //Isso efetivamente "desconecta" o bloco atual da lista.
                }
                free(atual);
            }
            return posicaoAlocada;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return -1;
}
 
void desalocar_processo(No **memoria, long inicio, long tamanho) {
    No *novo = (No*)malloc(sizeof(No));//vai representar o no que acabou de ser desalocado
    novo->bloco.inicio = inicio;
    novo->bloco.tam = tamanho;
    novo->proximo = NULL;

    // Insere o novo bloco ordenado pela posição de "inicio" na memória
    //verifica se a memoria esta vazia ou se o inicio do bloco de memoria e maior que o inicio 
    //do bloco desalocado, se for quer dizer que o bloco desalocado tem que vir na frente
    if (*memoria == NULL || (*memoria)->bloco.inicio > inicio) {
        novo->proximo = *memoria;
        *memoria = novo;
    } else {//o bloco fica no meio ou no fim
        No *atual = *memoria;//no comecando do primeiro no da lista
        //verifica se o no prox existe
        //verifica tmb se o valor do prox no e menor do que o bloco que queremos desalocar
        while (atual->proximo != NULL && atual->proximo->bloco.inicio < inicio) {
            atual = atual->proximo;//se for a gente vai para esse no
        }
        //supondo agora que o valor do prox no seja 60, e o no que eu quero desalocar seja 45
        //nao posso continuar andando pois 60 e maior que 45
        //o que quer dizer que o no que queremos desalocar deve vir atras do no anterior que e menor que o no a ser desalocado
        //o no que vamos desalocar aponta agr para o prox do atual, que e maior que ele
        novo->proximo = atual->proximo;
        //e o atual que verificamos ser menor que o no que vamos desalocar passa a apontar para o novo no
        atual->proximo = novo;
    }
}
/*
void juntar_intervalos(No *memoria) {//Note que aqui você usou apenas um ponteiro simples (No *memoria). Está correto! Como a função sempre funde o nó da frente no nó de trás, o primeiríssimo nó da lista nunca será excluído (ele apenas vai crescer de tamanho). Logo, o endereço inicial da lista nunca muda, dispensando o ponteiro duplo ().
    No *atual = memoria;
    //Controla o loop de varredura. A condição exige que o nó atual exista E que exista um nó logo à frente dele
    while (atual != NULL && atual->proximo != NULL) {
        // Se o fim do bloco atual for exatamente o começo do próximo bloco
        if (atual->bloco.inicio + atual->bloco.tam == atual->proximo->bloco.inicio) {
            //como vamos juntar os blocos precisamos de guardar o endereco do bloco prox
            No *remover = atual->proximo;
            //o bloco atual pega o tamanho do atual->prox e o soma
            atual->bloco.tam += remover->bloco.tam;
            //o bloco atual passa a apontar para o prox do remover, ja que agr eles se juntaram
            atual->proximo = remover->proximo;
            free(remover);
        } else {
            atual = atual->proximo;
        }
    }
}*/

void juntar_intervalos(No *memoria) {
    No *atual = memoria;
    // Percorre a lista juntando os blocos que estao em sequencia
    while (atual != NULL && atual->proximo != NULL) {
         // Se o fim do bloco atual for exatamente o começo do próximo bloco
        if ((atual->bloco.inicio + atual->bloco.tam) == atual->proximo->bloco.inicio) {
            No *aux = atual->proximo;
            //o bloco atual pega o tamanho do atual->prox e o soma
            atual->bloco.tam += aux->bloco.tam;
            //o bloco atual passa a apontar para o prox do remover, ja que agr eles se juntaram
            atual->proximo = aux->proximo; 
            free(aux);
        } else {
            atual = atual->proximo;
        }
    }
}

void liberaMemoria(No *memoria) {
    No *atual = memoria;
    while (atual != NULL) {
        No *aux = atual;
        atual = atual->proximo;
        free(aux);
    }
}

long* first_fit(Processo *processos, int num_processos) {
    long *alocacoes = (long*) malloc(num_processos * sizeof(long));
    Evento *eventos = (Evento*) malloc(num_processos * 2 * sizeof(Evento)); 
    long max_tam = 0;

    for (int i = 0; i < num_processos; i++) {
        eventos[2*i].tempo = processos[i].tempo_aloc;
        eventos[2*i].tipo = ALLOCATION_EVENT;
        eventos[2*i].processo = i;

        eventos[2*i+1].tempo = processos[i].tempo_desaloc;
        eventos[2*i+1].tipo = DEALLOCATION_EVENT;
        eventos[2*i+1].processo = i;

        max_tam += processos[i].tam;
        alocacoes[i] = -1;
    }  
    
    ordena_eventos_merge(eventos, 0, (num_processos * 2) - 1);

    No *memoriaLivre = (No*) malloc(sizeof(No));
    memoriaLivre->bloco.inicio = 0;
    memoriaLivre->bloco.tam = max_tam;
    memoriaLivre->proximo = NULL;

    for (int i = 0; i < num_processos * 2; i++) {
        long pos;
        int processoPosicao = eventos[i].processo;
        if (eventos[i].tipo == ALLOCATION_EVENT) {
            pos = alocar_processo(&memoriaLivre, processos[processoPosicao].tam);
            alocacoes[processoPosicao] = pos;
        } else if (eventos[i].tipo == DEALLOCATION_EVENT) {
            pos = alocacoes[processoPosicao];
            desalocar_processo(&memoriaLivre, pos, processos[processoPosicao].tam);
            juntar_intervalos(memoriaLivre);
            
        }
    } 

    liberaMemoria(memoriaLivre);
    free(eventos);
    
    return alocacoes;
}