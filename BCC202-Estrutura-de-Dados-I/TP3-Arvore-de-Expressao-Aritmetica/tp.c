#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "aluno.h"
#define T 100

int main() {
    int n;
    int inicio, fim, erro;
    int resultado;
    char expressao[T];
    char expressaoComEspaco[T];
    scanf("%d", &n);
    getchar();
    char** respostas = malloc(sizeof(char*) * n);

    for (int i = 0; i < n; i++) {
        respostas[i] = malloc(T * sizeof(char));
    }

    for(int i = 0; i < n; i++){
        erro = 0;
        fgets(expressaoComEspaco, T, stdin);
        removerEspacos(expressao,expressaoComEspaco);

        if (expressao[0] == '-') {
           char temp[T + 1]; // Buffer temporário grande o suficiente
           sprintf(temp, "0%s", expressao); // Adiciona o 0 na frente
           strcpy(expressao, temp);        // Copia de volta para a string principal
        }

        No* raiz = NULL;
        inicio = 0;
        fim = strlen(expressao);
        CriaExprRec(expressao, inicio, fim, &raiz);
        resultado = AvaliarExpr(raiz, &erro);
        if(erro == 1){
           strcpy(respostas[i], "Erro: divisao por zero.");
        }
        else{
            sprintf(respostas[i], "%d", resultado);
        }
       destroiExpr(&raiz);
    }
    for(int i = 0; i < n; i++){
        printf("%s\n", respostas[i]);
    }

    for(int i = 0; i < n; i++){
        free(respostas[i]);
    }
    free(respostas);

    return 0;
}