# Trabalhos Práticos - Ciência da Computação (UFOP)

Repositório dedicado ao armazenamento e organização dos Trabalhos Práticos (TPs) desenvolvidos ao longo do curso de Ciência da Computação na Universidade Federal de Ouro Preto (UFOP).

---

## 📚 Estrutura das Disciplinas

### 🔹 BCC201 - Introdução à Programação
Projetos e exercícios introdutórios focados na construção de lógica de programação e algoritmos básicos.
* **TP - Jogo Sumplete**: Implementação do jogo de lógica Sumplete em C, com alocação dinâmica de memória, manipulação de matrizes e persistência de dados.

### 🔹 BCC202 - Estruturas de Dados I
Estudo e implementação de estruturas de dados lineares e não-lineares, alocação dinâmica de memória e manipulação de ponteiros em C.
* **TP1 - Calculadora de Polinômios**: Operações e manipulação de expressões polinomiais.
* **TP2 - Alocação Dinâmica e Memória**: Exercícios práticos de gestão de memória e ponteiros.
* **TP3 - Árvore de Expressão Aritmética**: Construção e avaliação de expressões matemáticas através de árvores binárias.

### 🔹 BCC203 - Estruturas de Dados II
Algoritmos de ordenação em memória secundária, pesquisa externa e árvores de busca balanceadas.
* **TP - External Quicksort**: Algoritmo de ordenação externa Quicksort em C com suporte a ordenações ascendente, descendente e aleatória.



---

## 🛠️ Tecnologias e Ferramentas

* **Linguagens:** C, C++, Java
* **Compiladores & Ferramentas:** GCC, GHC / GHCup, Makefiles, Valgrind
* **Ambiente de Desenvolvimento:** Visual Studio Code, Windows Subsystem for Linux (WSL Ubuntu), Git & GitHub

---

## 🚀 Como Executar os Projetos

O repositório conta com um **Makefile mestre** na pasta raiz que automatiza a compilação de todos os trabalhos práticos de forma simples.

1. **Clonar o repositório:**
   ```bash
   git clone https://github.com/robertateixeiradonascimento/TrabalhosPraticos.git
   cd TrabalhosPraticos
   ```

2. **Compilar tudo com o comando `make`:**
   ```bash
   make
   ```

3. **Escolher a pasta do projeto que você quer testar e entrar nela:**
   ```bash
   cd BCC202-Estrutura-de-Dados-I
   ```

4. **Executar o programa:**
   ```bash
   ./tp_exec
   ```

5. **(Opcional) Caso deseje apagar os executáveis e arquivos `.o` gerados:**
   ```bash
   make clean
   ```