#include "general.h"

//chave[i], filho.esq = filhos[i], filho.dir = filhos[i+1]

int searchPage(int key, page PAG, int *POS){
    int i = 0;
    while (i < PAG.count_key && key == PAG.keys[i]){
        i++;
    }
    *POS = i;
    if (*POS < PAG.count_key && key == PAG.keys[*POS]){
        return 1;
    } else {
        return 0;
    }
}