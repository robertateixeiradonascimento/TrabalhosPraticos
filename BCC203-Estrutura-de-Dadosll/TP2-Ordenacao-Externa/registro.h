#ifndef REGISTRO_H
#define REGISTRO_H

/* Registro unico usado pelos 3 metodos (104 bytes).
   O ultimo campo e usado so pelos metodos de intercalacao, com dois nomes para o mesmo int:
     - congelado (metodo 1): registro que ficou "congelado" no heap por pertencer ao proximo bloco;
     - marcado   (metodo 2): marca de fim de bloco nas fitas / registro do proximo bloco no heap.
   O metodo 3 (Quicksort Externo) ignora esse campo.
   Usa union anonima (C11; o gcc aceita por padrao). */
typedef struct {
    long  inscricao;
    float nota;
    char  estado[3];
    char  cidade[51];
    char  curso[31];
    union {
        int congelado;
        int marcado;
    };
} Registro;

typedef Registro TipoRegistro;   /* nome usado no quicksort.c / quicksort.h */

#endif
