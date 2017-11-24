#include "trab2_ia.h"

int main(int argc, char **argv) {
    char lado, lixo, lado_acao, acao;
    int c_tamanho, n_acao, p_bola, p_bola_recebida;
    scanf("%c%d", &lado, &c_tamanho);

    //printf("%c %d\n", lado, c_tamanho);

    char campo[c_tamanho+2];
    char campo_recebido[c_tamanho+2];
    campo[0] = 'v';
    campo[c_tamanho+1] = 'v';
    campo_recebido[0] = 'v';
    campo_recebido[c_tamanho] = 'v';
    scanf("%c", &lixo);
    for (int i=1; i<=c_tamanho; ++i) {
        scanf("%c", &campo[i]);
        if (campo[i] == 'o')
            p_bola = i;
    }

    scanf("%c", &lixo);
    while (campo[0] == 'v' && campo[c_tamanho+1] == 'v') {
        scanf("%c%c", &lado_acao, &lixo);
        scanf("%c", &acao);

        switch (acao) {
            case 'f':
                InsereFilosofo(campo);
                break;

            case 'o':
                MoveBola(campo, &p_bola);
                break;
            case 'n':
                break;
        }
        ImprimeCampo(campo, c_tamanho);
        int aux, cont_chutes;
		int chutes[c_tamanho];
        //Lado Esquerdo
        if (lado == 'e') {
            if (campo[p_bola+1] == '.') {
                // printf("primeiro if\n");
                InsereFilosofoAuto(campo, p_bola+1, lado);
            } else if (campo[p_bola-1] == 'f') {
                // printf("segundo if\n");
                aux = p_bola+1;
				cont_chutes = 0;
				chutes[c_tamanho];
                do {
                    aux++;
                    while(campo[aux] != '.' && campo[aux] != 'v')
                        aux++;
                    MoveBolaAuto(campo, &p_bola, aux);
					chutes[cont_chutes] = p_bola;
					cont_chutes++;
                    if (campo[aux] == 'v') break;
                } while (campo[aux+1] == 'f');
				printf("%c o %d", lado, cont_chutes);
				for (int i=0; i<cont_chutes; ++i) {
					printf(" %d", chutes[i]);
				}
				printf("\n");
            } else {
                // printf("terceiro if\n");
                aux = p_bola+1;
                do {
                    aux++;
                    while(campo[aux] != '.' && campo[aux] != 'v')
                        aux++;
                    if (campo[aux] == 'v') {
                        MoveBolaAuto(campo, &p_bola, aux);
						printf("%c o 1 %d\n", lado, p_bola);
                    } else if (campo[aux+1] == '.')
                        InsereFilosofoAuto(campo, aux+1, lado);
                } while (campo[aux+1] == 'f');
            }
        } else if (lado == 'd') { //Lado Direito
            if (campo[p_bola-1] == '.') {
                InsereFilosofoAuto(campo, p_bola-1, lado);
            } else if (campo[p_bola+1] == 'f') {
                aux = p_bola-1;
				cont_chutes = 0;
				chutes[c_tamanho];
                do {
                    aux--;
                    while(campo[aux] != '.' && campo[aux] != 'v')
                        aux--;
                    MoveBolaAuto(campo, &p_bola, aux);
					chutes[cont_chutes] = p_bola;
					cont_chutes++;
                    if (campo[aux] == 'v') break;
                } while (campo[aux-1] == 'f');
				printf("%c o %d", lado, cont_chutes);
				for (int i=0; i<cont_chutes; ++i) {
					printf(" %d", chutes[i]);
				}
				printf("\n");
            } else {
                aux = p_bola-1;
                do {
                    aux--;
                    while(campo[aux] != '.' && campo[aux] != 'v')
                        aux--;
                    if (campo[aux] == 'v') {
                        MoveBolaAuto(campo, &p_bola, aux);
						printf("%c o 1 %d\n", lado, p_bola);
                    } else if (campo[aux-1] == '.')
                        InsereFilosofoAuto(campo, aux-1, lado);
                } while (campo[aux-1] == 'f');
            }
        }
        ImprimeCampo(campo, c_tamanho);
        scanf("%c", &lixo);

    	scanf("%c", &lixo);
		if (lixo != lado) {
			printf("Entrada incorreta! (Esperava ‘%c’ ao inves de ‘%c’)\n", lado, lixo);
			return 0;
		}
    	scanf("%c", &lixo);
    	for (int i=1; i<=c_tamanho; ++i) {
        	scanf("%c", &campo_recebido[i]);
        	if (campo_recebido[i] == 'o')
            	p_bola_recebida = i;
			if (campo_recebido[i] != campo[i]) {
				printf("Entrada incorreta! (Verifique o Campo da entrada)\n");
				return 0;
			}
    	}
    	scanf("%c", &lixo);
    }
        //MINIMAX
        /*
        //Cria a arvore
        nodo *raiz = novoNodo(c_tamanho-1, 0, campo, c_tamanho);
        int countFilhos = 0;
        for (int i = 1; i <= c_tamanho; ++i) {
            if (campo[i] == '.') {
                raiz->filhos[countFilhos] = novoNodo(c_tamanho-1, 0, campo, c_tamanho);
                countFilhos++;
            }
        }
        //ve a esquerda da bola
        int aux = p_bola-1;
        if (campo[aux] == 'f') {
            do {
                aux -= 1;
                while (campo[aux] != '.' && campo[aux] != 'v')
                    aux -= 1;
                if (campo[aux] == 'v') {
                    int valor = 100000;
                    if (lado == 'e')
                        valor *= -1;
                    raiz->filhos[countFilhos] = novoNodo(c_tamanho-1, valor, campo, c_tamanho);
                }
                else { //falta quando tem filosofo do lado
                    int valor = 0;
                    if (lado == 'e') {
                        valor = c_tamanho - aux + 1;
                        valor *= -1;
                    }
                    else {
                        if (aux % 2 == 0)
                            valor = aux;
                        else
                            valor = aux + 1; 
                    }
                    raiz->filhos[countFilhos] = novoNodo(c_tamanho-1, valor, campo, c_tamanho);
                }
                countFilhos++;
            } while (campo[aux-1] == 'f');
        }

        //ve a direita da bola
        aux = p_bola+1;
        if (campo[aux] == 'f') {
            do {
                aux += 1;
                while (campo[aux] != '.' && campo[aux] != 'v')
                    aux += 1;
                if (campo[aux] == 'v') {
                    int valor = 100000;
                    if (lado == 'd')
                        valor *= -1;
                    raiz->filhos[countFilhos] = novoNodo(c_tamanho-1, valor, campo, c_tamanho);
                    countFilhos++;
                }
                else
                    raiz->filhos[countFilhos] = novoNodo(c_tamanho-1, 0, campo, c_tamanho);
                countFilhos++;
            } while (campo[aux+1] == 'f');
        }
        */
    ImprimeCampo(campo, c_tamanho);
    scanf("%c", &lixo);


    //n->filhos[0] = novoNodo(10, 3);
    //n->children[1] is NULL
    //n->filhos[2] = novoNodo(15, 3);

    //free(n->filhos[0]);
    //free(n);

    return 0;
}


