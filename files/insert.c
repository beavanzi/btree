#include "general.h"

void readPage(int RRN, page* pag){
    int byteoffset = (sizeof(page) * RRN) + sizeof(int);
    fseek(btree, byteoffset, SEEK_SET);
    fread(pag, sizeof(page), 1, btree);
}

void writePage(int RRN, page *pag){
    int byteoffset = (sizeof(page) * RRN) + sizeof(int);
    fseek(btree, byteoffset, SEEK_SET);
    fwrite(pag, sizeof(page), 1, btree);
}

void createAuxPage(aux_page *pag){
    int i;
    pag->count_key = 0;
    for(i = 0; i < (MAXKEYS+1); i++){
        pag->keys[i] = 0;
        pag->children[i] = -1;
    }
    pag->children[MAXKEYS+1] = -1;
}

void copyPage(page *pag, aux_page *auxp){
    int i;
    createAuxPage(auxp);
    for(i = 0; i < MAXKEYS; i++){
        auxp->keys[i] = pag->keys[i];
         // auxp->byteoffset[i] = pag->byteoffset[i];
        auxp->children[i] = pag->children[i];
    }
    auxp->children[i] = pag->children[i];
    auxp->RRN  =  pag->RRN;
    auxp->count_key = pag->count_key;
}

void insertInPage(int keys, int right_child, page *pag){
    int i = pag->count_key;
    while((i > 0) && (keys < pag->keys[i - 1])){
        pag->keys[i] = pag->keys[i - 1];
      //  pag->byteoffset[i] = pag->byteoffset[i - 1];
        pag->children[i + 1] = pag->children[i];
        i--;
    }
    pag->count_key++;
    pag->keys[i] =  keys;
  //  pag->byteoffset[i] = byteoffset_reg;
    pag->children[i + 1] = right_child;
}
 
void insertInAuxPage(int keys, int right_child, aux_page *pag){
    int i = pag->count_key;
    while((i > 0) && (keys < pag->keys[i - 1])){
        pag->keys[i] = pag->keys[i - 1];
      //  pag->byteoffset[i] = pag->byteoffset[i - 1];
        pag->children[i + 1] = pag->children[i];
        i--;
    }
    pag->count_key++;
    pag->keys[i] =  keys;
  //  pag->byteoffset[i] = byteoffset_reg;
    pag->children[i + 1] = right_child;
}

void insertKey(int *root, int key, page *new_page){
    int right_child_promote, key_promote;

    printf("\nNova insercao: %d\n", key);
        if(insert(*root, key, &right_child_promote, &key_promote)){ //Quando troca a raiz
            createPage(new_page);
            printf("\nNova raiz: %d", new_page->RRN);
            new_page->keys[0] = key_promote;
           // new_page.byteoffset[0] = offset_pro;
            new_page->children[0] = *root;
            new_page->children[1] = right_child_promote;
            new_page->count_key++;
            writePage(new_page->RRN, new_page);
            *root = new_page->RRN;
        }      
}

void slice(int key, int right_child, page *pag , int *key_promote, int *right_child_promote, page *newPage){
    int middle = 2;
    aux_page auxp;

    copyPage(pag, &auxp); //auxp tem uma posicao a mais
    insertInAuxPage(key, right_child, &auxp);
    createPage(newPage); // CRIAR RRN NOVA PAG

    *key_promote = auxp.keys[middle];
    //*byte_pro = auxp.byteoffset[middle];
    *right_child_promote = newPage->RRN;

    //copiar as 2 primeiras chaves da pag aux em pag
    int i = 0;
    cleanPage(pag, MAXKEYS);
    while(i < middle){
        pag->keys[i] = auxp.keys[i];
        pag->children[i] = auxp.children[i];
        //pag->byteoffset[i] = auxp.byteoffset[i];
        pag->count_key++;
        i++;
    }

    // i = middle
    // pag: 2 chaves: pos 0 e 1
    pag->children[i] = auxp.children[i];

    //copiar as 2 ultimas chaves da pag aux em novapag
    i = middle + 1;
    while(i < MAXKEYS+1){ //count_key = 0
        newPage->keys[newPage->count_key] = auxp.keys[i];
        newPage->children[newPage->count_key] = auxp.children[i];
        //newPage->byteoffset[newPage->count_key] = auxp.byteoffset[i];
        newPage->count_key = newPage->count_key + 1;
        i++;
    }

    newPage->children[newPage->count_key] = auxp.children[i];
}

bool insert(int RRN_now,int key, int *right_child_promote, int *key_promote){
    page PAG, newPage;
    int POS = -1, RRN_promote, k_pro; //rrn_pro recebe o valor do rrn da pag promovida para o nivel corrent, k_pro recebe o valor da chave promovida para o nivel corrente
    bool found;
    if (RRN_now == -1){
        *key_promote = key;
        *right_child_promote = -1;
        return true; //promocao
    } else {
        readPage(RRN_now, &PAG); //leia a page armazenada em RRN_now para PAG 
        found = searchPage(key, PAG, &POS);
    }
    if (found) {
        printf("\nChaves duplicadas!\n");
        return false;
    }
    bool ans = insert(PAG.children[POS], key, &RRN_promote, &k_pro);
    if (ans){ //tem promocao
        if (PAG.count_key < MAXKEYS){
            insertInPage(k_pro, RRN_promote, &PAG);             //insira k_pro e rrn_promote (chave promovida e filha) em PAG
            writePage(RRN_now, &PAG);   //escreva PAG no arquivo em rrn_now 
            return false; //nao promove
        } else {
            slice(k_pro, RRN_promote, &PAG, key_promote, right_child_promote, &newPage);
            writePage(RRN_now, &PAG); //escreve PAG no arquivo em rrn_now
            writePage(*right_child_promote, &newPage); //escreva newPage no arquivo em right_child_promote
            return true; //promove
        }
    }else{
        return ans;
    }
} 