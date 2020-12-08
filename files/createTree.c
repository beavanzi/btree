#include "general.h"

int readKey(FILE *keys_file){
    // int key, ans;
    
    // ans = fscanf(keys_file, "%d", &key);
    
    // if (ans == -1) return -1;

    // return key;

    if(feof(keys_file) != 0){
        return EOF;
    } else {
        char key = fgetc(keys_file);
        char str[12];
        memset(str, '\0', 12);

        while(key != '\n' && key != EOF){
            str[strlen(str)] = key;
            key = getc(keys_file); 
        }
        return atoi(str);
    }
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
    for(i = 0; i < (max); i++){
        pag->keys[i] = 0;
        pag->children[i] = -1;
    }
}

void createPage(page* pag){
    cleanPage(pag, MAXKEYS);
    pag->children[MAXKEYS] = -1;
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
        fseek(btree, 0, SEEK_SET);
        fread(&root, sizeof(int), 1, btree);
    }

    keys_file = fopen(argv, "r");
    int key = readKey(keys_file); 
    
    while(key != -1){
        insertKey(&root, key, &new_page);
        key = readKey(keys_file);
    }
    
    fseek(btree, 0, SEEK_SET);
    fwrite(&root, sizeof(int), 1, btree);

    fclose(btree);
    fclose(keys_file);
}
