#include <stdio.h>
#include <stdlib.h>
#include "aluno.h"

int main() {
    polinomio* p;
    int n, e;
    float a, x, y;

    // leitura do numero de termos
    if (scanf("%d", &n) != 1) return 0;
    
    p = criarPolinomio(n);

    // leitura dos coeficientes e expoentes
    for(int i = 0; i < n; i++){
        scanf("%f %d", &a, &e);
        lerPolinomio(p, i, a, e);
    }

    // leitura do intervalo [x, y]
    scanf("%f %f", &x, &y);

    // verificação de expoente negativo
    if(verificaDerivada(p)){
        destroiPolinomio(p);
        return 0;
    }

    if(verificaPolinomio(p) == 0){
        printf("Polinomio: ");
        imprimirPolinomio(p);

        polinomio* d = derivarPolinomio(p);
        printf("Derivada: ");
        imprimirDerivada(d);
        processarEImprimir(d, x , y);
        

        destroiPolinomio(d); // libera a memoria da derivada
    }
    else {
        printf("Erro: expoente negativo.\n");
    }

    destroiPolinomio(p); // libera a memoria do polinomio original
    return 0;
}