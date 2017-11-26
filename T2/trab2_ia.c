#include "trab2_ia.h"

int main(int argc, char **argv) {
    char lado, lixo, lado_acao, acao;
    int c_tamanho, n_acao, p_bola;
	// Leitura do lado e do tamanho do campo
    scanf("%c%d", &lado, &c_tamanho);

    int n_campo[c_tamanho+1];
    char campo[c_tamanho+2];
	// Valoramos 'v' para a posicao dos gols
    campo[0] = 'v';
    campo[c_tamanho+1] = 'v';
    scanf("%c", &lixo);
	// Leitura do campo e atribuicao da posicao da bola
    for (int i=1; i<=c_tamanho; ++i) {
        scanf("%c", &campo[i]);
        if (campo[i] == 'o')
            p_bola = i;
    }

    scanf("%c", &lixo);
	// Enquanto nao ocorrer o gol, executa o algoritmo
    while (campo[0] == 'v' && campo[c_tamanho+1] == 'v') {
		// leitura da acao do oponente
        scanf("%c%c", &lado_acao, &lixo);
        scanf("%c", &acao);
		if (acao == 'f') {
        	scanf("%d", &n_acao);
        } else if (acao == 'o') {
        	scanf("%d", &n_acao);
            for (int j=0; j<n_acao; ++j) {
                scanf("%d", &n_campo[j]);
            }
        } else if (acao == 'n' && p_bola == -1) { //Acaba o jogo
            return 0;
        }
        //Cria a arvore
        nodo *raiz = novoNodo(0, campo, c_tamanho, acao, n_acao, n_campo, p_bola);
        int countIteracoes = 2;
		nodo *max;
		max = raiz->filhos[0];
        int valor = miniMax(raiz, countIteracoes, lado);

		// Encontra o filho que possui o maior valor
		for (int i=0; i < raiz->count_filhos; ++i){
			if (raiz->filhos[i]->val == valor) {
				max = raiz->filhos[i];
			}
		}
		// Se a acao for 'f', chama a funcao de inserir filosofo e printa
		if (max->jogada == 'f') {
			printf("%c f %d\n", lado, max->posicao_jogada);
		// Se a acao for 'o', chama a funcao de mover bola e printa
		} else if (max->jogada == 'o') {
			printf("%c o %d", lado, max->posicao_jogada);
            for (int i=0; i < max->posicao_jogada; ++i) {
				printf(" %d", max->posicoes_pulos[i]);
			}
			printf("\n");
		// Se a acao for 'n', apenas printa
		} else if (max->jogada == 'n') {
			printf("%c n\n", lado);
		}
    	scanf("%c", &lixo);
		// Nova leitura do campo
        scanf("%c%d", &lado, &c_tamanho);
        scanf("%c", &lixo);
		// p_bola recebe -1 quando nao eh encontrado no campo lido
        p_bola = -1;
    	for (int i=1; i<=c_tamanho; ++i) {
        	scanf("%c", &campo[i]);
        	if (campo[i] == 'o')
            	p_bola = i;
    	}
    	scanf("%c", &lixo);
    }
    scanf("%c", &lixo);

    return 0;
}


