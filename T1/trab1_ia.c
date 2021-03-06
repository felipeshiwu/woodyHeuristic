#include "trab1_ia.h"

int main(int argc, char **argv) {
    mapa m;
    int semente;
    int countPosicoes;

  	carrega_mapa(&m);

    countPosicoes = m.nlinhas * m.ncolunas;

    //=====================================================================================================//
    tipoLista listaNodos;
    InicLista(&listaNodos);

    tipoElementoLista *nodo_atual;
    InsereLista(&listaNodos, NULL, 0, 0, 10000);
    nodo_atual = listaNodos.frente->prox;
    //=====================================================================================================//
    int mapaAux[m.nlinhas][m.ncolunas];

    for (int linha = 0; linha < m.nlinhas; linha++) {
        for (int coluna = 0; coluna < m.ncolunas; coluna++) {
            mapaAux[linha][coluna] = m.mapa[linha][coluna];
        }
    }

    //=====================================================================================================//
    menorCaminho menorCaminho;
    tipoElementoLista *caminhoEscolhido;
    int isCompleto = 0;
    while (!isCompleto) {
        for (int linha = 0; linha < m.nlinhas; linha++) {
            for (int coluna = 0; coluna < m.ncolunas; coluna++) {
                m.mapa[linha][coluna] = mapaAux[linha][coluna];
            }
        }

        for (int passo = 0; passo < nodo_atual->quantidadePassos; passo++) {
            pinta_mapa(&m, nodo_atual->passos[passo]);
        }

        criaMatrizAdjacencia(&m, countPosicoes, matriz);

        menorCaminho = dijkstra(countPosicoes, matriz);
        adicionaNodo(&menorCaminho, menorCaminho.indice, menorCaminho.distancia, nodo_atual, &m, &listaNodos);
        if (nodo_atual->quantidadeFilhos > 0) {
            RemoveLista(&listaNodos, nodo_atual);
        }
        caminhoEscolhido = escolheMelhorCaminho(&listaNodos);
        nodo_atual = caminhoEscolhido;
        pinta_mapa(&m, caminhoEscolhido->passos[caminhoEscolhido->quantidadePassos-1]);

        int corFundo = m.mapa[0][0];
        isCompleto = 1;
        for (int k = 0; k < m.nlinhas; k++) {
            for (int w = 1; w < m.ncolunas; w++) {
                if (corFundo != m.mapa[k][w]) {
                    isCompleto = 0;
                }
            }
        }
    }

    printf("%d\n", nodo_atual->quantidadePassos);
    for (int i = 0; i < nodo_atual->quantidadePassos; i++) {
        printf("%d ", nodo_atual->passos[i]);
    }
    //=====================================================================================================//
    printf("\n");

    return 0;
}

//Funcao que inicializa a lista de compras dos clientes
void InicLista(tipoLista *lista) {
    tipoElementoLista *l;
    l = (tipoElementoLista *)calloc(1,sizeof(tipoElementoLista));
    l->passos = NULL;
    l->quantidadePassos = 100000;
    l->quantidadeFilhos = 0;
    l->g = 0;
    l->menorDistanciaPosicaoMaisLonge = 100000;
    lista->frente = l;
    lista->final = lista->frente;
    lista->frente->prox = NULL;
}

//Funcao que insere elementos na lista dos clientes
void InsereLista(tipoLista *lista, int *passos, int quantidadePassos, int g, int menorDistanciaPosicaoMaisLonge) {
    lista->final->prox = (tipoElementoLista *)calloc(1, sizeof(tipoElementoLista));
    lista->final = lista->final->prox;
    lista->final->passos = passos;
    lista->final->quantidadePassos = quantidadePassos;
    lista->final->quantidadeFilhos = 0;
    lista->final->g = g;
    lista->final->menorDistanciaPosicaoMaisLonge = menorDistanciaPosicaoMaisLonge;
    lista->final->prox = NULL;
}

//Remove elemento da lista
void RemoveLista(tipoLista *lista, tipoElementoLista *elemento) {
    tipoElementoLista *aux;
    aux = lista->frente;
    for (aux; aux != NULL; aux = aux->prox) {
        if (aux->prox == elemento) {
            aux->prox = elemento->prox;
            elemento->prox = NULL;
            if (elemento == lista->final) {
                lista->final = aux;
            }
            break;
        }
    }
    free(elemento);
}

void gera_mapa(mapa *m, int semente) {
    int i, j;

    if(semente < 0)
    srand(time(NULL));
    else
    srand(semente);
    m->mapa = (int**) calloc(m->nlinhas, sizeof(int*));
    for(i = 0; i < m->nlinhas; i++) {
        m->mapa[i] = (int*) calloc(m->ncolunas, sizeof(int));
        for(j = 0; j < m->ncolunas; j++)
        m->mapa[i][j] = 1 + rand() % m->ncores;
    }
}

void pinta(mapa *m, int l, int c, int fundo, int cor) {
    m->mapa[l][c] = cor;
    if(l < m->nlinhas - 1 && m->mapa[l+1][c] == fundo)
    pinta(m, l+1, c, fundo, cor);
    if(c < m->ncolunas - 1 && m->mapa[l][c+1] == fundo)
    pinta(m, l, c+1, fundo, cor);
    if(l > 0 && m->mapa[l-1][c] == fundo)
    pinta(m, l-1, c, fundo, cor);
    if(c > 0 && m->mapa[l][c-1] == fundo)
    pinta(m, l, c-1, fundo, cor);
}

