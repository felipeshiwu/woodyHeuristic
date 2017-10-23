#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAXSIZE 10000
int matriz[MAXSIZE][MAXSIZE];
int menorCusto = 0x7f;

//---------------------------------- MAPA -------------------------------------------//
typedef struct {
    int nlinhas;
    int ncolunas;
    int ncores;
    int **mapa;
} mapa;

//-------------------------------- FRONTEIRA ----------------------------------------//
typedef struct {
    int linha;
    int coluna;
    int cor;
} posicao;

typedef struct {
    int tamanho;
    posicao *elementos;
} fronteira;

//---------------------- ARVORE DE CAMINHOS POSSIVEIS -------------------------------//
typedef struct nodo nodo;

typedef struct tipoElementoLista {
    int *passos;
    mapa *mapa;
    int quantidadePassos;
    int quantidadeFilhos;
    int g;
    int menorDistanciaBorda;
    int atingiuBordaCimaDireita;
    int atingiuBordaBaixoDireita;
    int atingiuBordaBaixoEsquerda;
    int atingiuBordaBaixoMeio;
    struct tipoElementoLista *prox;
} tipoElementoLista;

typedef struct tipoLista {
    tipoElementoLista *frente, *final;
} tipoLista;


//------------------------------ GRAFO ----------------------------------------------//
typedef struct {
    int distancia;
    int distanciaBD;
    int distanciaCD;
    int distanciaBE;
    int distanciaBM;
    int *caminhos;
} menorCaminho;


//Funcao que inicializa a lista de compras dos clientes
void InicLista(tipoLista *lista) {
    tipoElementoLista *l;
    l = (tipoElementoLista *)calloc(1,sizeof(tipoElementoLista));
    l->passos = NULL;
    l->mapa = NULL;
    l->quantidadePassos = 0x7f;
    l->quantidadeFilhos = 0;
    l->g = 0;
    l->menorDistanciaBorda = 0x7f;
    l->atingiuBordaCimaDireita = 0;
    l->atingiuBordaBaixoDireita = 0;
    l->atingiuBordaBaixoEsquerda = 0;
    l->atingiuBordaBaixoMeio = 0;
    lista->frente = l;
    lista->final = lista->frente;
    lista->frente->prox = NULL;
}

//Funcao que insere elementos na lista dos clientes
void InsereLista(tipoLista *lista, int *passos, mapa *mapa, int quantidadePassos, int g, int menorDistanciaBorda, int bordaCD, int bordaBD, int bordaBE, int bordaBM) {
    lista->final->prox = (tipoElementoLista *)calloc(1, sizeof(tipoElementoLista));
    lista->final = lista->final->prox;
    lista->final->passos = passos;
    lista->final->mapa = mapa;
    lista->final->quantidadePassos = quantidadePassos;
    lista->final->quantidadeFilhos = 0;
    lista->final->g = g;
    lista->final->menorDistanciaBorda = menorDistanciaBorda;
    lista->final->atingiuBordaCimaDireita = bordaCD;
    lista->final->atingiuBordaBaixoDireita = bordaBD;
    lista->final->atingiuBordaBaixoEsquerda = bordaBE;
    lista->final->atingiuBordaBaixoEsquerda = bordaBM;
    lista->final->prox = NULL;
}

//Testa se a lista do cliente está vazia
int VaziaLista(tipoLista lista) {
    return (lista.final == lista.frente);
}

//Remove elementos da lista
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

void carrega_mapa(mapa *m) {
    int i, j;
    
    scanf("%d", &(m->nlinhas));
    scanf("%d", &(m->ncolunas));
    scanf("%d", &(m->ncores));
    m->mapa = (int**) calloc(m->nlinhas, sizeof(int*));
    for(i = 0; i < m->nlinhas; i++) {
        m->mapa[i] = (int*) calloc(m->ncolunas, sizeof(int));
        for(j = 0; j < m->ncolunas; j++)
        scanf("%d", &(m->mapa[i][j]));
    }
}

