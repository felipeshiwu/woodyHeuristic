#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXV 100

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
struct caminho {
    int *passos;
    int quantidadePassos;
    struct caminho *filho;
    struct caminho *proximoIrmao;
};

typedef struct caminho caminho;

//------------------------------ GRAFO ----------------------------------------------//



void gera_mapa(mapa *m, int semente) {
    int i, j;

    if(semente < 0)
        srand(time(NULL));  
    else
        srand(semente);  
    m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
    for(i = 0; i < m->nlinhas; i++) {
        m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
        for(j = 0; j < m->ncolunas; j++)
            m->mapa[i][j] = 1 + rand() % m->ncores;
    }
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

void mostra_mapa(mapa *m) {
    int i, j;

    printf("%d %d %d\n", m->nlinhas, m->ncolunas, m->ncores);
    for(i = 0; i < m->nlinhas; i++) {
        for(j = 0; j < m->ncolunas; j++)
            if(m->ncores > 10)
                printf("%02d ", m->mapa[i][j]);
            else
                printf("%d ", m->mapa[i][j]);
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
                printf("%s%02d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
            else
                printf("%s%d%s ", cor_ansi[m->mapa[i][j]], m->mapa[i][j], cor_ansi[0]);
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

caminho *criaCaminho(int *passos, int quantidadePassos) {
    caminho *new_caminho;
    new_caminho = malloc(sizeof(caminho));
    new_caminho->passos = passos;
    new_caminho->quantidadePassos = quantidadePassos;
    new_caminho->filho = NULL;
    new_caminho->proximoIrmao = NULL;
    return new_caminho;
}

caminho *adicionaProximoIrmao(caminho *c, int *passos, int quantidadePassos) {
    while (c->proximoIrmao)
        c = c->proximoIrmao;

    return (c->proximoIrmao = criaCaminho(passos, quantidadePassos));
}

caminho *adicionaFilho(caminho *c, int *passos, int quantidadePassos) {
    if ( c->filho )
        return adicionaProximoIrmao(c->filho, passos, quantidadePassos);
    else
        return (c->filho = criaCaminho(passos, quantidadePassos));
}


int getDistanciaBordaCimaDireita(mapa *m) {
    fronteira *f = malloc(sizeof(fronteira));
    f->tamanho = 0;
    f->elementos = malloc(m->nlinhas * m->ncolunas * (sizeof(posicao)));

    int **mapaAux = (int**) malloc(m->nlinhas * sizeof(int*));
    for(int i = 0; i < m->nlinhas; i++) 
        mapaAux[i] = (int*) malloc(m->ncolunas * sizeof(int));

    getFronteira(f, m, 0, 0, m->mapa[0][0], mapaAux);
    int menorDistancia = -1;
    for (int i = 0; i < f->tamanho; i++) {
        int distanciaAux = 0;
        if (f->elementos[i].linha == m->mapa[0][m->ncolunas-1])
            distanciaAux = m->ncolunas-1 - f->elementos[i].coluna;
        else 
            distanciaAux = f->elementos[i].linha + m->ncolunas-1 - f->elementos[i].coluna;
        if (menorDistancia < 0 || distanciaAux < menorDistancia)
            menorDistancia = distanciaAux;
    }
    return menorDistancia;
}

int getDistanciaBordaBaixoEsquerda() {

}

int getDistanciaBordaBaixoDireita() {

}

void criaMatrizAdjacencia(mapa *m, int matriz[MAXV][MAXV]) {
    for (int i = 0; i < MAXV; i++)
        for (int j = 0; j < MAXV; j++) {
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

int main(int argc, char **argv) {
    int cor;
    mapa m;
    int semente;

    if(argc < 4 || argc > 5) {
        printf("uso: %s <numero_de_linhas> <numero_de_colunas> <numero_de_cores> [<semente_aleatoria>]\n", argv[0]);
        exit(1);
    }

    m.nlinhas = atoi(argv[1]);
    m.ncolunas = atoi(argv[2]);
    m.ncores = atoi(argv[3]);

    if(argc == 5)
        semente = atoi(argv[4]);
    else
        semente = -1;
    gera_mapa(&m, semente);
    mostra_mapa_cor(&m); 

    //=====================================================================================================//
    caminho *raiz,*caminho_atual;
    int *passos;
    raiz = criaCaminho(passos, 0);
    caminho_atual = raiz;
    //caminho_atual->child[i]=createNode(array[i],(current_node->depth)+1);
    //caminho_atual->children++;

    //int distancia = getDistanciaBordaCimaDireita(&m);
    //printf("distancia: %d\n", distancia);
    
    //===========================================================================
    fronteira *f = malloc(sizeof(fronteira));
    f->tamanho = 0;
    f->elementos = malloc(m.nlinhas * m.ncolunas * (sizeof(posicao)));
    
    int **mapaAux = (int**) malloc(m.nlinhas * sizeof(int*));
    for(int i = 0; i < m.nlinhas; i++) 
    mapaAux[i] = (int*) malloc(m.ncolunas * sizeof(int));
    
    getFronteira(f, &m, 0, 0, m.mapa[0][0], mapaAux);

    // Matriz de adjacências
    // Se G[i][j] > 0, então há aresta que liga 'i' a 'j' com custo G[i][j].
    int matriz[MAXV][MAXV];
    criaMatrizAdjacencia(&m, matriz);
    int V; // quantidade de vértices no grafo G
    // Armazena a distância mínima partindo de um vértice 'Vi' até todos os outros vértices
    // dis[i] representa a menor distância de 'Vi' a 'i'.
    int dis[MAXV];
    char vis[MAXV];
    memset (vis, 0, sizeof (vis));
    memset (dis, 0x7f, sizeof (dis));
    dis[9] = 0;
    
    
    
    
    int t, i;
    for (t = 0; t < MAXV; t++)
    {
        int v = -1;
        for (i = 0; i < MAXV; i++)
            if (!vis[i] && (v < 0 || dis[i] < dis[v]))
                v = i;
        vis[v] = 1;
        for (i = 0; i < MAXV; i++)
            if (matriz[v][i] >= 0 && dis[i] > dis[v] + matriz[v][i])
            dis[i] = dis[v] + matriz[v][i];
    }

    for (int i = 0; i < MAXV; i++)
        printf("%d ", dis[i]);



















    //===========================================================================
    

    scanf("%d", &cor);
    while(cor > 0 && cor <= m.ncores) {
        pinta_mapa(&m, cor);
        mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
        scanf("%d", &cor);
    }

    return 0;
}
