#include "trab2_ia.h"

int main(int argc, char **argv) {
    char lado, lixo, lado_acao, acao;
    int c_tamanho, n_acao, p_bola, p_bola_recebida;
    scanf("%c%d", &lado, &c_tamanho);
    int n_campo[c_tamanho+1];

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
		if (acao == 'f') {
        	scanf("%d", &n_acao);
         	InsereFilosofo(campo, n_acao);
        } else if (acao == 'o') {
        	scanf("%d", &n_acao);
            for (int j=0; j<n_acao; ++j) {
                scanf("%d", &n_campo[j]);
            }
        	MoveBola(campo, &p_bola, n_acao, n_campo);
        }
        ImprimeCampo(campo, c_tamanho);
        int aux, cont_chutes;
        int chutes[c_tamanho];
        //Cria a arvore
        nodo *raiz = novoNodo(0, campo, c_tamanho, acao, n_acao, n_campo);
        int countIteracoes = 2;
		nodo *max;
		max = raiz->filhos[0];
		printf("pre minimax\n");
        miniMax(raiz, countIteracoes, p_bola, lado);
		printf("pos minimax\n");

		// Encontra o filho que possui o maior valor
		for (int i=0; i < raiz->count_filhos; ++i){
			printf("morri\n");
			if (raiz->filhos[i]->val == raiz->val) {
				max = raiz->filhos[i];
			}
		}
		// Se a acao for 'f', chama a funcao de inserir filosofo e printa
		if (max->jogada == 'f') {
       		InsereFilosofoAuto(campo, max->posicao_jogada, lado);
			printf("%c f %d\n", lado, max->posicao_jogada);
		// Se a acao for 'o', chama a funcao de mover bola e printa
		} else if (max->jogada == 'o') {
			printf("%c o", lado);
            for (int i=0; i < max->posicao_jogada; ++i) {
                MoveBolaAuto(campo, &p_bola, max->posicoes_pulos[i]);
				printf(" %d", max->posicoes_pulos[i]);

			}
			printf("\n");
		// Se a acao for 'n', apenas printa
		} else if (max->jogada == 'n') {
			printf("%c n\n", lado);
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
    ImprimeCampo(campo, c_tamanho);
    scanf("%c", &lixo);



    //free(raiz);

    return 0;
}


nodo *novoNodo(int val, char *campo, int c_tamanho, char acao, int n_acao, int *n_campo) {
    nodo *x = calloc(sizeof(nodo) + (c_tamanho-1)*sizeof(nodo*), 1);
    x->campo = calloc(sizeof(char) * (c_tamanho+2), 1);
	x->count_filhos = 0;
    x->val = val;
    for (int i = 0; i < c_tamanho+2; ++i) {
        x->campo[i] = campo[i];
    }
    x->c_tamanho = c_tamanho;
    x->jogada = acao;
    x->posicao_jogada = n_acao;
    if (acao == 'o') {
        x->posicoes_pulos = calloc(sizeof(int) * (n_acao), 1);
        for (int i = 0; i < n_acao; ++i) {
            x->posicoes_pulos[i] = n_campo[i];
        }
    }
    return x;
}


void ImprimeCampo (char *campo, int c_tamanho) {
    for (int i=0; i<=c_tamanho+1; ++i) {
        printf("%c", campo[i]);
    }
    printf("\n");
}

void InsereFilosofo (char *campo, int n_campo) {
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

void MoveBola (char *campo, int *p_bola, int n_acao, int *n_campo) {
    for (int j=0; j<n_acao; ++j) {
        if (n_campo[j] > *p_bola) {
            for (int k=*p_bola; k<n_campo[j]; ++k) {
                campo[k] = '.';   
            }
        } else {
            for (int k=*p_bola; k>n_campo[j]; --k) {
                campo[k] = '.';   
            }
        }
        campo[n_campo[j]] = 'o';
        *p_bola = n_campo[j];
    }
}

int miniMax(nodo *pai, int countIteracoes, int p_bola, char lado) {
	printf ("dea\n");
    if (countIteracoes == 0) {
        int valor = 0;
        int indiceOposto = 0;
        if (lado == 'e') {
            if (p_bola == 0)
                valor = pai->c_tamanho * pai->c_tamanho * -1;
            else if (p_bola == pai->c_tamanho + 1)
                valor = pai->c_tamanho * pai->c_tamanho;
            else {
                /* DA PRA JUNTAR EM UM FOR SO*/
                for (int i = 1; i < p_bola-1; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * p_bola) - i; //p_bola - i + p_bola;
                        if (indiceOposto > pai->c_tamanho)
                            valor += i * -1;
                        else
                            valor += i * indiceOposto * -1;
                    }
                }
                if (pai->campo[p_bola-1] == 'f') {
                    indiceOposto = (2 * p_bola) - p_bola - 1; //p_bola - i + p_bola;
                    if (indiceOposto > pai->c_tamanho)
                        valor += p_bola - 1 * -1 * pai->c_tamanho;
                    else
                        valor += p_bola - 1 * indiceOposto * -1 * pai->c_tamanho;
                }
                for (int i = p_bola + 1; i <= pai->c_tamanho; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * p_bola) - i; // p_bola - (i - p_bola)
                        if (indiceOposto < 1)
                            valor += i;
                        else
                            valor += i * indiceOposto;
                    }
                }
            }
        } else {
            if (p_bola == 0)
                valor = pai->c_tamanho * pai->c_tamanho;
            else if (p_bola == pai->c_tamanho + 1)
                valor = pai->c_tamanho * pai->c_tamanho * -1;
            else {
                /* DA PRA JUNTAR EM UM FOR SO*/
                for (int i = 1; i < p_bola; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * p_bola) - i; //p_bola - i + p_bola;
                        if (indiceOposto > pai->c_tamanho)
                            valor += i;
                        else
                            valor += i * indiceOposto;
                    }
                }
                if (pai->campo[p_bola+1] == 'f') {
                    indiceOposto = (2 * p_bola) - p_bola + 1; // p_bola - (i - p_bola)
                    if (indiceOposto < 1)
                        valor += p_bola - 1 * -1 * pai->c_tamanho;
                    else
                        valor += p_bola - 1 * indiceOposto * -1 * pai->c_tamanho;
                }
                for (int i = p_bola + 2; i <= pai->c_tamanho; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * p_bola) - i; // p_bola - (i - p_bola)
                        if (indiceOposto < 1)
                            valor += i * -1;
                        else
                            valor += i * indiceOposto * -1;
                    }
                }
            }
        }
        return valor;
    }
    countIteracoes--;
    int countFilhos = 0;
    int valor;
    nodo *filho;
    for (int i = 1; i <= pai->c_tamanho; ++i) {
        if (pai->campo[i] == '.') {
            filho = novoNodo(0, pai->campo, pai->c_tamanho, pai->jogada, pai->posicao_jogada, pai->posicoes_pulos);
            filho->campo[i] = 'f';
            pai->filhos[countFilhos] = filho;
            valor = miniMax(filho, countIteracoes, p_bola, lado);
            countFilhos++;
            if (countIteracoes % 2 == 0) { //MINI
                if (pai->val > valor)
                    pai->val = valor;
            } else { //MAX
                if (pai->val < valor)
                    pai->val = valor;
            }
        }
    }

    //ve a esquerda da bola
    int aux = p_bola-1;
    if (aux > 0) {
        if (pai->campo[aux] == 'f') {
            do {
                aux -= 1;
                while (pai->campo[aux] != '.' && pai->campo[aux] != 'v')
                    aux -= 1;
                filho = novoNodo(0, pai->campo, pai->c_tamanho, pai->jogada, pai->posicao_jogada, pai->posicoes_pulos);
                MoveBolaAuto(filho->campo, &p_bola, aux); //CUIDAR PRA VER SE A POSICAO DA BOLA FICA CERTO PRA CADA NODO DA ARVORE
                pai->filhos[countFilhos] = filho;
                valor = miniMax(filho, countIteracoes, p_bola, lado);
                if (countIteracoes % 2 == 0) { //MINI
                    if (pai->val > valor)
                        pai->val = valor;
                } else { //MAX
                    if (pai->val < valor)
                        pai->val = valor;
                }
                countFilhos++;
            } while (pai->campo[aux-1] == 'f');
        }
    }
    //ve a direita da bola
    aux = p_bola+1;
    if (aux <= pai->c_tamanho) {
        if (pai->campo[aux] == 'f') {
            do {
                aux += 1;
                while (pai->campo[aux] != '.' && pai->campo[aux] != 'v')
                    aux += 1;
                filho = novoNodo(0, pai->campo, pai->c_tamanho, pai->jogada, pai->posicao_jogada, pai->posicoes_pulos);
                MoveBolaAuto(filho->campo, &p_bola, aux); //CUIDAR PRA VER SE A POSICAO DA BOLA FICA CERTO PRA CADA NODO DA ARVORE
                pai->filhos[countFilhos] = filho;
                valor = miniMax(filho, countIteracoes, p_bola, lado);
                if (countIteracoes % 2 == 0) { //MINI
                    if (pai->val > valor)
                        pai->val = valor;
                } else { //MAX
                    if (pai->val < valor)
                        pai->val = valor;
                }
                countFilhos++;
            } while (pai->campo[aux+1] == 'f');
        }
    }
	pai->count_filhos = countFilhos;

    return pai->val;
}
