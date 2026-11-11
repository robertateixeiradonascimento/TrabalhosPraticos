/*Roberta Teixeira Do Nascimento e Luiza Oliveira*/



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "aluno.h"

 struct p{
    float *a;//coeficiente
    int *e;//expoente
    int n;
};//essa e a struct que vai guardar as informacoes do polinomio

 struct i{
    float *crescimento;
    float *decrescimento;
    float *transicao;
    int quantidadeC;
    int quantidadeD;
    int quantidadeT;
 };


//aloca dinamicamente espaco para o que vai ser digitado, por isso precisa saber quantos valores vao ser
//digitados, se forem 2 eles coloca n para o int n da struct, e aloca 2 espacos de float para a e dois espacos de int para e
polinomio* criarPolinomio(int n){
    polinomio* p = (polinomio*) malloc(sizeof(polinomio));
    if(p != NULL){
    p->n = n;
    p->a = (float*) malloc(n * sizeof(float));
    p->e = (int*) malloc(n * sizeof(int));
    }
    return p;
}
//libera a struct
void destroiPolinomio(polinomio* p) {
    if (p != NULL) {
        free(p->a); 
        free(p->e); 
        free(p);    
    }
}

intervalo* criarIntervalo(int n) {
    intervalo* i = (intervalo*) malloc(sizeof(intervalo));
    if (i != NULL) {
        i->crescimento = (float*) malloc(n * sizeof(float));
        i->decrescimento = (float*) malloc(n * sizeof(float));
        i->transicao = (float*) malloc(n * sizeof(float));
        i->quantidadeC = 0;
        i->quantidadeD = 0;
        i->quantidadeT = 0;
    }
    return i;
}
void destroiIntervalo(intervalo* i){
    if(i != NULL){
       free(i->crescimento); 
       free(i->decrescimento);
       free(i->transicao);
       free(i); 
    }
}


//coloca nos coef e exp do polinomio seus valores
//exemplo se eu digitar 1 e 2 o 1 vai para o A da posicao i e o 2 vai para o E da struct
void lerPolinomio(polinomio* p, int i, float a, int e){
    p->a[i] = a;
    p->e[i] = e;
}
//nao se pode digitar um expoente negativo, entao caso o expoente for negativo ja nem faz o resto
int verificaPolinomio(polinomio* p){
    for(int i = 0; i < p->n; i++){
        if(p->e[i] < 0){
            return 1;
        }
    }
    return 0;
}

int verificaDerivada(polinomio* p){
    float soma = 0.0;
    int expoente0 = 0;
    for(int i = 0; i < p->n; i++){
        if(p->e[i] == 0){
            expoente0 ++;
            soma += p->a[i];
        }
        
    }
    if(expoente0 == p->n){
        printf("Funcao constante em y = %.2f.\n" , soma);
        return 1;
    }  
    else{
        return 0;
    }
}



//imprime o polinomio ja conferindo se o expoente e igual a 0
//a forma como o polinomio vai ser escrito depende do valor do seu expoente 
void imprimirPolinomio(polinomio* p){
    for(int i = 0; i < p->n; i++){

        if (p->a[i] == 0){
        continue;
        }
        if (i > 0 && p->a[i] > 0) {
            printf("+");
        }
        if (p->e[i] == 0) {
            printf("%.2f", p->a[i]); //todo numero elevado a 0 e igual a 1 entao o x nao precisa aparecer
        } else if (p->e[i] == 1) {
            printf("%.2fx", p->a[i]);//todo numero elevado a 1 ja e ele mesmo entao aparece so o x
        } else {
            printf("%.2fx^%d", p->a[i], p->e[i]);// aqui sao os demais casos
        }
    }
    printf("\n");
}
//cria um polinomio para guardar a derivada e ja se coloca os valores de coef e exp nela
polinomio* derivarPolinomio(polinomio* p) {
    polinomio* derivada = criarPolinomio(p->n);//e criado outra struct que vai alocar a derivada
    
    for(int i = 0; i < p->n; i++) {
        derivada->a[i] = p->a[i] * p->e[i];//usando a regra de derivada ja passa o expoente para frente multiplicando-o com o numero
        
        if (p->e[i] > 0) {
            derivada->e[i] = p->e[i] - 1;// seguindo tmb a regra da derivada se subtrai 1 do expoente se esse for diferente de 0
        } else {// se o expoente for 0 a derivada da 0
            derivada->e[i] = 0;
            derivada->a[i] = 0;//a tmb vira 0 pois derivada de uma constante e 0
        }
    }
    return derivada;
}
//so precisa mostrar o que imprimir, os calculos ja foram feitos no de cima
//ao a dicionar os valores a derivada->e[i] por exemplo eu ja disse o que tem que ser escrito
void imprimirDerivada(polinomio* d) {
    for(int i = 0; i < d->n; i++) {
        if(d->a[i] == 0) 
         continue;

        if(i > 0 && d->a[i] > 0)
         printf("+");//isso e pra colocar o sinal de mais caso o numero seja positivo e nao seja a 1 posicao pq ai nao precisa

        if(d->e[i] == 0)// expoente de zero nesse caso nao precisa escrever o x pq elevado a 0 ficaria 1
         printf("%.2f", d->a[i]);
        else if(d->e[i] == 1)
         printf("%.2fx", d->a[i]);//o expoente deu 1 nesse caso so se escreve o x pq elevado a 1 da ele mesmo
        else
        printf("%.2fx^%d", d->a[i], d->e[i]);//demais casos
    }
    printf("\n");
}

