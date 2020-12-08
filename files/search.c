#include "general.h"

//chave[i], filho.esq = filhos[i], filho.dir = filhos[i+1]

//trabalha em duas etapas, alterna entre busca de paginas e busca dentro da pagina

//search(root, key, &RRN_found, &pos_found)

bool searchPage(int key, page PAG, int *POS){
    int i = 0;
    while (i < PAG.count_key && key == PAG.keys[i]){
        i++;
    }
    *POS = i;
    if (*POS < PAG.count_key && key == PAG.keys[*POS]){
        return true;
    } else {
        return false;
    }
}

// int search(int RRN, int key, int *RRN_found, int *pos_found){
//     page PAG;
//     int POS;
//     bool found;
//     //RRN da pag onde a chave esta, posicao da chave dentro da pagina encontrada
//     if (RRN == -1) {
//         return -1;
//     } else {
//         readPage(RRN, &PAG);
//         found = searchPage(key, PAG, &POS); // POS recebe a posicao em que key ocorre
//         if (found){
//             *RRN_found = RRN;
//             *pos_found = POS;
//             return 1;
//         } else {
//             return (search(PAG.children[POS], key, RRN_found, pos_found));
//         }  
//     }

// }