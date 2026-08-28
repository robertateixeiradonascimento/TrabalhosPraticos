//Roberta Teixeira do Nascimento 
//25.2.4150
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#define T 27// nome de jogador e comandos
#define TAM 100 // para nomes de arquivo
#define ANSI_BOLD "\033[1m"
#define ANSI_RESET   "\033[0m"
#define ANSI_COLOR_CYAN       "\x1b[36m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m" 
#define ANSI_COLOR_MAGENTA    "\x1b[35m" 
#define TAB_HOR "\u2501" // ━ 
#define TAB_VER "\u2503" // ┃    
#define TAB_TL  "\u250F" // ┏ 
#define TAB_ML  "\u2523" // ┣ 
#define TAB_BL  "\u2517" // ┗ 
#define TAB_TJ  "\u2533" // ┳ 
#define TAB_MJ  "\u254B" // ╋ 
#define TAB_BJ  "\u253B" // ┻ 
#define TAB_TR  "\u2513" // ┓ 
#define TAB_MR  "\u252B" // ┫ 
#define TAB_BR  "\u251B" // ┛ 

typedef struct{
    char nome[T];
    double tempo;
}Ranking;

typedef struct{
    int tamanho;
    int **tabela;
    int **solucao;
    int **removidoouadicionado;
    int *somalinha;
    int *somacoluna;
    char jogador[T] , comando[T] , entrada[50];
    int dificuldade;
    double tempogasto;
    Ranking lista[11];
}Sumplete;

void liberarMemoria(Sumplete *j) {
    if ((*j).tabela != NULL) {
        for (int i = 0; i < (*j).tamanho; i++) {
            free((*j).tabela[i]);
            free((*j).solucao[i]);
            free((*j).removidoouadicionado[i]);
        }
        free((*j).tabela); (*j).tabela = NULL;
        free((*j).solucao); (*j).solucao = NULL;
        free((*j).removidoouadicionado); (*j).removidoouadicionado = NULL;
        free((*j).somalinha); (*j).somalinha = NULL;
        free((*j).somacoluna); (*j).somacoluna = NULL;
    }
}

void limpar_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void comandos(){
    printf(ANSI_BOLD"BEM VINDO AO JOGO SUMPLETE\n" ANSI_RESET);
    printf("\n");
    printf(ANSI_BOLD"COMANDOS DO JOGO\n"ANSI_RESET);
    printf(ANSI_BOLD"AJUDA: "ANSI_RESET "Exibe os comandos do jogo\n");
    printf(ANSI_BOLD"SAIR: "ANSI_RESET "Sair do jogo\n");
    printf(ANSI_BOLD"NOVO: "ANSI_RESET "Começar um novo jogo\n");
    printf(ANSI_BOLD"CARREGAR: " ANSI_RESET"Carrregar um jogo salvo em arquivo\n");
    printf(ANSI_BOLD"RANKING: "ANSI_RESET "Exibir o ranking\n");
    printf(ANSI_BOLD"SALVAR: " ANSI_RESET"Salva o jogo atual\n");
    printf(ANSI_BOLD"DICA: "ANSI_RESET "Marca uma posição no jogo\n");
    printf(ANSI_BOLD"RESOLVER: "ANSI_RESET "Resolve o jogo atual\n");
    printf(ANSI_BOLD"REMOVER <lin> <col>: "ANSI_RESET "Remove a posição da soma\n");
    printf(ANSI_BOLD"ADICIONAR <lin> <col>: "ANSI_RESET "Adiciona a posição de volta à soma\n");
}

