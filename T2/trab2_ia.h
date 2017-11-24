#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct nodo {
    int val;
    char *campo;
    int c_tamanho;
    struct nodo* filhos[0];
} nodo;

typedef struct nodo nodo;

void ImprimeCampo (char *campo, int c_tamanho);
void InsereFilosofo (char *campo);
void InsereFilosofoAuto (char *campo, int pos, int lado);
void MoveBola (char *campo, int *p_bola);
void MoveBolaAuto (char *campo, int *p_bola, int pos);
nodo *novoNodo(int n, int val, char *campo, int c_tamanho);
