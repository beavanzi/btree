#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define MAXKEYS 4 //maxkeys+1 = ordem

typedef struct {
    int count_key; //n de chaves na pag
    int RRN;
    int children[MAXKEYS+1]; //RRNs dos filhos
    int keys[MAXKEYS]; //vetor que armazena as chaves
} page;

typedef struct{
    int count_key;
    int RRN;
    int children[MAXKEYS+2];
    int keys[MAXKEYS+1];
} aux_page;


FILE *btree;

void createTree(char *argv);
void readPage(int RRN, page* pag);
bool searchPage(int key, page PAG, int *POS);
void createPage(page* pag);
void cleanPage(page *pag, int max);
bool insert(int RRN_now,int key, int *right_child_promote, int *key_promote );
void insertKey(int *root, int key, page *new_page);
void printTree();