double avaliarPolinomio(polinomio* p, double x, int i) {
    //caso base: se i chegou ao final do vetor, retorna 0
    if (i >= p->n) {
        return 0;
    }
    //passo recursivo: termo atual + resultado da proxima chamada
    return (p->a[i] * pow(x, p->e[i])) + avaliarPolinomio(p, x, i + 1);
}

int obterSinal(float r) {
    if (fabs(r) < 0.0001) {
        return 0;  
    } else if (r > 0) {
        return 1; 
    } else {
        return -1;  
    }
}



 void analisarPolinomio(polinomio* d, float x, float y, intervalo* i) {
    //caso base, se for maior que y e porque acabou o intervalo
    if (x >= y - 0.0001f) 
    return;
    
    float passo = 0.1f;
    //se o proximo passo for ultrapassar o limite y, o passo vira a sobra
    //para poder lidar com o intervalo quebrado
    if (x + passo > y) {
        passo = y - x;
    }

    float r1 = avaliarPolinomio(d, x, 0);
    float r2 = avaliarPolinomio(d, x + passo, 0);

    int s1 = obterSinal(r1);
    int s2 = obterSinal(r2);

    //se s2 e igual a zero quer dizer que o proximo passo e um ponto critico
    //pq s2 e o ponto atual + passo
    if (s2 == 0) {//o realloc e quantidade + 2 pois eu guardo o intervalo
        i->transicao = (float*)realloc(i->transicao, (i->quantidadeT + 2) * sizeof(float));
        i->transicao[i->quantidadeT++] = x + passo;
        i->transicao[i->quantidadeT++] = x + passo;
        //se houver mudanca de sinal quer dizer que dentro desse intervalo tinha uma raiz que zeraria a derivada
        //novamente guardo o intervalo
    } else if (s1 != 0 && s1 != s2) {
        i->transicao = (float*)realloc(i->transicao, (i->quantidadeT + 2) * sizeof(float));
        i->transicao[i->quantidadeT++] = x;
        i->transicao[i->quantidadeT++] = x + passo;
    }

    //sse a derivada for positiva no intervalo ela esta esta crescendo
    if ((s1 >= 0 && s2 > 0) || (s1 > 0 && s2 >= 0)) {
        i->crescimento = (float*)realloc(i->crescimento, (i->quantidadeC + 2) * sizeof(float));
        i->crescimento[i->quantidadeC++] = x;
        i->crescimento[i->quantidadeC++] = x + passo; // Guardamos o par [início, fim]
    }

    //se a derivada for negativa no intervalo ela esta decrescendo
    if ((s1 <= 0 && s2 < 0) || (s1 < 0 && s2 <= 0)) {
        i->decrescimento = (float*)realloc(i->decrescimento, (i->quantidadeD + 2) * sizeof(float));
        i->decrescimento[i->quantidadeD++] = x;
        i->decrescimento[i->quantidadeD++] = x + passo;
    }
    //faz isso ate o fim do intervalo
    analisarPolinomio(d, x + passo, y, i);
}   

void imprimirIntervalos(float* arr, int n, char* nome, int ehTransicao) {
    printf("%s:", nome); 
        
        if (n == 0) { 
            printf("\n"); 
            return;
        }

    int aberto = 0;//isso e pra controlar se estamos dentro de um intervalo continuo(de 0.1 em 0.1) ou nao
    float inicioAtual, fimAtual;
    //percorre de dois em dois pois os pontos foram salvos em dupla
    for (int i = 0; i < n; i += 2) {
        if (ehTransicao) {
            printf(" [%.2f, %.2f]", arr[i], arr[i+1]);
        } else {
            if (!aberto) {
                inicioAtual = arr[i];
                aberto = 1;
            }
            fimAtual = arr[i+1];

            if (i + 2 >= n || fabs(arr[i+2] - fimAtual) > 0.001f) {
                printf(" [%.2f, %.2f]", inicioAtual, fimAtual);
                aberto = 0;
            }
        }
    }
    printf("\n");
}

void processarEImprimir(polinomio* d, float inicio, float fim) {
    //cria a estrutura de intervalos
    //passamos um valor inicial (ex: 10), o realloc cuidara de aumentar se precisar
    intervalo* i = criarIntervalo(10); 

    if (i == NULL) return; // segurança

    //chama a funçao recursiva de analise
    analisarPolinomio(d, inicio, fim, i);

    //chama as funçoes de impressao
    imprimirIntervalos(i->crescimento, i->quantidadeC, "Intervalos de crescimento", 0);
    imprimirIntervalos(i->decrescimento, i->quantidadeD, "Intervalos de decrescimento", 0);
    imprimirIntervalos(i->transicao, i->quantidadeT, "Pontos de transicao", 1);

    //libera a memoria para evitar Memory Leak
    destroiIntervalo(i);
}