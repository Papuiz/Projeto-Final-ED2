#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nodoHash{
    int campoChave;
    struct nodoHash *prox;
}No;

int tamanho=0, Ncomp=0, compTotal=0;;

No *alocarTabela();
int funcaoHash(int chave);
void inserir(No **tabelaHash, int chave);
int buscar(No *tabelaHash, int chave);
void desalocarTudo(No **tabelaHash, FILE **arq);
void desalocaorNo(No **nodo);

int main(){
    clock_t ticks[2];
    ticks[0]=clock();
    
    No *tabelaHash = alocarTabela();
    if(tabelaHash==NULL){
        printf("Erro ao criar a tabela");
        return 0;
    }
    char opcao;
    int chave;

    FILE *arq=NULL;
    arq=fopen("in1.txt", "r");                                              //Renomear arquivo ou alterar qual arquivo abrir para escolher
    if(arq==NULL){
        printf("Erro ao abrir arquivo de entrada");
        return 0;
    }

    while(!feof(arq)){
        fscanf(arq, "%c %d", &opcao, &chave);
        if(opcao=='i')
            inserir(&tabelaHash, chave);
        if(opcao=='b')
            buscar(tabelaHash, chave);   
    }
    
    desalocarTudo(&tabelaHash, &arq);

    ticks[1]=clock();
    double tempo = (ticks[1] - ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("\nForam feitas %d comparacoes no total, com %g ms. de tempo gasto", compTotal, tempo);

    if(tabelaHash == NULL && arq == NULL)
        printf("\nPrograma finalizado com sucesso");
    return 0;
}

No *alocarTabela(){
    No *p=NULL;
    p=(No *)malloc(sizeof(No)*100);
    if(p==NULL){
        return NULL;
    }

    int i=0;
    while(i!=100){
        (p+i)->prox=NULL;
        (p+i)->campoChave=-1;
        i++;
    }
    return p;
}

int funcaoHash(int chave){
    chave = abs(chave);
    chave=chave%100;
    return chave;
}

void inserir(No **tabelaHash, int chave){
    if(*tabelaHash==NULL)
        return;

    int indice=funcaoHash(chave);

    if((*tabelaHash+indice)->prox==NULL && (*tabelaHash+indice)->campoChave==-1){
        (*tabelaHash+indice)->campoChave=chave;
        return;
    }

    No *novo=(No *)malloc(sizeof(No)), *aux=NULL;
    if(novo==NULL){
        printf("Erro ao alocar");
        return;
    }
    novo->campoChave=chave;
    novo->prox=NULL;
    aux=(*tabelaHash+indice);

    while(aux->prox!=NULL){
        aux=aux->prox;
    }
    aux->prox=novo;
    return;
}

int buscar(No *tabelaHash, int chave){
    if(tabelaHash==NULL)
        return 0;

    int indice=funcaoHash(chave);
    No *aux=(tabelaHash+indice);

    while(aux!=NULL){
        compTotal++;
        if(aux->campoChave==chave)
            return 1;
        aux=aux->prox;
    }

    return 0;
}

void desalocarTudo(No **tabelaHash, FILE **arq) {
    if(*tabelaHash!=NULL) {
        int i=0;
        No *aux=NULL;
        while(i!=100){
            aux=*tabelaHash+i;
            if (aux->campoChave!=-1){
                desalocaorNo(&(aux->prox));
            }
            i++;
        }
        free(*tabelaHash);
        *tabelaHash = NULL;
    }

    if(*arq!=NULL){
        fclose(*arq);
        *arq=NULL;
    }
}

void desalocaorNo(No **nodo) {
    if(*nodo==NULL) {
        return;
    }

    if((*nodo)->prox!=NULL) {
        desalocaorNo(&((*nodo)->prox));
    }
    free(*nodo);
    *nodo=NULL;
    return;
}