void mostra_mapa(mapa *m) {
    int i, j;
    
    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
        if(m->ncores > 10)
        printf("%3d", m->mapa[i][j]);
        else
        printf("%3d ", m->mapa[i][j]);
        printf("\n");
    }
}

void mostra_mapa_cor(mapa *m) {
    int i, j;
    char* cor_ansi[] = { "\x1b[0m",
    "\x1b[31m", "\x1b[32m", "\x1b[33m",
    "\x1b[34m", "\x1b[35m", "\x1b[36m",
    "\x1b[37m", "\x1b[30;1m", "\x1b[31;1m",
    "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m",
    "\x1b[35;1m", "\x1b[36;1m", "\x1b[37;1m" };
    
    if(m->ncores > 15) {
        mostra_mapa(m);
        return;
    }
    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
        if(m->ncores > 10)
        printf("%s%3d%s", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
        else
        printf("%s%3d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
        printf("\n");
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

void getFronteira(fronteira *f, mapa *m, int linha, int coluna, int cor, int **mapaAux) {
    if (cor == m->mapa[0][0]) {
        mapaAux[linha][coluna] = -1;
        if (coluna != 0 && mapaAux[linha][coluna-1] != -1)
        getFronteira(f, m, linha, coluna - 1, m->mapa[linha][coluna-1], mapaAux);
        if (coluna < m->ncolunas - 1 && mapaAux[linha][coluna+1] != -1)
        getFronteira(f, m, linha, coluna + 1, m->mapa[linha][coluna+1], mapaAux);
        if (linha != 0 && mapaAux[linha-1][coluna] != -1)
        getFronteira(f, m, linha - 1, coluna, m->mapa[linha-1][coluna], mapaAux);
        if (linha < m->nlinhas - 1 && mapaAux[linha+1][coluna] != -1)
        getFronteira(f, m, linha + 1, coluna, m->mapa[linha+1][coluna], mapaAux);
    }
    else {
        //Repeticao de elementos na fronteira - otimizar
        int jaExiste = 0;
        for (int i = 0; i < f->tamanho; i++) {
            if (f->elementos[i].linha == linha && f->elementos[i].coluna == coluna) {
                jaExiste = 1;
                break;
            }
        }
        if (!jaExiste) {
            posicao elemento;
            elemento.linha = linha;
            elemento.coluna = coluna;
            elemento.cor = cor;
            f->elementos[f->tamanho] = elemento;
            f->tamanho++;
        }
    }
}

int *clonaPassos(int *passos, int quantidadePassos, int cor) {
    int *passosFilho = (int*) calloc(quantidadePassos+1, sizeof(int));
    for (int i = 0; i < quantidadePassos; i++) {
        passosFilho[i] = passos[i];
    }
    passosFilho[quantidadePassos] = cor;
    return passosFilho;
}

mapa clonaMapa(mapa *m, int cor) {
    mapa mapaFilho;
    mapaFilho.ncolunas = m->ncolunas;
    mapaFilho.nlinhas = m->nlinhas;
    mapaFilho.ncores = m->ncores;
    mapaFilho.mapa = (int**) calloc(m->nlinhas, sizeof(int*));
    for (int i = 0; i < m->nlinhas; i++) {
        mapaFilho.mapa[i] = (int*) calloc(m->ncolunas, sizeof(int)); 
    }
    for (int i = 0; i < m->nlinhas; i++) {
        for (int j = 0; j < m->ncolunas; j++) {
            mapaFilho.mapa[i][j] = m->mapa[i][j];
        } 
    }
    pinta_mapa(&mapaFilho, cor);
    return mapaFilho;
}

void adicionaNodo(menorCaminho *menorCaminho, int borda, int distancia, tipoElementoLista *nodo_atual, tipoLista *lista, int count) {
    mapa *m = nodo_atual->mapa;
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
    mapa mapaNovo = clonaMapa(nodo_atual->mapa, cor);
    InsereLista(lista, clonaPassos(nodo_atual->passos, nodo_atual->quantidadePassos, cor),
    &mapaNovo,
    (nodo_atual->quantidadePassos)+1, nodo_atual->g + 1, distancia,
    nodo_atual->atingiuBordaCimaDireita, nodo_atual->atingiuBordaBaixoDireita, nodo_atual->atingiuBordaBaixoEsquerda, nodo_atual->atingiuBordaBaixoMeio);
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

menorCaminho dijkstra(int countPosicoes, int matriz[countPosicoes][countPosicoes], int BD, int CD, int BE, int BM) {
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
    menor.distancia = dis[0];
    menor.distanciaBD = dis[BD];
    menor.distanciaCD = dis[CD];
    menor.distanciaBE = dis[BE];
    menor.distanciaBM = dis[BM];
    return menor;
}

tipoElementoLista *escolheMelhorCaminho(tipoLista *lista) {
    tipoElementoLista *melhorCaminho;
    tipoElementoLista *aux = lista->frente->prox;
    melhorCaminho = aux;
    printf("EscolherMelhorCaminho mapa linhas: %d, mapa colunas: %d, mapa cores: %d\n", melhorCaminho->mapa->nlinhas, melhorCaminho->mapa->ncolunas, melhorCaminho->mapa->ncores);    
    for (aux; aux != NULL; aux = aux->prox) {
        int custo = aux->g + aux->menorDistanciaBorda;
        int menorCusto = melhorCaminho->g + melhorCaminho->menorDistanciaBorda;
        if (custo < menorCusto) {
            melhorCaminho = aux;
            printf("EscolherMelhorCaminho mapa linhas: %d, mapa colunas: %d, mapa cores: %d\n", melhorCaminho->mapa->nlinhas, melhorCaminho->mapa->ncolunas, melhorCaminho->mapa->ncores);    
        }
    }
    //printf("EscolherMelhorCaminho mapa linhas: %d, mapa colunas: %d, mapa cores: %d\n", melhorCaminho->mapa->nlinhas, melhorCaminho->mapa->ncolunas, melhorCaminho->mapa->ncores);    
    return melhorCaminho;
}


int main(int argc, char **argv) {
    int cor;
    mapa m;
    int semente;
    int countPosicoes;
    int testeScan;
    
    if(argc < 4 || argc > 5) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }
    
    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);
    
    countPosicoes = m.nlinhas * m.ncolunas;
    
    if(argc == 5)
    semente = atoi(argv[4]);
    else
    semente = -1;
    gera_mapa(&m, semente);
    mostra_mapa_cor(&m);
    
    //=====================================================================================================//
    
    //Inicializacao da pilha de recibos e fila de clientes	
    tipoLista listaNodos;
    InicLista(&listaNodos);
    
    tipoElementoLista *nodo_atual;
    InsereLista(&listaNodos, NULL, &m, 0, 0, 10000, 0, 0, 0, 0);
    nodo_atual = listaNodos.frente->prox;
    //=====================================================================================================//
    
    //=====================================================================================================//
    int countPassos = 0;
    menorCaminho distanciaBordaCimaDireita;
    menorCaminho distanciaBordaBaixoDireita;
    menorCaminho distanciaBordaBaixoEsquerda;
    menorCaminho distanciaBordaBaixoMeio;
    menorCaminho distancias;
    tipoElementoLista *caminhoEscolhido;
    
    //==========================================Ponto Direita Baixo===========================================================//
    while (!nodo_atual->atingiuBordaCimaDireita || !nodo_atual->atingiuBordaBaixoDireita 
        || !nodo_atual->atingiuBordaBaixoEsquerda || !nodo_atual->atingiuBordaBaixoMeio) {
            criaMatrizAdjacencia(nodo_atual->mapa, countPosicoes, matriz);
            int count = 0;
            tipoElementoLista *teste = listaNodos.frente->prox;
            for (teste; teste != NULL; teste = teste->prox) {
                count++;
            }
            distancias = dijkstra(countPosicoes, matriz, countPosicoes-1, nodo_atual->mapa->ncolunas-1, countPosicoes-nodo_atual->mapa->ncolunas, countPosicoes-(nodo_atual->mapa->ncolunas/2));
            if (!nodo_atual->atingiuBordaBaixoDireita) {
                //distanciaBordaBaixoDireita = dijkstra(countPosicoes, matriz, countPosicoes-1);
                if (distancias.distanciaBD == 0) {
                    nodo_atual->atingiuBordaBaixoDireita++;
                }
                else {
                    adicionaNodo(&distancias, countPosicoes-1, distancias.distanciaBD, nodo_atual, &listaNodos, count);
                }
            }
            if (!nodo_atual->atingiuBordaCimaDireita) {
                //distanciaBordaCimaDireita = dijkstra(countPosicoes, matriz, countPosicoes-1, m.ncolunas-1, countPosicoes-m.ncolunas, countPosicoes-(m.ncolunas/2));
                if (distancias.distanciaCD == 0) {
                    nodo_atual->atingiuBordaCimaDireita++;
                }
                else {
                    adicionaNodo(&distancias, nodo_atual->mapa->ncolunas-1, distancias.distanciaCD, nodo_atual, &listaNodos, count);
                }
            }
            if (!nodo_atual->atingiuBordaBaixoEsquerda) {
                //distanciaBordaBaixoEsquerda = dijkstra(countPosicoes, matriz, countPosicoes-1, m.ncolunas-1, countPosicoes-m.ncolunas, countPosicoes-(m.ncolunas/2));
                if (distancias.distanciaBE == 0) {
                    nodo_atual->atingiuBordaBaixoEsquerda++;
                }
                else {
                    adicionaNodo(&distancias, countPosicoes-nodo_atual->mapa->ncolunas, distancias.distanciaBE, nodo_atual, &listaNodos, count);
                }
            }
            if (!nodo_atual->atingiuBordaBaixoMeio) {
                //distanciaBordaBaixoMeio = dijkstra(countPosicoes, matriz, countPosicoes-1, m.ncolunas-1, countPosicoes-m.ncolunas, countPosicoes-(m.ncolunas/2));
                if (distancias.distanciaBM == 0) {
                    nodo_atual->atingiuBordaBaixoMeio++;
                }
                else {
                    adicionaNodo(&distancias, countPosicoes-(nodo_atual->mapa->ncolunas/2), distancias.distanciaBM, nodo_atual, &listaNodos, count);
                }
            }
            if (nodo_atual->quantidadeFilhos > 0) {
                RemoveLista(&listaNodos, nodo_atual);
            }
            caminhoEscolhido = escolheMelhorCaminho(&listaNodos);
            nodo_atual = caminhoEscolhido;
            printf("tamanho lista: %d\n", count);
            countPassos++;
            printf("iteracoes ate agora: %d\n", countPassos);
            printf("passou\n");
            printf("mapa linhas: %d, mapa colunas: %d, mapa cores: %d\n", nodo_atual->mapa->nlinhas, nodo_atual->mapa->ncolunas, nodo_atual->mapa->ncores);
            scanf("%d", &testeScan);
            mostra_mapa_cor(nodo_atual->mapa); // para mostrar sem cores use mostra_mapa(&m);
            //scanf("%d", &cor);
            scanf("%d", &cor);
        }
        
        printf("passos ate now: %d\n", caminhoEscolhido->quantidadePassos);
        //=====================================================================================================//
        
    while(cor > 0 && cor <= m.ncores) {
        pinta_mapa(&m, cor);
        mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
        scanf("%d", &cor);
    }

    return 0;
}
