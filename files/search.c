#include "general.h"

//chave[i], filho.esq = filhos[i], filho.dir = filhos[i+1]

int searchPage(int key, page PAG, int *POS){
    int i = 0;
    while (i < PAG.count_key && key > PAG.keys[i]){
        i++;
    }
    *POS = i;
    if (*POS < PAG.count_key && key == PAG.keys[*POS]){
        return 1;
    } else {
        return 0;
    }
}

int search(int RRN, int key, int *RRN_found, int *pos_found){
    page PAG;
    int POS;
    int found;
    //RRN da pag onde a chave esta, posicao da chave dentro da pagina encontrada
    if (RRN == -1) {
        return -1;
    } else {
        readPage(RRN, &PAG);
        found = searchPage(key, PAG, &POS); // POS recebe a posicao em que key ocorre
        if (found){
            *RRN_found = RRN;
            *pos_found = POS;
            return 1;
        } else {
            return (search(PAG.children[POS], key, RRN_found, pos_found));
        }  
    }

}