int verificaComando(Sumplete *j){
    if(strcmp((*j).comando, "adicionar") == 0 || strcmp((*j).comando, "Adicionar") == 0) return 1;
    else if(strcmp((*j).comando, "remover") == 0 || strcmp((*j).comando, "Remover") == 0) return 2;
    else if(strcmp((*j).comando, "novo") == 0 || strcmp((*j).comando , "Novo") == 0) return 3;
    else if(strcmp((*j).comando, "carregar") == 0 || strcmp((*j).comando, "Carregar") == 0) return 4;
    else if(strcmp((*j).comando, "ranking") == 0 || strcmp((*j).comando, "Ranking") == 0) return 5;
    else if(strcmp((*j).comando,"salvar") == 0 || strcmp((*j).comando , "Salvar") == 0) return 6;
    else if(strcmp((*j).comando, "dica") == 0 || strcmp((*j).comando, "Dica") == 0) return 7;
    else if(strcmp((*j).comando, "ajuda") == 0 || strcmp((*j).comando, "Ajuda") == 0) return 8;
    else if(strcmp((*j).comando, "sair") == 0 || strcmp((*j).comando, "Sair") == 0) return 9;
    else if(strcmp((*j).comando, "resolver") == 0 || strcmp((*j).comando, "Resolver") == 0) return 10;
    else return 0;
}

int **criaMatriz(int tamanho){
    int **m = malloc(tamanho * sizeof(int*));
    for(int i = 0; i < tamanho; i++){
        m[i] = malloc(tamanho * sizeof(int));
    }
    return m;
}

void preencherTabela(Sumplete *j){
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            (*j).tabela[i][r] = (rand() % 9) + 1;
            (*j).solucao[i][r] = (rand() % 2);
            (*j).removidoouadicionado[i][r] = 2;
            if((*j).solucao[i][r] == 1){
                (*j).somalinha[i] += (*j).tabela[i][r];
                (*j).somacoluna[r] += (*j).tabela[i][r];
            }
        }
    }
}

void inicializarMatrizes(Sumplete *j){
    (*j).tabela = criaMatriz((*j).tamanho);
    (*j).solucao = criaMatriz((*j).tamanho);
    (*j).removidoouadicionado = criaMatriz((*j).tamanho);
    (*j).somalinha = calloc((*j).tamanho, sizeof(int));
    (*j).somacoluna = calloc((*j).tamanho, sizeof(int));
    preencherTabela(j);
}

void tabuleiro(Sumplete *j) {
    printf(ANSI_BOLD ANSI_COLOR_MAGENTA "\n          SUMPLETE!          \n\n" ANSI_RESET);
    printf("       "); 
    for (int r = 0; r < (*j).tamanho; r++) {
        printf(ANSI_COLOR_CYAN "  %d   " ANSI_RESET, r + 1);
    }
    printf("\n");

    printf("      " TAB_TL);
    for (int r = 0; r < (*j).tamanho; r++) {
        printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR);
        if (r < (*j).tamanho - 1) printf(TAB_TJ);
    }
    printf(TAB_TR "\n");

    for (int i = 0; i < (*j).tamanho; i++) {
        printf(ANSI_COLOR_CYAN " %2d   " ANSI_RESET TAB_VER, i + 1);

        for (int r = 0; r < (*j).tamanho; r++) {
            if ((*j).removidoouadicionado[i][r] == 1) {
                printf(ANSI_COLOR_GREEN "  %2d " ANSI_RESET TAB_VER, (*j).tabela[i][r]);
            }
            else if ((*j).removidoouadicionado[i][r] == 0){
                printf(ANSI_COLOR_RED "  %2d " ANSI_RESET TAB_VER, (*j).tabela[i][r]);
            }
            else {
                printf("  %2d " TAB_VER, (*j).tabela[i][r]);
            }
        }
        printf(ANSI_BOLD "  %d\n" ANSI_RESET, (*j).somalinha[i]);
        if (i < (*j).tamanho - 1) {
            printf("      " TAB_ML);
            for (int r = 0; r < (*j).tamanho; r++) {
                printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR);
                if (r < (*j).tamanho - 1) 
                printf(TAB_MJ);
            }
            printf(TAB_MR "\n");
        }
    }

    printf("      " TAB_BL);
    for (int r = 0; r < (*j).tamanho; r++) {
        printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_HOR);
        if (r < (*j).tamanho - 1) printf(TAB_BJ);
    }
    printf(TAB_BR "\n");

    printf("       ");
    for (int r = 0; r < (*j).tamanho; r++) {
        printf(ANSI_BOLD " %2d   " ANSI_RESET, (*j).somacoluna[r]);
    }
    printf("\n\n");
}

