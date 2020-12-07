// #include "general.h"

// void printTree(){
//     page pag;
//     int i;

//     if((btree = fopen("btree.dat","r+b")) == NULL){
//         printf("Erro ao abrir arquivo de arvore.\n");
//         exit(1);
//     }

//     int rrn_raiz;
//     fread(&rrn_raiz, sizeof(int), 1, btree);
//     printf("\n");

//     while(fread(&pag, sizeof(page), 1, btree) > 0){

//         if(rrn_raiz == pag.RRN){
//            printf("\n- - - - Pagina Raiz - - - -");
//         }

//         printf("\nPagina %d\n",pag.RRN);
//         printf("Chaves: ");
//         for(i = 0; i < pag.num_chaves;i++){
//             printf("%d | ",pag.chave[i]);
//         }
//         printf("\nOffsets: ");

//         for(i = 0; i < pag.num_chaves; i++){
//             printf("%d | ",pag.byteoffset[i]);
//         }

//         printf("\nFilhos: ");

//         for(i = 0; i < pag.num_chaves + 1;i++){
//             printf("%d | ",pag.filho[i]);
//         }
//         printf("\n");
//         if(rrn_raiz == pag.RRN){
//            printf("- - - - - - - - - - - - - - \n");
//         }

//     }
//     fclose(btree);
// }