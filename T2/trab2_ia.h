#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct nodo {
    int val;
    int c_tamanho;
    int posicao_jogada;
    int count_filhos;
    int *posicoes_pulos;
    char jogada;
    char *campo;
    struct nodo* filhos[0];
} nodo;

typedef struct nodo nodo;

void ImprimeCampo (char *campo, int c_tamanho);
void InsereFilosofo (char *campo, int n_campo);
void InsereFilosofoAuto (char *campo, int pos, int lado);
void MoveBola (char *campo, int *p_bola, int n_acao, int *n_campo);
void MoveBolaAuto (char *campo, int *p_bola, int pos);
nodo *novoNodo(int val, char *campo, int c_tamanho, char acao, int n_acao, int *n_campo);
int miniMax(nodo *pai, int countIteracoes, int p_bola, char lado);
