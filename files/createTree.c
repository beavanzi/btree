#include "general.h"

int readKey(FILE *keys_file){
    int key;
    fscanf(keys_file, "%d", &key);
    return key;
}

int RRN_newPage(){ 
    int pageLength = sizeof(page);
    int headerLength = sizeof(int);
    fseek(btree, 0, SEEK_END);
    int byteoffset = ftell(btree);
    return ((byteoffset - headerLength) / pageLength);
}

void cleanPage(page *pag, int max){
	int i;
    pag->count_key = 0;
    for(i = 0; i < (max+1); i++){
        pag->keys[i] = 0;
        pag->children[i] = -1;
    }
}

void createPage(page* pag){
    cleanPage(pag, MAXKEYS);
   // pag->children[MAXKEYS+1] = -1;
    pag->RRN = RRN_newPage();
}

void createTree(char *argv){
    FILE *keys_file;
    page new_page;
    int root = 0;

    if ((btree = fopen("btree.dat", "r+b")) == NULL) {
        btree = fopen("btree.dat", "w+b");
        fwrite(&root, sizeof(int), 1, btree);
        createPage(&new_page);
        fwrite(&new_page, sizeof(page), 1, btree);
    } else {
        fread(&root, sizeof(int), 1, btree);
    }

    keys_file = fopen(argv, "r");
    int key = readKey(keys_file); 
    
    while(key != EOF){
        
        insertKey(root, key, &new_page);
        // printf("\n---NOVA INSERCAO---\n");
        

        // if(insert(root, key, &right_child_promote, &key_promote)){ //Quando troca a raiz
        //     createPage(&new_page);
        //     printf("Nova raiz: %d", new_page.RRN);
        //     new_page.keys[0] = key_promote;
        //    // new_page.byteoffset[0] = offset_pro;
        //     new_page.children[0] = root;
        //     new_page.children[1] = right_child_promote;
        //     new_page.count_key++;
        //     writePage(new_page.RRN, &new_page);
        //     root = new_page.RRN;
        // }
        //offset_reg = offset_reg + tam_reg + sizeof(short int);
        //num_reg--;
        // strcpy(buffer,"");
        //tam_reg = get_rec(keys_file, buffer, 256);
        //char_id = strtok(buffer, "|");
        // chave = atoi(char_id);
        key = readKey(keys_file);
    }
    
    fseek(btree, 0, SEEK_SET);
    fwrite(&root, sizeof(int), 1, btree);

    fclose(btree);
    fclose(keys_file);
}
