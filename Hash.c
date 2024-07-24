#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int tamanho=0, capacidade=100, Ncomp=0, compTotal=0;;

int *alocarTabela();
int funcaoHash(int chave);
void inserir(int **tabelaHash, int chave);
int buscar(int *tabelaHash, int chave);
int calcFator();
void dobrarTamanho(int **tabelaHash);
void desalocar(int **tabelaHash, FILE **arq);

int main(){
    clock_t ticks[2];
    ticks[0]=clock();

    int *tabelaHash = alocarTabela();
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

    desalocar(&tabelaHash, &arq);

    ticks[1]=clock();
    double tempo = (ticks[1] - ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    printf("Foram feitas %d comparacoes, com %g ms. de tempo gasto", compTotal, tempo);

    if(tabelaHash == NULL && arq == NULL)
        printf("\nPrograma finalizado com sucesso");
    return 0;
}

int *alocarTabela(){
    int *p=NULL;
    p=(int *)malloc(sizeof(int)*capacidade);
    if(p==NULL){
        return NULL;
    }

    int i=0;
    while(i!=capacidade){
        *(p+i)=-1;
        i++;
    }
    return p;
}

int funcaoHash(int chave){
    chave = abs(chave);
    chave=chave%capacidade;
    return chave;
}

void inserir(int **tabelaHash, int chave){
    if(*tabelaHash==NULL)
        return;

    int indice=funcaoHash(chave);

    while(*(*tabelaHash+indice)!=-1){
        indice=(indice+1)%capacidade;
    }

    *(*tabelaHash+indice)=chave;
    tamanho++;
    
    if(calcFator()==1){
        dobrarTamanho(tabelaHash);
    }
    return;
}

int buscar(int *tabelaHash, int chave){
    if(tabelaHash==NULL)
        return 0;

    int indice=funcaoHash(chave);

    while(*(tabelaHash+indice)!=-1){
        Ncomp++;
        compTotal++;
        if(*(tabelaHash+indice)==chave){
            Ncomp=0;
            return 1;
        }
        indice=(indice+1)%capacidade;
    }
    Ncomp=0;
    return 0;
}

int calcFator(){
    float x=(float)tamanho/(float)capacidade;
    if(x>=(float)0.70){
        return 1;
    }
    else{
        return 0;
    }
}

void dobrarTamanho(int **tabelaHash){
    int antCapac=capacidade;
    capacidade=capacidade*2;
    tamanho=0;
    int *novo=alocarTabela(), i=0;
    while(i!=antCapac){
        if(*(*tabelaHash+i)!=-1)
            inserir(&novo, *(*tabelaHash+i));
        i++;
    }
    free(*tabelaHash);
    *tabelaHash=novo;
}

void desalocar(int **tabelaHash, FILE **arq){
    if(*tabelaHash!=NULL){
        free(*tabelaHash);
        *tabelaHash=NULL;
    }

    if(*arq!=NULL){
        fclose(*arq);
        *arq=NULL;
    }
}