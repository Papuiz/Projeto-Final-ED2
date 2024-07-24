#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct No {
    int chave;
    int altDireita;
    int altEsquerda;
    struct No* direita;
    struct No* esquerda;
} Nodo;

int comparacoes_globais = 0;

Nodo* criarNodo(int chave);
Nodo* balanceamento(Nodo* nodo);
Nodo* rotacaoEsquerda(Nodo* nodo);
Nodo* rotacaoDireita(Nodo* nodo);
Nodo* inserir(Nodo* raiz, int chave);
int buscar(Nodo* raiz, int chave);
void desalocarArvore(Nodo** raiz);

int main() {
    clock_t ticks[2];
    ticks[0] = clock();

    Nodo* raiz = NULL;
    char opcao;
    int chave;

    FILE* arq = fopen("in1.txt", "r"); 								//Renomear arquivo ou alterar qual arquivo abrir para escolher
    if (arq == NULL) {
        printf("Erro ao abrir arquivo de entrada");
        return 0;
    }

    while (!feof(arq)) {
        fscanf(arq, "%c %d\n", &opcao, &chave);
        if (opcao == 'i') {
            raiz = inserir(raiz, chave);
        } else if (opcao == 'b') {
            buscar(raiz, chave);  
        }
    }

    fclose(arq);
    arq=NULL;
    desalocarArvore(&raiz);
    ticks[1] = clock();
    
    double tempo = (ticks[1] - ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

    printf("Foram feitas %d comparacoes, com %g ms. de tempo gasto\n", comparacoes_globais, tempo);
    return 0;
}

Nodo* criarNodo(int chave) {
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->chave = chave;
    nodo->altDireita = 0;
    nodo->altEsquerda = 0;
    nodo->direita = NULL;
    nodo->esquerda = NULL;
    return nodo;
}

Nodo* balanceamento(Nodo* nodo) {
    if (nodo == NULL) {
        return NULL;
    } else {
        int fdX = nodo->altDireita - nodo->altEsquerda;

        if (fdX == 2) {
            int dfY = nodo->direita->altDireita - nodo->direita->altEsquerda;

            if (dfY >= 0) {
                nodo = rotacaoEsquerda(nodo);
            } else {
                nodo->direita = rotacaoDireita(nodo->direita);
                nodo = rotacaoEsquerda(nodo);
            }
        } else if (fdX == -2) {
            int dfY = nodo->esquerda->altDireita - nodo->esquerda->altEsquerda;

            if (dfY <= 0) {
                nodo = rotacaoDireita(nodo);
            } else {
                nodo->esquerda = rotacaoEsquerda(nodo->esquerda);
                nodo = rotacaoDireita(nodo);
            }
        }
        return nodo;
    }
}

Nodo* rotacaoEsquerda(Nodo* nodo) {
    Nodo* aux1, * aux2;
    aux1 = nodo->direita;
    aux2 = aux1->esquerda;
    nodo->direita = aux2;
    aux1->esquerda = nodo;

    if (nodo->direita == NULL) {
        nodo->altDireita = 0;
    } else if (nodo->direita->altEsquerda > nodo->direita->altDireita) {
        nodo->altDireita = nodo->direita->altEsquerda + 1;
    } else {
        nodo->altDireita = nodo->direita->altDireita + 1;
    }

    if (aux1->esquerda->altEsquerda > aux1->esquerda->altDireita) {
        aux1->altEsquerda = aux1->esquerda->altEsquerda + 1;
    } else {
        aux1->altEsquerda = aux1->esquerda->altDireita + 1;
    }
    return aux1;
}

Nodo* rotacaoDireita(Nodo* nodo) {
    Nodo* aux1, * aux2;
    aux1 = nodo->esquerda;
    aux2 = aux1->direita;
    nodo->esquerda = aux2;
    aux1->direita = nodo;
    if (nodo->esquerda == NULL) {
        nodo->altEsquerda = 0;
    } else if (nodo->esquerda->altEsquerda > nodo->esquerda->altDireita) {
        nodo->altEsquerda = nodo->esquerda->altEsquerda + 1;
    } else {
        nodo->altEsquerda = nodo->esquerda->altDireita + 1;
    }

    if (aux1->direita->altEsquerda > aux1->direita->altDireita) {
        aux1->altDireita = aux1->direita->altEsquerda + 1;
    } else {
        aux1->altDireita = aux1->direita->altDireita + 1;
    }

    return aux1;
}

Nodo* inserir(Nodo* raiz, int chave) {
    if (raiz == NULL) {
    return criarNodo(chave);
    }
        if (chave < raiz->chave) {
        raiz->esquerda = inserir(raiz->esquerda, chave);
    } else if (chave > raiz->chave) {
        raiz->direita = inserir(raiz->direita, chave);
    } else {
    raiz->direita = inserir(raiz->direita, chave);
}
    if (raiz->esquerda == NULL) {
        raiz->altEsquerda = 0;
    } else {
        if (raiz->esquerda->altEsquerda > raiz->esquerda->altDireita) {
            raiz->altEsquerda = raiz->esquerda->altEsquerda + 1;
        } else {
            raiz->altEsquerda = raiz->esquerda->altDireita + 1;
        }
    }
    if (raiz->direita == NULL) {
        raiz->altDireita = 0;
    } else {
        if (raiz->direita->altEsquerda > raiz->direita->altDireita) {
            raiz->altDireita = raiz->direita->altEsquerda + 1;
        } else {
            raiz->altDireita = raiz->direita->altDireita + 1;
        }
    }
    raiz = balanceamento(raiz);
    return raiz;
}

int buscar(Nodo* raiz, int chave) {
    if (raiz == NULL) {
        return 0;
    }
    comparacoes_globais++;
    if (chave == raiz->chave) {
        return 1;
    } else if (chave < raiz->chave) {
        return buscar(raiz->esquerda, chave);
    } else {
        return buscar(raiz->direita, chave);
    }
}
void desalocarArvore(Nodo** raiz) {
    if (*raiz != NULL) {
        desalocarArvore(&((*raiz)->esquerda));
        desalocarArvore(&((*raiz)->direita));
        free(*raiz);
        *raiz = NULL;
    }
}