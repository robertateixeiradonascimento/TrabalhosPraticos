/*Roberta Teixeira e Luiza Oliveira*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "aluno.h" 


void removerEspacos(char* destino, char* origem) {
    int i = 0, j = 0;
    while (origem[i] != '\0') {
        if (origem[i] != ' ' && origem[i] != '\n' && origem[i] != '\r') {
            destino[j++] = origem[i];
        }
        i++;
    }
    destino[j] = '\0';
}

bool ParentesesInuteis(char* conta, int inicio, int fim) {
    if (conta[inicio] != '(' || conta[fim - 1] != ')')
     return false;


    int parenteses = 0;
 
    for (int i = inicio; i < fim - 1; i++) {
        if (conta[i] == '(')
         parenteses++;
        else if (conta[i] == ')') 
        parenteses--;
        if (parenteses == 0)
         return false;
    }
    return true;
}

No* CriaNo(char* conteudo, int tam) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo != NULL) {
        strncpy(novo->dado, conteudo, tam);
        novo->dado[tam] = '\0'; 
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

No* CriaNoOperador(char operador) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo != NULL) {
        novo->dado[0] = operador;
        novo->dado[1] = '\0';
        novo->esq = NULL;
        novo->dir = NULL;
    }
    return novo;
}

int CalculaOper(char* conta, int inicio, int fim) {
    int pos = -1;
    int parenteses = 0;
    for(int i = inicio; i < fim; i++){
        if (conta[i] == '(') 
        parenteses++;
        else if (conta[i] == ')') 
        parenteses--;
        else if (parenteses == 0 && (conta[i] == '+' || conta[i] == '-')){
            pos = i;
      }
      
    }
    if (pos != -1)
     return pos;
    parenteses = 0;
    for(int i = inicio; i < fim; i++){
        if (conta[i] == '(')
         parenteses++;
        else if (conta[i] == ')') 
        parenteses--; 
        else if (parenteses == 0 && (conta[i] == '/' || conta[i] == '*'))
         pos = i;
    }
    return pos;
}

bool CriaExprRec(char* conta, int inicio, int fim, No** ppRaiz) {
    while (ParentesesInuteis(conta, inicio, fim)) {
        inicio++;
        fim--;
    }  
    
    int posicaoOperador = CalculaOper(conta, inicio, fim);

    if (posicaoOperador == -1) {
        *ppRaiz = CriaNo(&conta[inicio], fim - inicio);
        return (*ppRaiz != NULL);
    }
    
    //caso contrario cria o no operador
    *ppRaiz = CriaNoOperador(conta[posicaoOperador]);
    if (*ppRaiz == NULL){
     return false;
    } 

    //recebe os valores antes do operador
    bool esq = CriaExprRec(conta, inicio, posicaoOperador, &((*ppRaiz)->esq));
    //recebe os valores depois do operador
    bool dir = CriaExprRec(conta, posicaoOperador + 1, fim, &((*ppRaiz)->dir));

    return (esq && dir);
}


int stringParaInt(char* str) {
    int valor = 0; 
    int sinal = 1; 
    int i = 0;

  
    if (str[0] == '-') {
        sinal = -1;
        i = 1;
    }

    for (; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            valor = (valor * 10) + (str[i] - '0');
        }
    }
    return valor * sinal;
}

float AvaliarExpr(No* raiz, int* erro) {
    if (raiz == NULL) 
    return 0;

    
    if (raiz->esq == NULL && raiz->dir == NULL) {
        return stringParaInt(raiz->dado); 
    }

    float valorEsq = AvaliarExpr(raiz->esq, erro);
    float valorDir = AvaliarExpr(raiz->dir, erro);

    //avalia o operador para fazer as contas
    switch (raiz->dado[0]) {
        case '+': 
         return valorEsq + valorDir;
        case '-': 
         return valorEsq - valorDir;
        case '*':
         return valorEsq * valorDir;
        case '/': 
            if (valorDir == 0) {
                //se for dividir por zero, erro recebe 1 para poder escrever que deu erro na hora de printar
                *erro = 1;
                return 0;
            }
            else{
                return valorEsq / valorDir;
            }
        default: 
            return 0;
    }
}
 
void destroiExpr(No** raiz) {
    if (raiz == NULL || *raiz == NULL){
        return;
    }

    destroiExpr(&((*raiz)->esq));
    destroiExpr(&((*raiz)->dir));
    free(*raiz);
    *raiz = NULL;
}





































