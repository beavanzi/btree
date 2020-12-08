#include "general.h"

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
           printf("\n- - - - Página Raiz - - - -");
        }
        printf("\nPágina: %d",pag.RRN);
        printf("\nChaves: ");
        for(i = 0; i < pag.count_key; i++){
            printf("%d | ", pag.keys[i]);
        }
        // printf("\nOffsets: ");
        // for(i = 0; i < pag.num_chaves; i++){
        //     printf("%d | ",pag.byteoffset[i]);
        // }
        printf("\nFilhos: ");
        for(i = 0; i < pag.count_key + 1; i++){
            printf("%d | ", pag.children[i]);
        }
        printf("\n");
        if(rrn_raiz == pag.RRN){
           printf("- - - - - - - - - - - - - - \n");
        }

    }
    printf("\nDigite enter para sair: ");
    getchar();
    fclose(btree);
}