void juntarNome(char *jogador){
    int i = 0, r = 0;
    while(jogador[i] != '\0'){
        if(jogador[i] != ' '){
          jogador[r] = jogador[i];
         r++;
      }
     i++;
    }
    jogador[r] = '\0';
}

void jogoNovo(Sumplete *j){
    int valido = 0, numero;
    char escolha; // Variável auxiliar para ler a letra do teclado

    if((*j).tamanho > 0){
        liberarMemoria(j);
    }
    
    do{
     printf("\nDigite o nome do jogador: "); 
     fgets((*j).jogador, sizeof((*j).jogador), stdin);
     (*j).jogador[strcspn((*j).jogador, "\n")] = '\0';
     juntarNome((*j).jogador);
     numero = strlen((*j).jogador);
     if(numero > 27)
     printf("ERRO!!! Digite um nome de ate 27 caracteres!\n");
    }while(numero > 27);
    
    printf(ANSI_BOLD"\n F:"ANSI_RESET "nível fácil, tamanho 3 x 3\n"ANSI_BOLD " M:"ANSI_RESET "nível médio, tamanho 5 x 5\n "ANSI_BOLD"D:" ANSI_RESET "nível difícil, tamanho 7 x 7\n Digite a dificuldade: "); 
        
     do{
         scanf(" %c" , &escolha);
         limpar_buffer();
         
        if(escolha == 'F' || escolha == 'f'){
          (*j).dificuldade = 1;
          (*j).tamanho = 3;
          valido = 1;
        }
        else if(escolha == 'M' || escolha == 'm'){
          (*j).dificuldade = 2;
          (*j).tamanho = 5;
          valido = 1;
        }
        else if(escolha == 'D' || escolha == 'd'){
          (*j).dificuldade = 3;
          (*j).tamanho = 7;
          valido = 1;
        }
        else{
          printf("ERRO! Digite F para fácil, M para médio e D para díficil: ");
        }
     }while (valido == 0);
     
     inicializarMatrizes(j);
}  

int verificaVitoria(Sumplete *j) {
    int todasCertasMarcadas = 1;
    int todasErradasRemovidas = 1;

    for (int i = 0; i < (*j).tamanho; i++) {
        for (int r = 0; r < (*j).tamanho; r++) {
            if ((*j).solucao[i][r] == 1) {
                if ((*j).removidoouadicionado[i][r] != 1) {
                    todasCertasMarcadas = 0; 
                }
            } 
            else if ((*j).solucao[i][r] == 0) {
                if ((*j).removidoouadicionado[i][r] != 0) {
                    todasErradasRemovidas = 0; 
                }
            }
            
        }
    }

    if (todasCertasMarcadas == 1 || todasErradasRemovidas == 1) {
        return 1; 
    }
    
    return 0; 
}

void darDica(Sumplete *j) {
    for (int i = 0; i < (*j).tamanho; i++) {
        for (int r = 0; r < (*j).tamanho; r++) {
            if ((*j).removidoouadicionado[i][r] != (*j).solucao[i][r] && (*j).solucao[i][r] == 1) {
                (*j).removidoouadicionado[i][r] = 1;
                return;
            }
        }
    }
  printf("\nOPS! Sem mais números para adicionar à soma!\n");   
}

void resolverJogo(Sumplete *j){
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            if((*j).solucao[i][r] == 1)
             (*j).removidoouadicionado[i][r] = 1;
            else
             (*j).removidoouadicionado[i][r] = 0;
        }
    }
}
 