void pinta_mapa(mapa *m, int cor) {
    if(cor == m->mapa[0][0])
    return;
    pinta(m, 0, 0, m->mapa[0][0], cor);
}

// Clona os passos do pai e adiciona a cor escolhida, criando o filho
int *clonaPassos(int *passos, int quantidadePassos, int cor) {
    int *passosFilho = (int*) calloc(quantidadePassos+1, sizeof(int));
    for (int i = 0; i < quantidadePassos; i++) {
        passosFilho[i] = passos[i];
    }
    passosFilho[quantidadePassos] = cor;
    return passosFilho;
}

void adicionaNodo(menorCaminho *menorCaminho, int borda, int distancia, tipoElementoLista *nodo_atual, mapa *m, tipoLista *lista) {
    int posicao = borda;
    int aux = 0;
    int corAnterior = 0;
    int cor = m->mapa[posicao/m->ncolunas][posicao%m->ncolunas];
    int auxDistancia = 1;
    while (auxDistancia != distancia) {
        aux = posicao;
        corAnterior = cor;
        cor = m->mapa[menorCaminho->caminhos[aux]/m->ncolunas][menorCaminho->caminhos[aux]%m->ncolunas];
        if (cor != corAnterior) {
            auxDistancia++;
        }
        posicao = menorCaminho->caminhos[aux];
    }
    int *passosFilho = clonaPassos(nodo_atual->passos, nodo_atual->quantidadePassos, cor);
    InsereLista(lista, passosFilho, nodo_atual->quantidadePassos+1, nodo_atual->g + 1, distancia);
    nodo_atual->quantidadeFilhos++;
}

void criaMatrizAdjacencia(mapa *m, int countPosicoes, int matriz[countPosicoes][countPosicoes]) {
    for (int i = 0; i < countPosicoes; i++)
    for (int j = 0; j < countPosicoes; j++) {
        if (i == j)
        matriz[i][j] = 0;
        else
        matriz[i][j] = -1;
    }
    for (int linha = 0; linha < m->nlinhas; linha ++) {
        for (int coluna = 0; coluna < m->ncolunas; coluna++) {
            if (coluna != 0) {
                if (m->mapa[linha][coluna] == m->mapa[linha][coluna-1])
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * linha) + coluna-1] = 0;
                else
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * linha) + coluna-1] = 1;
            }
            if (coluna < m->ncolunas - 1) {
                if (m->mapa[linha][coluna] == m->mapa[linha][coluna+1])
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * linha) + coluna+1] = 0;
                else
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * linha) + coluna+1] = 1;
            }
            if (linha != 0) {
                if (m->mapa[linha][coluna] == m->mapa[linha-1][coluna])
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * (linha-1)) + coluna] = 0;
                else
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * (linha-1)) + coluna] = 1;
            }
            if (linha < m->nlinhas - 1) {
                if (m->mapa[linha][coluna] == m->mapa[linha+1][coluna])
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * (linha+1)) + coluna] = 0;
                else
                matriz[((m->ncolunas) * linha) + coluna][((m->ncolunas) * (linha+1)) + coluna] = 1;
            }
        }
    }
}

menorCaminho dijkstra(int countPosicoes, int matriz[countPosicoes][countPosicoes]) {
    menorCaminho menor;
    menor.caminhos = (int*) calloc(countPosicoes, sizeof(int));
    int dis[countPosicoes];
    char vis[countPosicoes];
    memset (vis, 0, sizeof (vis));
    memset (dis, 0x7f, sizeof (dis));
    dis[0] = 0;
    int t, i;
    for (t = 0; t < countPosicoes; t++)
    {
        int v = -1;
        for (i = 0; i < countPosicoes; i++) {
            if (!vis[i] && (v < 0 || dis[i] < dis[v])) {
                v = i;
            }
        }
        vis[v] = 1;
        for (i = 0; i < countPosicoes; i++) {
            if (matriz[v][i] >= 0 && dis[i] > dis[v] + matriz[v][i]) {
                dis[i] = dis[v] + matriz[v][i];
                menor.caminhos[i] = v;
            }
        }
    }
    int maior = 0;
    int indice = 0;
    for (int i = 0; i < countPosicoes; i++) {
        if (dis[i] > maior) {
            maior = dis[i];
            indice = i;
        }
    }
    menor.distancia = maior;
    menor.indice = indice;
    return menor;
}

tipoElementoLista *escolheMelhorCaminho(tipoLista *lista) {
    tipoElementoLista *melhorCaminho;
    tipoElementoLista *aux = lista->frente->prox;
    melhorCaminho = aux;
    for (aux; aux != NULL; aux = aux->prox) {
        int custo = aux->g + aux->menorDistanciaPosicaoMaisLonge;
        int menorCusto = melhorCaminho->g + melhorCaminho->menorDistanciaPosicaoMaisLonge;
        if (custo < menorCusto) {
            melhorCaminho = aux;
        }
    }
    return melhorCaminho;
}

void carrega_mapa(mapa *m) {
  int i, j;

  scanf("%d", &(m->nlinhas));
  scanf("%d", &(m->ncolunas));
  scanf("%d", &(m->ncores));
  m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
  for(i = 0; i < m->nlinhas; i++) {
    m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
    for(j = 0; j < m->ncolunas; j++)
      scanf("%d", &(m->mapa[i][j]));
  }
}