nodo *novoNodo(int n, int val, char *campo, int c_tamanho) {
    nodo *x = calloc(sizeof(nodo) + n*sizeof(nodo*), 1);
    x->campo = calloc(sizeof(char) * (c_tamanho+2), 1);
    x->val = val;
    for (int i = 0; i < c_tamanho+2; ++i) {
        x->campo[i] = campo[i];
    }
    x->c_tamanho = c_tamanho;
    return x;
}


void ImprimeCampo (char *campo, int c_tamanho) {
    for (int i=0; i<=c_tamanho+1; ++i) {
        printf("%c", campo[i]);
    }
    printf("\n");
}

void InsereFilosofo (char *campo) {
    int n_campo;
    scanf("%d", &n_campo);
    campo[n_campo] = 'f';
}

void InsereFilosofoAuto (char *campo, int pos, int lado) {
    campo[pos] = 'f';
	printf("%c f %d\n", lado, pos);
}

void MoveBolaAuto (char *campo, int *p_bola, int pos) {
    if (*p_bola < pos) {
        for (int i = *p_bola; i < pos; ++i) {
            campo[i] = '.';
        }
    } else {
        for (int i = *p_bola; i > pos; --i) {
            campo[i] = '.';
        }
    }
    *p_bola = pos;
    campo[pos] = 'o';
}

void MoveBola (char *campo, int *p_bola) {
    int n_acao, n_campo;
    scanf("%d", &n_acao);
    for (int j=0; j<n_acao; ++j) {
        scanf("%d", &n_campo);
        if (n_campo > *p_bola) {
            for (int k=*p_bola; k<n_campo; ++k) {
                campo[k] = '.';   
            }
        } else {
            for (int k=*p_bola; k>n_campo; --k) {
                campo[k] = '.';   
            }
        }
        campo[n_campo] = 'o';
        *p_bola = n_campo;
    }
}
