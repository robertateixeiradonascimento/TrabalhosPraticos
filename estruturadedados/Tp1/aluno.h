#ifndef ALUNO_H
#define ALUNO_H


typedef struct p polinomio;
typedef struct i intervalo;
polinomio* criarPolinomio(int n);
void destroiPolinomio(polinomio* p);
intervalo* criarIntervalo(int n);
void destroiIntervalo(intervalo* i);
void lerPolinomio(polinomio* p, int i, float a, int e);
int verificaPolinomio(polinomio* p);
void imprimirPolinomio(polinomio* p);
polinomio* derivarPolinomio(polinomio* p);
void imprimirDerivada(polinomio* d);
double avaliarPolinomio(polinomio* p, double x, int i);
int obterSinal(float r);
void analisarPolinomio(polinomio* d, float x, float y, intervalo* i);
void processarEImprimir(polinomio* d, float inicio, float fim);
void imprimirIntervalos(float* arr, int n, char* nome, int ehTransicao);
int verificaDerivada(polinomio* p);


#endif