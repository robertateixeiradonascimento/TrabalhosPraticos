# Makefile Mestre - TrabalhosPraticos UFOP

# Lista com os caminhos de todos os TPs que possuem Makefile próprio
SUBDIRS = BCC201-Introducao-a-Programacao/TP-Jogo-Sumplete \
          BCC202-Estruturas-de-Dados-I/TP1-Calcular-Polinomios \
          BCC202-Estruturas-de-Dados-I/TP2-Alocacao-e-Ponteiros \
          BCC202-Estruturas-de-Dados-I/TP3-Arvore-de-Expressao-Aritmetica

# Compila todos os TPs sequencialmente
all:
	@for dir in $(SUBDIRS); do \
		echo "=== Compilando em $$dir ==="; \
		$(MAKE) -C $$dir; \
	done

# Limpa os executáveis e arquivos .o de todos os TPs
clean:
	@for dir in $(SUBDIRS); do \
		echo "=== Limpando $$dir ==="; \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: all clean $(SUBDIRS)