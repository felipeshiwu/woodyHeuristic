#include "trab2_ia.h"

int main(int argc, char **argv) {
    char lado, lixo, lado_acao, acao;
    int c_tamanho, n_acao, p_bola;
    scanf("%c%d", &lado, &c_tamanho);

    //printf("%c %d\n", lado, c_tamanho);

    char campo[c_tamanho+2];
    campo[0] = 'v';
    campo[c_tamanho+1] = 'v';
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
        scanf("%c", &lixo);
    }

    return 0;
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

void MoveBola (char *campo, int *p_bola) {
    int n_acao, n_campo;
    scanf("%d", &n_acao);
    for (int j=0; j<n_acao; ++j) {
        scanf("%d", &n_campo);
        printf("%d\n", n_campo);
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