nodo *novoNodo(int val, char *campo, int c_tamanho, char acao, int n_acao, int *n_campo, int p_bola) {
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
    x->p_bola = p_bola;
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

int miniMax(nodo *pai, int countIteracoes, char lado) {
    if (countIteracoes == 0) {
		// Valoracao do campo
        int valor = 0;
        int indiceOposto = 0;
		// Acoes quando o algoritmo tenta fazer gol no lado direito
        if (lado == 'e') {
			// Status do campo que ocorre o gol contra ele
            if (pai->p_bola == 0)
                valor = pai->c_tamanho * pai->c_tamanho * -99999999;
			// Status do campo que ocorre o gol a favor dele
            else if (pai->p_bola == pai->c_tamanho + 1)
                valor = pai->c_tamanho * pai->c_tamanho * 99999999;
			// Status do campo que nao ocorre gols
            else {
				// Valora posicao da bola
				int meio_do_campo = (pai->c_tamanho/2) + 1;
				if (meio_do_campo > pai->p_bola) {
                   	indiceOposto = (2 * meio_do_campo) - pai->p_bola;
					valor += indiceOposto * pai->p_bola * -1;
				} else if (meio_do_campo < pai->p_bola) {
					indiceOposto = (2 * meio_do_campo) - pai->p_bola;
					valor += indiceOposto * pai->p_bola * pai->c_tamanho * pai->c_tamanho;
				}
				// Valora posicao dos filosofos antes da bola
                for (int i = 1; i < pai->p_bola-1; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * pai->p_bola) - i; //p_bola - i + p_bola;
                        if (indiceOposto > pai->c_tamanho)
                            valor += i * -1;
                        else
                            valor += i * indiceOposto * -1;
                    }
                }
				// Valora posicao do filosofo imediatamente antes da bola
                if (pai->campo[pai->p_bola-1] == 'f') {
                    indiceOposto = (2 * pai->p_bola) - pai->p_bola - 1; //p_bola - i + p_bola;
                    if (indiceOposto > pai->c_tamanho)
                        valor += (pai->p_bola - 1) * -1 * pai->c_tamanho;
                    else
                        valor += (pai->p_bola - 1) * indiceOposto * -1 * pai->c_tamanho;
                }
				// Valora posicao do filosofo imediatamente depois da bola
                if (pai->campo[pai->p_bola+1] == 'f') {
                    indiceOposto = (2 * pai->p_bola) - pai->p_bola + 1; //p_bola - i + p_bola;
                    if (indiceOposto > pai->c_tamanho)
                        valor += (pai->p_bola + 1) * pai->c_tamanho;
                    else
                        valor += (pai->p_bola + 1) * indiceOposto * pai->c_tamanho;
				}
				// Valora posicao dos filosofos depois da bola
                for (int i = pai->p_bola + 2; i <= pai->c_tamanho; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * pai->p_bola) - i; // p_bola - (i - p_bola)
                        if (indiceOposto < 1)
                            valor += i;
                        else
                            valor += i * indiceOposto;
                    }
                }
            }
        } else {
			// Espelhamos os calculos para quando o algoritmo tenta fazer o gol no lado esquerdo
            if (pai->p_bola == 0)
                valor = pai->c_tamanho * pai->c_tamanho * 99999999;
            else if (pai->p_bola == pai->c_tamanho + 1)
                valor = pai->c_tamanho * pai->c_tamanho * -99999999;
            else {
				int meio_do_campo = (pai->c_tamanho/2) + 1;
				if (meio_do_campo < pai->p_bola) {
                   	indiceOposto = (2 * meio_do_campo) - pai->p_bola;
					valor += indiceOposto * pai->p_bola * -1;
				} else if (meio_do_campo > pai->p_bola) {
					indiceOposto = (2 * meio_do_campo) - pai->p_bola;
					valor += indiceOposto * pai->p_bola * pai->c_tamanho * pai->c_tamanho;
				}
                /* DA PRA JUNTAR EM UM FOR SO*/
                for (int i = 1; i < pai->p_bola; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * pai->p_bola) - i; //p_bola - i + p_bola;
                        if (indiceOposto > pai->c_tamanho)
                            valor += i;
                        else
                            valor += i * indiceOposto;
                    }
                }
                if (pai->campo[pai->p_bola-1] == 'f') {
                    indiceOposto = (2 * pai->p_bola) - pai->p_bola - 1; //p_bola - i + p_bola;
                    if (indiceOposto > pai->c_tamanho)
                        valor += (pai->p_bola - 1) * pai->c_tamanho;
                    else
                        valor += (pai->p_bola - 1) * indiceOposto * pai->c_tamanho;
				}
                if (pai->campo[pai->p_bola+1] == 'f') {
                    indiceOposto = (2 * pai->p_bola) - pai->p_bola + 1; // p_bola - (i - p_bola)
                    if (indiceOposto < 1)
                        valor += (pai->p_bola + 1) * -1 * pai->c_tamanho;
                    else
                        valor += (pai->p_bola + 1) * indiceOposto * -1 * pai->c_tamanho;
                }
                for (int i = pai->p_bola + 2; i <= pai->c_tamanho; ++i) {
                    if (pai->campo[i] == 'f') {
                        indiceOposto = (2 * pai->p_bola) - i; // p_bola - (i - p_bola)
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
	// Recursao antes de chegar na folha
    countIteracoes--;
    int countFilhos = 0;
    int valor;
    nodo *filho;
	// Para cada jogada 'f' possivel eh criado um filho
    for (int i = 1; i <= pai->c_tamanho; ++i) {
        if (pai->campo[i] == '.') {
            filho = novoNodo(0, pai->campo, pai->c_tamanho, 'f', i, pai->posicoes_pulos, pai->p_bola);
            filho->campo[i] = 'f';
            pai->filhos[countFilhos] = filho;
            valor = miniMax(filho, countIteracoes, lado);
            countFilhos++;
			filho->val = valor;
        }
    }
	// Para cada jogada 'o' para o lado esquerdo da bola eh criado filhos de 1 ou mais pulos
    int aux = pai->p_bola-1;
    if (aux > 0) {
        if (pai->campo[aux] == 'f') {
            int countPulos = 0;
            int pulos[pai->c_tamanho];
            do {
                aux -= 1;
                while (pai->campo[aux] != '.' && pai->campo[aux] != 'v')
                    aux -= 1;
                pulos[countPulos] = aux;
                countPulos++;
                filho = novoNodo(0, pai->campo, pai->c_tamanho, 'o', countPulos, pulos, pai->p_bola);
                MoveBolaAuto(filho->campo, &filho->p_bola, aux);
                pai->filhos[countFilhos] = filho;
                valor = miniMax(filho, countIteracoes, lado);
				filho->val = valor;
                countFilhos++;
            } while (pai->campo[aux-1] == 'f');
        }
    }
	// Para cada jogada 'o' para o lado direito da bola eh criado filhos de 1 ou mais pulos
    aux = pai->p_bola+1;
    if (aux <= pai->c_tamanho) {
        if (pai->campo[aux] == 'f') {
            int countPulos = 0;
            int pulos[pai->c_tamanho];
            do {
                aux += 1;
                while (pai->campo[aux] != '.' && pai->campo[aux] != 'v')
                    aux += 1;
                pulos[countPulos] = aux;
                countPulos++;
                filho = novoNodo(0, pai->campo, pai->c_tamanho, 'o', countPulos, pulos, pai->p_bola);
                MoveBolaAuto(filho->campo, &filho->p_bola, aux);
                pai->filhos[countFilhos] = filho;
                valor = miniMax(filho, countIteracoes, lado);
				filho->val = valor;
                countFilhos++;
            } while (pai->campo[aux+1] == 'f');
        }
    }
	// Atribuicao minimax
	pai->count_filhos = countFilhos;
    pai->val = pai->filhos[0]->val;
	for (int i = 0; i < countFilhos; ++i) {
    	if (countIteracoes % 2 == 0) { //MINI
        	if (pai->val > pai->filhos[i]->val)
            	pai->val = pai->filhos[i]->val;
    	} else { //MAX
        	if (pai->val < pai->filhos[i]->val)
            	pai->val = pai->filhos[i]->val;
    	}
	}
	return pai->val;
}
