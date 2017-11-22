#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct nodo {
    int val;
    struct *nodo filhos[0];
} node;

void ImprimeCampo (char *campo, int c_tamanho);
void InsereFilosofo (char *campo);
void MoveBola (char *campo, int *p_bola);
nodo *novoNodo(int n, int val);