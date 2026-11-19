#ifndef PREPARO_H
#define PREPARO_H

#define ARQ_BIN_BASE "binario.bin"

/* Gera binario.bin (a partir de PROVAO.TXT), crescente.bin e decrescente.bin
   quando ainda nao existem. Retorna 0 em caso de sucesso. */
int  garantirArquivosBase(void);
long contarRegistros(const char *nomeArqBin);

#endif
