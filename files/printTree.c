#include "general.h"

int getHeight()
{
    int root;
    page pag;
    int i, rrn = 0;
    int height = -1; //inicial
    
    fseek(btree, 0 , SEEK_SET);
    fread(&root, sizeof(int), 1, btree);
    rrn = root;
    while(rrn != -1)
    {
        readPage(rrn, &pag);
        rrn = pag.children[0];
        height++;
    }
    return height;
}

int getInformation(){                   //ler informacoes de cada pagina para obter as informacoes gerais
    page pag;
    int keysCount = 0, pagesCount = 0;

    fseek(btree, sizeof(int), SEEK_SET);
    while(fread(&pag, sizeof(page), 1, btree) > 0)
    {
        pagesCount++;
        keysCount = pag.count_key + keysCount;
    }

    printf("\n\nEstatisticas da Arvore-B: \n");
    printf("* Altura: %d\n", getHeight());
    printf("* Quantidade de chaves: %d\n", keysCount);
    printf("* Quantidade de paginas: %d\n", pagesCount);
    printf("* Taxa de ocupacao: %.2f%%\n", (((float)keysCount) * 100)/(pagesCount * MAXKEYS));
}

void printTree(){
    page pag;
    int i;

    if((btree = fopen("btree.dat","r+b")) == NULL){
        printf("Erro ao abrir arquivo de arvore.\n");
        exit(1);
    }

    int rrn_raiz;
    fread(&rrn_raiz, sizeof(int), 1, btree);
    printf("\n");

    while(fread(&pag, sizeof(page), 1, btree) > 0){
        if(rrn_raiz == pag.RRN){
           printf("\n- - - - Pagina Raiz - - - -");
        }
        printf("\nPagina: %d",pag.RRN);
        printf("\nChaves: ");
        for(i = 0; i < pag.count_key; i++){
            printf("%d | ", pag.keys[i]);
        }
        printf("\nFilhos: ");
        for(i = 0; i < pag.count_key + 1; i++){
            printf("%d | ", pag.children[i]);
        }
        printf("\n");
        if(rrn_raiz == pag.RRN){
           printf("- - - - - - - - - - - - - - \n");
        }

    }
    getInformation();
    printf("\nDigite enter para sair: ");
    getchar();
    fclose(btree);
}