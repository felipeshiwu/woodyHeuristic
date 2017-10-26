#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAXSIZE 10000
int matriz[MAXSIZE][MAXSIZE];


//---------------------------------- MAPA -------------------------------------------//
typedef struct {
    int nlinhas;
    int ncolunas;
    int ncores;
    int **mapa;
} mapa;

//---------------------- ARVORE DE CAMINHOS POSSIVEIS -------------------------------//
typedef struct nodo nodo;

typedef struct tipoElementoLista {
    int *passos;
    int quantidadePassos;
    int quantidadeFilhos;
    int g;
    int menorDistanciaPosicaoMaisLonge;
    struct tipoElementoLista *prox;
} tipoElementoLista;

typedef struct tipoLista {
    tipoElementoLista *frente, *final;
} tipoLista;

//------------------------------ GRAFO ----------------------------------------------//
typedef struct {
    int distancia;
    int indice;
    int *caminhos;
} menorCaminho;



void InicLista(tipoLista *lista);
void InsereLista(tipoLista *lista, int *passos, int quantidadePassos, int g, int menorDistanciaPosicaoMaisLonge);
void RemoveLista(tipoLista *lista, tipoElementoLista *elemento);
void gera_mapa(mapa *m, int semente);
void pinta(mapa *m, int l, int c, int fundo, int cor);
void pinta_mapa(mapa *m, int cor);
int *clonaPassos(int *passos, int quantidadePassos, int cor);
void adicionaNodo(menorCaminho *menorCaminho, int borda, int distancia, tipoElementoLista *nodo_atual, mapa *m, tipoLista *lista);
void criaMatrizAdjacencia(mapa *m, int countPosicoes, int matriz[countPosicoes][countPosicoes]);
menorCaminho dijkstra(int countPosicoes, int matriz[countPosicoes][countPosicoes]);
tipoElementoLista *escolheMelhorCaminho(tipoLista *lista);