void salvarJogo(Sumplete *j, char *nomearquivo){
    int remover = 0, pecasmodificadas = 0;
    char nomedojogosalvo[TAM];
    
    sprintf(nomedojogosalvo, "%s.sum", nomearquivo);
    
    FILE* arquivo;
    arquivo = fopen(nomedojogosalvo, "w");
    
    fprintf(arquivo, "%d\n" ,(*j).tamanho);
    
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            fprintf(arquivo, "%d ", (*j).tabela[i][r]);
        }
        fprintf(arquivo, "\n");
    }
    
    for(int i = 0; i < (*j).tamanho; i++){
        fprintf(arquivo, "%d ", (*j).somalinha[i]);
    }
    fprintf(arquivo, "\n");
    
    for(int r = 0; r < (*j).tamanho; r++){
        fprintf(arquivo, "%d " , (*j).somacoluna[r]);
    }
    fprintf(arquivo, "\n");
    
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            if((*j).solucao[i][r] == 0)
                remover++;
        }
    }
    fprintf(arquivo, "%d ", remover);
    
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            if((*j).solucao[i][r] == 0)
                fprintf(arquivo, "\n%d %d", i, r);
        }
    }
    
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            if((*j).removidoouadicionado[i][r] != 2)
                pecasmodificadas++;
        }
    }
    fprintf(arquivo, "\n%d\n" , pecasmodificadas);
    
    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            if((*j).removidoouadicionado[i][r] == 1)
                fprintf(arquivo,"a %d %d\n",  i, r);
            else if((*j).removidoouadicionado[i][r] == 0)
                fprintf(arquivo,"r %d %d\n",  i, r);
        }
    }
    
    fprintf(arquivo, "%s\n", (*j).jogador);
    fprintf(arquivo, "%lf", (*j).tempogasto);
    
    fclose(arquivo);
}

void carregarJogo(Sumplete *j, char *nome){
    FILE *arquivojogo;
    char nomearquivo[TAM], letra;
    int remover, linha, coluna, pecasmodificadas;
    sprintf(nomearquivo, "%s.sum", nome);
    arquivojogo = fopen(nomearquivo, "r");
    if(arquivojogo == NULL){
        printf("\nERRO! Digite um nome de arquivo válido!\n");
        return;
    }
    
    if((*j).tamanho > 0) liberarMemoria(j);
    
    fscanf(arquivojogo, "%d\n", &(*j).tamanho);
    if ((*j).tamanho != 3 && (*j).tamanho != 5 && (*j).tamanho != 7) {
        printf("\nERRO! Arquivo corrompido: tamanho do tabuleiro inválido.\n");
        (*j).tamanho = 0;
        fclose(arquivojogo);
        return;
    }

    (*j).tabela = criaMatriz((*j).tamanho);
    (*j).solucao = criaMatriz((*j).tamanho);
    (*j).removidoouadicionado = criaMatriz((*j).tamanho);
    (*j).somalinha = calloc((*j).tamanho, sizeof(int));
    (*j).somacoluna = calloc((*j).tamanho, sizeof(int));

    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            if(fscanf(arquivojogo, "%d ", &(*j).tabela[i][r]) != 1) {
                printf("\nERRO! Dados corrompidos na tabela.\n");
                liberarMemoria(j);
                (*j).tamanho = 0;
                fclose(arquivojogo);
                return;
            }
        }
        fscanf(arquivojogo, "\n");
    }

    for(int i = 0; i < (*j).tamanho; i++){
        for(int r = 0; r < (*j).tamanho; r++){
            (*j).solucao[i][r] = 1;
            (*j).removidoouadicionado[i][r] = 2;
        }
    }
    for(int i = 0; i < (*j).tamanho; i++){
        fscanf(arquivojogo, "%d ", &(*j).somalinha[i]);
    }
     fscanf(arquivojogo, "\n");
    for(int r = 0; r < (*j).tamanho; r++){
        fscanf(arquivojogo,"%d ", &(*j).somacoluna[r]);
    }
    fscanf(arquivojogo, "\n");
    fscanf(arquivojogo, "%d ", &remover);
    for(int i = 0; i < remover; i++){
            fscanf(arquivojogo, "%d %d", &linha, &coluna);
            if (linha >= 0 && linha < (*j).tamanho && coluna >= 0 && coluna < (*j).tamanho)
                (*j).solucao[linha][coluna] = 0;
    }
    fscanf(arquivojogo, "%d", &pecasmodificadas);

    for(int i = 0; i < pecasmodificadas; i++){
            fscanf(arquivojogo, " %c %d %d", &letra, &linha, &coluna);
            if (linha >= 0 && linha < (*j).tamanho && coluna >= 0 && coluna < (*j).tamanho) {
                if(letra == 'a')
                 (*j).removidoouadicionado[linha][coluna] = 1;
                else if(letra == 'r')
                 (*j).removidoouadicionado[linha][coluna] = 0;
                else
                 (*j).removidoouadicionado[linha][coluna] = 2;
            }
    }
    fscanf(arquivojogo," ");
    fgets((*j).jogador, sizeof((*j).jogador), arquivojogo);
    (*j).jogador[strcspn((*j).jogador, "\n")] = '\0';
    fscanf(arquivojogo, "%lf", &(*j).tempogasto);

    fclose(arquivojogo);
    tabuleiro(j);
}

