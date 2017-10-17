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
typedef struct caminho caminho;

struct caminho {
    int *passos;
    int quantidadePassos;
    int quantidadeFilhos;
    int menorDistanciaBorda;
    int atingiuBordaCimaDireita;
    int atingiuBordaBaixoDireita;
    int atingiuBordaBaixoEsquerda;
    caminho *filho[3];
};


//------------------------------ GRAFO ----------------------------------------------//
typedef struct {
    int distancia;
    int *caminhos;
} menorCaminho;


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

caminho *criaCaminho(int *passos, int quantidadePassos, int menorDistanciaBorda, int bordaCD, int bordaBD, int bordaBE) {
    caminho *new_caminho;
    new_caminho = malloc(sizeof(caminho));
    new_caminho->passos = passos;
    new_caminho->quantidadePassos = quantidadePassos;
    new_caminho->quantidadeFilhos = 0;
    new_caminho->menorDistanciaBorda = menorDistanciaBorda;
    new_caminho->filho[0] = NULL;
    new_caminho->filho[1] = NULL;
    new_caminho->filho[2] = NULL;
    new_caminho->atingiuBordaCimaDireita = bordaCD;
    new_caminho->atingiuBordaBaixoDireita = bordaBD;
    new_caminho->atingiuBordaBaixoEsquerda = bordaBE;
    return new_caminho;
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

menorCaminho dijkstra(int countPosicoes, int matriz[countPosicoes][countPosicoes], int borda) {
    menorCaminho menor;
    menor.caminhos = (int*) malloc(floor(sqrt(countPosicoes)) * sizeof(int));
    int dis[countPosicoes];
    char vis[countPosicoes];
    memset (vis, 0, sizeof (vis));
    memset (dis, 0x7f, sizeof (dis));
    dis[borda] = 0;
    int t, i;
    for (t = 0; t < countPosicoes; t++)
    {
        int v = -1;
        for (i = 0; i < countPosicoes; i++)
            if (!vis[i] && (v < 0 || dis[i] < dis[v]))
                v = i;
        vis[v] = 1;
        for (i = 0; i < countPosicoes; i++)
            if (matriz[v][i] >= 0 && dis[i] > dis[v] + matriz[v][i]) {
                dis[i] = dis[v] + matriz[v][i];
                menor.caminhos[i] = v;
            }
    }
    menor.distancia = dis[0];
    return menor;
}

int min(int distanciaBordaCimaDireita, int distanciaBordaBaixoDireita, int distanciaBordaBaixoEsquerda) {
    if (distanciaBordaCimaDireita == 0) distanciaBordaCimaDireita = 0x7f;
    if (distanciaBordaBaixoDireita == 0) distanciaBordaBaixoDireita = 0x7f;
    if (distanciaBordaBaixoEsquerda == 0) distanciaBordaBaixoEsquerda = 0x7f;

    if (distanciaBordaCimaDireita < distanciaBordaBaixoDireita)
        if (distanciaBordaCimaDireita < distanciaBordaBaixoEsquerda)
            return 0;
        else
            return 2;
    else if (distanciaBordaBaixoDireita < distanciaBordaBaixoEsquerda)
        return 1;
    else if (distanciaBordaBaixoEsquerda != 0x7f)
        return 2;
    return 3;
}

int *getPassos(int *passos, int quantidadePassos, int cor) {
    int *passosFilho = (int*) malloc(quantidadePassos * sizeof(int));
    for (int i = 0; i < quantidadePassos; i++) {
        passosFilho[i] = passos[i];
    }
    passosFilho[quantidadePassos] = cor;
    return passosFilho;
}

void adicionaCaminho(menorCaminho *menorCaminho, caminho *caminho_atual, mapa *m) {
    int posicao = 0;
    int aux = 0;
    int cor = m->mapa[menorCaminho->caminhos[posicao]/m->ncolunas][menorCaminho->caminhos[posicao]%m->ncolunas];
    while (m->mapa[0][0] == cor) {
        aux = menorCaminho->caminhos[posicao];
        cor = m->mapa[menorCaminho->caminhos[aux]/m->ncolunas][menorCaminho->caminhos[aux]%m->ncolunas];
        posicao = aux;
    }
    caminho_atual->filho[caminho_atual->quantidadeFilhos] = criaCaminho(getPassos(caminho_atual->passos, caminho_atual->quantidadePassos, cor),(caminho_atual->quantidadePassos)+1, menorCaminho->distancia-1, caminho_atual->atingiuBordaCimaDireita, caminho_atual->atingiuBordaBaixoDireita, caminho_atual->atingiuBordaBaixoEsquerda);
    caminho_atual->quantidadeFilhos++;
}

caminho *escolheMelhorCaminho(caminho *raiz, caminho *menorCaminho) {
    if (raiz->quantidadeFilhos == 0) {
        int custo = raiz->quantidadePassos + raiz->menorDistanciaBorda;
        int menorCusto = menorCaminho->quantidadePassos + menorCaminho->menorDistanciaBorda;
        if (custo < menorCusto) {
            menorCaminho = raiz;
        }
    }
    else {
        for(int i = 0; i < raiz->quantidadeFilhos; i++) {
            menorCaminho = escolheMelhorCaminho(raiz->filho[i], menorCaminho);
        }
    }
    return menorCaminho;
}

int main(int argc, char **argv) {
    int cor;
    mapa m;
    int semente;
    int countPosicoes;

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
    caminho *raiz,*caminho_atual;
    int *passos = NULL;
    raiz = criaCaminho(passos, 0, 0x7f, 0, 0, 0);
    caminho_atual = raiz;
    int count = 0;
    menorCaminho distanciaBordaCimaDireita;
    menorCaminho distanciaBordaBaixoDireita;
    menorCaminho distanciaBordaBaixoEsquerda;
    int mapaAux[m.nlinhas][m.ncolunas];
    for (int linha = 0; linha < m.nlinhas; linha++) {
        for (int coluna = 0; coluna < m.ncolunas; coluna++) {
            mapaAux[linha][coluna] = m.mapa[linha][coluna];
        }
    }
    while (!caminho_atual->atingiuBordaCimaDireita || !caminho_atual->atingiuBordaBaixoDireita || !caminho_atual->atingiuBordaBaixoEsquerda) {
        for (int linha = 0; linha < m.nlinhas; linha++) {
            for (int coluna = 0; coluna < m.ncolunas; coluna++) {
                m.mapa[linha][coluna] = mapaAux[linha][coluna];
            }
        }

        for (int passo = 0; passo < caminho_atual->quantidadePassos; passo++) {
            pinta_mapa(&m, caminho_atual->passos[passo]);
        }
        
        criaMatrizAdjacencia(&m, countPosicoes, matriz);
        if (!caminho_atual->atingiuBordaCimaDireita) {
            distanciaBordaCimaDireita = dijkstra(countPosicoes, matriz, m.ncolunas-1);
            if (distanciaBordaCimaDireita.distancia == 0) {
                caminho_atual->atingiuBordaCimaDireita = 1;
            }
            else {
                adicionaCaminho(&distanciaBordaCimaDireita, caminho_atual, &m);
            }
        }
        if (!caminho_atual->atingiuBordaBaixoDireita) {         
            distanciaBordaBaixoDireita = dijkstra(countPosicoes, matriz, countPosicoes-1);
            if (distanciaBordaBaixoDireita.distancia == 0) {
                caminho_atual->atingiuBordaBaixoDireita = 1;
            }
            else {
                adicionaCaminho(&distanciaBordaBaixoDireita, caminho_atual, &m);
            }
        }
        if (!caminho_atual->atingiuBordaBaixoEsquerda) {
            distanciaBordaBaixoEsquerda = dijkstra(countPosicoes, matriz, countPosicoes-m.ncolunas);
            if (distanciaBordaBaixoEsquerda.distancia == 0) {
                caminho_atual->atingiuBordaBaixoEsquerda = 1;
            }
            else {
                adicionaCaminho(&distanciaBordaBaixoEsquerda, caminho_atual, &m);
            }
        }
        printf("raiz tamanho filhos: %d\n", caminho_atual->quantidadeFilhos);
        caminho *caminhoEscolhido = escolheMelhorCaminho(raiz, raiz);
        caminho_atual = caminhoEscolhido;
        count++;
        int teste;
        //scanf("%d", &teste);
        printf("passos 0: %d\n", caminhoEscolhido->passos[0]);
        printf("passos 1: %d\n", caminhoEscolhido->passos[1]);
        printf("passos 2: %d\n", caminhoEscolhido->passos[2]);
        printf("passos 3: %d\n", caminhoEscolhido->passos[3]);
        printf("passos 4: %d\n", caminhoEscolhido->passos[4]);
        printf("passos 5: %d\n", caminhoEscolhido->passos[5]);
        printf("passos 6: %d\n", caminhoEscolhido->passos[6]);
        printf("passos 7: %d\n", caminhoEscolhido->passos[7]);
        printf("passos 8: %d\n", caminhoEscolhido->passos[8]);
        printf("passos 9: %d\n", caminhoEscolhido->passos[9]);
        printf("passos 10: %d\n", caminhoEscolhido->passos[10]);
        printf("passos 11: %d\n", caminhoEscolhido->passos[11]);
        printf("passos 12: %d\n", caminhoEscolhido->passos[12]);
        printf("passos 13: %d\n", caminhoEscolhido->passos[13]);
        printf("passos 14: %d\n", caminhoEscolhido->passos[14]);        
        printf("cor escolhida: %d\n", caminhoEscolhido->passos[caminhoEscolhido->quantidadePassos-1]);
        if (caminhoEscolhido->passos[caminhoEscolhido->quantidadePassos-1] == 65)
            caminhoEscolhido->passos[caminhoEscolhido->quantidadePassos-1] = 1;
        pinta_mapa(&m, caminhoEscolhido->passos[caminhoEscolhido->quantidadePassos-1]);
        mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
        printf("%d\n", count); 
        //scanf("%d", &cor);
    }
    printf("passos: %d\n", count);
    printf("passosfinal: %d\n", caminho_atual->quantidadePassos);
    mostra_mapa_cor(&m);
    // printf("distancia baixo esquerda: %d\n", distanciaBordaBaixoEsquerda.distancia);
    // for (int i = 0; i < countPosicoes; i++)
    //     printf("%d:%d\n", i, distanciaBordaBaixoEsquerda.caminhos[i]);

    //printf("min: %d\n", min(0, 2, 3));
    // int escolha = min(distanciaBordaCimaDireita.distancia, distanciaBordaBaixoDireita.distancia, distanciaBordaBaixoEsquerda.distancia));
    // if (escolha == 0) {

    // }
    //     //bordaCimaDireita
    // else if (escolha == 1)
    //     //bordaBaixoDireita
    // else
        //bordaBaixoEsquerda

    //caminho_atual->child[i]=createNode(array[i],(current_node->depth)+1);
    //caminho_atual->children++;

    //int distancia = getDistanciaBordaCimaDireita(&m);
    //printf("distancia: %d\n", distancia);
    
    //===========================================================================
    // fronteira *f = malloc(sizeof(fronteira));
    // f->tamanho = 0;
    // f->elementos = malloc(m.nlinhas * m.ncolunas * (sizeof(posicao)));
    
    // int **mapaAux = (int**) malloc(m.nlinhas * sizeof(int*));
    // for(int i = 0; i < m.nlinhas; i++) 
    // mapaAux[i] = (int*) malloc(m.ncolunas * sizeof(int));
    
    // getFronteira(f, &m, 0, 0, m.mapa[0][0], mapaAux);

    // Matriz de adjacências
    // Se G[i][j] > 0, então há aresta que liga 'i' a 'j' com custo G[i][j].
    //int matriz[countPosicoes][countPosicoes];
    //criaMatrizAdjacencia(&m, countPosicoes, matriz);
    //int distanciaBordaCimaDireita = dijkstra(countPosicoes, matriz, m.ncolunas-1);
    // int distanciaBordaBaixoDireita = dijkstra(countPosicoes, matriz, countPosicoes-1);
    // int distanciaBordaBaixoEsquerda = dijkstra(countPosicoes, matriz, countPosicoes-m.ncolunas);
    // printf("Distancia cima direita: %d\n", distanciaBordaCimaDireita);
    // printf("Distancia baixo direita: %d\n", distanciaBordaBaixoDireita);
    // printf("Distancia baixo esquerda: %d\n", distanciaBordaBaixoEsquerda);
    

    // scanf("%d", &cor);
    // while(cor > 0 && cor <= m.ncores) {
    //     pinta_mapa(&m, cor);
    //     mostra_mapa_cor(&m); // para mostrar sem cores use mostra_mapa(&m);
    //     scanf("%d", &cor);
    // }

    return 0;
}
