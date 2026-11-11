#ifndef ALUNO_H
#define ALUNO_H

#include <stdbool.h>

typedef struct No {
    char dado[100];      
    struct No *esq;      
    struct No *dir;      
} No;

bool CriaExprRec(char* conta, int inicio, int fim, No** ppRaiz);
bool ParentesesInuteis(char* conta, int inicio, int fim);
int CalculaOper(char* conta, int inicio, int fim);
float AvaliarExpr(No* raiz, int* erro);
int stringParaInt(char* str);
void removerEspacos(char* destino, char* origem);
No* CriaNo(char* conteudo, int tam);
No* CriaNoOperador(char operador);
void destroiExpr(No** raiz);

#endif