double tempogasto(struct timeval inicio, struct timeval fim){
    long sec = fim.tv_sec - inicio.tv_sec;
    long microsec = fim.tv_usec - inicio.tv_usec;
    double passado = sec + microsec * 1e-6;
    return passado;
}

void dadosRanking(Sumplete *j){
    Ranking aux;
    int n = 0;
    FILE *arquivo = fopen("sumplete.rnk", "r");
    if(arquivo != NULL){
      while (n < 10 && fscanf(arquivo, "%26s %lf", (*j).lista[n].nome, &(*j).lista[n].tempo) == 2) {
            n++;
     }
     fclose(arquivo);
    }
    strcpy((*j).lista[n].nome , (*j).jogador);
    (*j).lista[n].tempo = (*j).tempogasto;
    n++;
    for(int i = 0; i < n - 1; i++){
        for(int r = i + 1; r < n; r++){
            if((*j).lista[r].tempo < (*j).lista[i].tempo){
                aux = (*j).lista[i];
                (*j).lista[i] = (*j).lista[r];
                (*j).lista[r] = aux;
            }
        }
    }
    arquivo = fopen("sumplete.rnk", "w");
    for(int i = 0; i < n && i < 10; i++)
    fprintf(arquivo, "%s %.0lf\n", (*j).lista[i].nome, (*j).lista[i].tempo);
    fclose(arquivo);
}

int buscarPosicao(Sumplete *j) {
    for (int i = 0; i < 10; i++) {
        if (strcmp((*j).lista[i].nome, (*j).jogador) == 0 && 
            (*j).lista[i].tempo == (*j).tempogasto) {
            return i + 1; 
        }
    }
    return -1;
}

void exibirRanking(Sumplete *j){
    char nome[TAM];
    double tempo;
    FILE *arquivo ;
    arquivo = fopen("sumplete.rnk", "r");
    if(arquivo == NULL){
        printf("Nenhum ranking encontrado.\n");
        return;
    }
    while(fscanf(arquivo, "%26s %lf", nome , &tempo) == 2){
     printf("%s %.0lf\n", nome, tempo);
    }
    fclose(arquivo);
}

