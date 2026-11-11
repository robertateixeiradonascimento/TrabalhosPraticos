# Makefile Mestre - TrabalhosPraticos UFOP

# Caminhos exatos identificados pelo terminal:
SUBDIRS = BCC202-Estrutura-de-Dados-I/TP1-Calcular-Polinomios \
          BCC202-Estrutura-de-Dados-I/TP2-Alocacao-e-Ponteiros \
          BCC202-Estrutura-de-Dados-I/TP3-Arvore-de-Expressao-Aritmetica

all:
	@for dir in $(SUBDIRS); do \
		if [ -d "$$dir" ]; then \
			echo "=== Compilando em $$dir ==="; \
			$(MAKE) -C $$dir; \
		fi \
	done

clean:
	@for dir in $(SUBDIRS); do \
		if [ -d "$$dir" ]; then \
			echo "=== Limpando $$dir ==="; \
			$(MAKE) -C $$dir clean; \
		fi \
	done

.PHONY: all clean $(SUBDIRS)