int main(){
    Sumplete j;
    srand(time(NULL));
    int resultado, L , C, vitoria = 0, lugarRanking, salvarounao;
    char r[T], nomearquivo[TAM], nome[TAM];
    j.tamanho = 0;
    j.tabela = NULL;
    struct timeval inicio = {0};
    struct timeval fim = {0};
    comandos();

    do{
        if (j.tamanho > 0)
         printf("%s, digite um comando:  ", j.jogador);
        else 
         printf("\nDigite um comando: ");
        
        fgets(j.entrada, sizeof(j.entrada), stdin);
        sscanf(j.entrada, "%s", j.comando);
        resultado = verificaComando(&j);

        if (j.tamanho == 0 && (resultado != 5 && resultado != 3 && resultado != 8 && resultado != 9 && resultado != 4)) {
            printf("Aviso: Use o comando 'novo' para começar!\n");
            continue;
        }
     
        switch (resultado){
        case 0:
            printf("Comando inválido!!! Tente de novo!\n");
            break;
        case 1:
            if(sscanf(j.entrada, "%s %d %d" , j.comando, &L, &C) != 3){
                printf("\nErro...! Forma correta, adicionar <linha> <coluna>\n");
            }
            else if(L <= 0 || L > j.tamanho || C <= 0 || C > j.tamanho) {
                printf("\nValores inválidos para linha ou coluna!!!\n");
            }
            else{
              j.removidoouadicionado[L-1][C-1] = 1;
              tabuleiro(&j);
            }
            // Adicionado a verificação de vitória ao adicionar
            vitoria = verificaVitoria(&j);
            if(vitoria == 1){
                gettimeofday(&fim, 0);
                j.tempogasto = tempogasto(inicio, fim);
                dadosRanking(&j);
                lugarRanking = buscarPosicao(&j);
                printf("\nPARABÉNS %s! Você ganhou!\nPosição %d, %.2lf segundos \n", j.jogador, lugarRanking, j.tempogasto);   
            }
            break;
        case 2:
            if(sscanf(j.entrada, "%s %d %d" , j.comando, &L, &C) != 3){
                printf("Erro...! Forma correta, remover <linha> <coluna>\n");
            }
            else if(L <= 0 || L > j.tamanho || C <= 0 || C > j.tamanho) {
                printf("Valores inválidos para linha ou coluna!!!\n");
            }
            else{
              j.removidoouadicionado[L-1][C-1] = 0;
              tabuleiro(&j);
            }
            vitoria = verificaVitoria(&j);
            if(vitoria == 1){
                gettimeofday(&fim, 0);
                j.tempogasto = tempogasto(inicio, fim);
                dadosRanking(&j);
                lugarRanking = buscarPosicao(&j);
                printf("\nPARABÉNS %s! Você ganhou!\nPosição %d, %.2lf segundos \n", j.jogador, lugarRanking, j.tempogasto);   
            }
            break;
        case 3:
            gettimeofday(&inicio, 0);
            jogoNovo(&j);
            tabuleiro(&j);
            break;
        case 4:
            if(sscanf(j.entrada, "%s %s", j.comando, nome) != 2){
                printf("\nERRO! Digite o comando na forma: carregar <nome do arquivo>\n");
            }
            else
                carregarJogo(&j, nome);
            gettimeofday(&inicio, 0);
            break;
        case 5:
            exibirRanking(&j);
            break;
        case 6:
            if(sscanf(j.entrada, "%s %s", j.comando, nomearquivo)!= 2){
                printf("\nERRO! Digite o comando na forma: salvar <nome do arquivo>\n");
            }
            else{
                gettimeofday(&fim, 0);
                j.tempogasto += tempogasto(inicio, fim);
                salvarJogo(&j,nomearquivo);
                gettimeofday(&inicio, 0);
            }
            break;
        case 7:
            darDica(&j);
            tabuleiro(&j);
            break;
        case 8:
            comandos();
            break;
        case 9:
            salvarounao = verificaVitoria(&j);
            if(salvarounao == 0 && j.tamanho > 0){
                printf("\nDeseja salvar o jogo atual? ");
                fgets(r, T, stdin);
                r[strcspn(r, "\n")] = '\0';
                if(strcmp(r,"sim") == 0 || strcmp(r, "Sim") == 0 ||strcmp(r, "S") == 0 || strcmp(r, "s") == 0){
                   printf("\nDigite o nome para salvamento:");
                   fgets(nomearquivo, TAM, stdin);
                   nomearquivo[strcspn(nomearquivo, "\n")] = '\0';
                   salvarJogo(&j, nomearquivo);
                }
                else if(strcmp(r,"não") != 0 && strcmp(r, "Não") != 0 && strcmp(r, "N") != 0 && strcmp(r, "n") != 0){
                    printf("\nERRO! Digite Sim para salvar e Não para não fazê-lo: ");
                    fgets(r,T,stdin);
                }
            }
            printf("\nSaindo do jogo... Até a próxima!");
            break;
        case 10:
            resolverJogo(&j);
            tabuleiro(&j);
            gettimeofday(&fim, 0);
            j.tempogasto = tempogasto(inicio, fim);
            dadosRanking(&j);
            lugarRanking = buscarPosicao(&j);
            printf("PARABÉNS %s! Você ganhou!\nPosição %d , %.2lf segundos\n", j.jogador, lugarRanking, j.tempogasto);
            break;
        } 
    }while(resultado != 9);
    
    liberarMemoria(&j);
    return 0;
}