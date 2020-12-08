#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE* btree;

typedef struct{
  int num_chaves;
  int RRN;
  int chave[4];
  int byteoffset[4];
  int filho[5];
} pagina;

typedef struct{
    int num_chaves;
    int RRN;
    int chave[5];
    int byteoffset[5];
    int filho[6];
} pagina_aux;

short int get_rec(FILE* fd, char* buffer, int size){

    short int rec_lgth;

    if (fread(&rec_lgth, sizeof(rec_lgth), 1, fd) == 0) // get record length
        return 0;

    if (rec_lgth < size) {
        rec_lgth = fread(buffer, sizeof(char), rec_lgth, fd); // read record
        buffer[rec_lgth] = '\0';
        return rec_lgth;
    }
    else {
        fprintf(stderr, "Buffer overflow\n");
        return 0;
    }
}

// int Procurar_final(){
//     int tamanho, byteoffset = 4;
//     char *flag;
//     char buffer[256];

//     fseek(dados, 0, SEEK_SET);
//     flag = fgets(buffer, 256, dados);
//     while(flag != NULL){
//         byteoffset = byteoffset + sizeof(short int) + strlen(buffer);
//         flag = fgets(buffer, 256, dados);
//     }
//     return byteoffset;
// }


int RRN_novapag(){ //CORRETO
    int tamanhopag = sizeof(pagina);
    int tamanhocab = sizeof(int);
    fseek(btree, 0, SEEK_END);
    int byteoffset = ftell(btree);
    return ((byteoffset - tamanhocab) / tamanhopag);
}

void inicializa_pagina(pagina* pag){
	int i;
    pag->num_chaves = 0;
    for(i = 0; i < 4; i++){
        pag->chave[i] = 0;
        pag->filho[i] = -1;
        pag->byteoffset[i] = -1;
    }
    pag->RRN = RRN_novapag();
    pag->filho[4] = -1;
}

void limpa_pag(pagina* pag){
    int i;
    pag->num_chaves = 0;
    for(i = 0; i < 4; i++){
        pag->chave[i] = 0;
        pag->filho[i] = -1;
        pag->byteoffset[i] = -1;
    }
    pag->filho[4] = -1;
}

void inicializa_pagina_aux(pagina_aux *pag){
    int i;
    pag->num_chaves = 0;
    for(i=0; i < 5; i++){
        pag->chave[i] = 0;
        pag->filho[i] = -1;
        pag->byteoffset[i] = -1;

    }
    pag->filho[5] = -1;
}

void insere_na_pagina(int chave, int byteoffset_reg, int filho_d, pagina *pag){
    int i = pag->num_chaves;
    while((i > 0) && (chave < pag->chave[i - 1])){
        pag->chave[i] = pag->chave[i - 1];
        pag->byteoffset[i] = pag->byteoffset[i - 1];
        pag->filho[i + 1] = pag->filho[i];
        i--;
    }
    pag->num_chaves++;
    pag->chave[i] =  chave;
    pag->byteoffset[i] = byteoffset_reg;
    pag->filho[i + 1] = filho_d;
}

void insere_na_pagina_aux(int chave, int byteoffset_reg, int filho_d, pagina_aux *pag){
    int i = pag->num_chaves;
    while((i > 0) && (chave < pag->chave[i - 1])){
        pag->chave[i] = pag->chave[i - 1];
        pag->filho[i + 1] = pag->filho[i];
        pag->byteoffset[i] = pag->byteoffset[i - 1];
        i--;
    }
    pag->num_chaves++;
    pag->chave[i] =  chave;
    pag->byteoffset[i] =  byteoffset_reg;
    pag->filho[i + 1] = filho_d;
}

void copia_pagina(pagina *pag, pagina_aux *pagaux){
    int i;
    inicializa_pagina_aux(pagaux);
    for(i = 0; i < 4; i++){
        pagaux->chave[i] = pag->chave[i];
        pagaux->byteoffset[i] = pag->byteoffset[i];
        pagaux->filho[i] = pag->filho[i];
    }
    pagaux->filho[i] = pag->filho[i];
    pagaux->RRN  =  pag->RRN;
    pagaux->num_chaves = pag->num_chaves;
}


void divide(int chave, int filho_d, int byteoffset_reg, pagina *pag , int *chave_pro, int *filho_d_pro, int *byte_pro,  pagina *novapag){

    int meio = 2;
    pagina_aux pagaux;

    copia_pagina(pag, &pagaux); //pagaux tem uma posicao a mais

    insere_na_pagina_aux(chave, byteoffset_reg, filho_d, &pagaux);

    inicializa_pagina(novapag); // CRIAR RRN NOVA PAG

    *chave_pro = pagaux.chave[meio];
    *byte_pro = pagaux.byteoffset[meio];
    *filho_d_pro = novapag->RRN;

    //copiar as 2 primeiras chaves da pag aux em pag

    int i = 0;
    limpa_pag(pag);
    while(i < meio){
        pag->chave[i] = pagaux.chave[i];
        pag->filho[i] = pagaux.filho[i];
        pag->byteoffset[i] = pagaux.byteoffset[i];
        pag->num_chaves++;
        i++;
    }

    // i = meio
    // pag: 2 chaves: pos 0 e 1
    pag->filho[i] = pagaux.filho[i];

    //copiar as 2 ultimas chaves da pag aux em novapag
    i = meio + 1;
    while(i < 5){ //num_chaves = 0
        novapag->chave[novapag->num_chaves] = pagaux.chave[i];
        novapag->filho[novapag->num_chaves] = pagaux.filho[i];
        novapag->byteoffset[novapag->num_chaves] = pagaux.byteoffset[i];
        novapag->num_chaves = novapag->num_chaves + 1;
        i++;
    }

    novapag->filho[novapag->num_chaves] = pagaux.filho[i];
}


int busca_na_pagina(int chave, pagina pag, int *pos){ //chave = 61

  int i = 0;
  while ((i < pag.num_chaves) && (chave > pag.chave[i])){
    i++;
  }
  *pos = i;
  if((*pos < pag.num_chaves) && (chave == pag.chave[*pos])){
    return 1; //achou
  } else {
    return 0; //nao achou
  }
}

void le_pagina(int RRN, pagina* pag){
    int byteoffset = (sizeof(pagina) * RRN) + sizeof(int);
    fseek(btree, byteoffset, SEEK_SET);
    fread(pag, sizeof(pagina), 1, btree);
}

void escreve_pagina(int RRN, pagina *pag){
    int byteoffset = (sizeof(pagina) * RRN) + sizeof(int);
    fseek(btree, byteoffset, SEEK_SET);
    fwrite(pag, sizeof(pagina), 1, btree);
}

int busca(int RRN, int chave, int *RRN_encontrado, int *pos_encontrada){

  int encontrada, posicao;
  pagina pag;

  if(RRN == -1){
    return -1;
  } else {
   le_pagina(RRN, &pag);
   encontrada = busca_na_pagina(chave, pag, &posicao);
   if(encontrada == 1){
     *RRN_encontrado = RRN;
     *pos_encontrada = posicao;
     return 1;
   } else {
     return(busca(pag.filho[posicao],chave,RRN_encontrado,pos_encontrada));
   }
  }
}

int insercao(int RRN_atual, int chave, int offset_reg, int *offset_filho_pro , int *Filho_D_pro, int *chave_pro){
    pagina pag, nova_pag;

    int pos = -1 , RRN_pro, chv_pro , achou = -1, retorno = -1, offset_pro ;

    if (RRN_atual == -1){
        *chave_pro = chave;
        *Filho_D_pro = -1;
        *offset_filho_pro = offset_reg;
        return 1; // promo
    }

    else{ //busca da chave na pag atual
        le_pagina(RRN_atual, &pag);
        achou = busca_na_pagina(chave, pag, &pos);
    }
    if(achou == 1){
        printf("ERRO: chave duplicada\n");
        return 3; //erro
    }
    retorno = insercao(pag.filho[pos], chave, offset_reg, &offset_pro, &RRN_pro, &chv_pro);

    if(retorno == 2 || retorno == 3){
        return retorno;
    }
    else{//houve promocao
        if(pag.num_chaves < 4){
            insere_na_pagina(chv_pro, offset_pro, RRN_pro, &pag);
            escreve_pagina(RRN_atual, &pag);
            return 2; //sem promo
        }
        else{
            divide(chv_pro, RRN_pro, offset_pro, &pag, chave_pro, Filho_D_pro, offset_filho_pro, &nova_pag);
            escreve_pagina(RRN_atual, &pag);
            escreve_pagina(*Filho_D_pro, &nova_pag);
            return 1; //promo
        }
     }
}


void criar_arvore(char *nome_arquivo){

    pagina novapag; //inicializar a arvore
    int raiz, offset_reg = 4 , offset_pro = -1; //raiz da arvore, byteoffset do reg da primeira pag e byteoffset do filho direito se houver promocao
    int num_reg; //numero de reg do arq dados.dat
    short int tam_reg; //tam de cada reg do arq dados.dat
    char buffer[256]; //buffer para carregar 1 reg de dados.dat
    int chave; //chave a ser inserido na arvore
    char *char_id; //chave em char
    int filho_d_pro = -1, chave_pro = -1; //variaveis de retorno

    if((btree = fopen("btree.dat", "r+b")) != NULL){
        fread(&raiz, sizeof(int), 1, btree);
    }
    else{
        btree = fopen("btree.dat", "w+b");
        raiz = 0;
        fwrite(&raiz, sizeof(int), 1, btree);
        inicializa_pagina(&novapag);
        fwrite(&novapag, sizeof(pagina), 1, btree);
    }
    FILE *dados = fopen(nome_arquivo, "r+b");

    fread(&num_reg, sizeof(int), 1, dados); //num de reg de dados.dat(cabecalho)
    tam_reg = get_rec(dados, buffer, 256); //leitura do primeiro reg de dados.dat

    if(tam_reg <=  0){
        printf("Erro: primeiro registro tem tamanho <= 0 \n");
        exit(1);
    }

    char_id = strtok(buffer, "|");
    chave = atoi(char_id);

    while(num_reg > 0){

        printf("\n");
        printf("\n---NOVA INSERCAO---\n");
        if(insercao(raiz, chave, offset_reg, &offset_pro, &filho_d_pro, &chave_pro) == 1){ //Quando troca a raiz
            inicializa_pagina(&novapag);
            printf("Nova raiz: %d", novapag.RRN);
            novapag.chave[0] = chave_pro;
            novapag.byteoffset[0] = offset_pro;
            novapag.filho[0] = raiz;
            novapag.filho[1] = filho_d_pro;
            novapag.num_chaves++;
            escreve_pagina(novapag.RRN, &novapag);
            raiz = novapag.RRN;
        }
        offset_reg = offset_reg + tam_reg + sizeof(short int);
        num_reg--;
        strcpy(buffer,"");
        tam_reg = get_rec(dados, buffer, 256);
        char_id = strtok(buffer, "|");
        chave = atoi(char_id);
    }
    fseek(btree, 0, SEEK_SET);
    fwrite(&raiz, sizeof(int), 1, btree);

    fclose(btree);
    fclose(dados);
}

void gerenciador_op( char *nome_arquivo, char *arquivo_op){

    FILE* comandos;
    pagina pag, novapag;
    int chave, achou, raiz;
    int RRN_encontrado, pos_encontrada, offset_filho_pro, filho_d_pro, chave_pro;
    short int tam_reg;
    char *chave_reg, *flag, buffer_op[256], reg[256], espaco = ' ';
    char *num_comando, argumento[256];
    long int offset;
    char *copia;
    int bola;

    comandos = fopen(arquivo_op, "r");
    if(comandos == NULL){
        printf("Erro na abertura do arquivo de comandos\n");
        exit(1);
    }else
        printf("Sucesso na abertura do arquivo de comandos\n");

    FILE *dados;
    btree = fopen("btree.dat", "r+b");
    dados = fopen(nome_arquivo, "r+b");

    fread(&raiz, sizeof(int), 1, btree);

    if(btree == NULL || dados == NULL){
        printf("Erro na abertura de arquivos\n");
        exit(1);
    }else printf("Sucesso na abertura dos arquivos\n");

    flag = fgets(buffer_op, 256, comandos);

    while(flag != NULL){
        num_comando = strtok(buffer_op," ");
        copia = strtok(NULL, "\n");
        strcpy(argumento, copia);
        printf("comando: %s, argumento: %s\n", num_comando, argumento);

        if(strcmp(num_comando, "1") == 0){
            chave = atoi(argumento);
            printf("Busca pelo registro de chave %d\n", chave);
            achou = busca(raiz, chave, &RRN_encontrado, &pos_encontrada);
            if(achou == 1){
                fseek(btree, 4 + (RRN_encontrado * sizeof(pagina)) , SEEK_SET);
                fread(&pag, sizeof(pagina), 1, btree);
                fseek(dados, pag.byteoffset[pos_encontrada], SEEK_SET);
                tam_reg = get_rec(dados, reg, 256);
                printf("%s  (%d bytes)\n", reg, tam_reg);
            }
            else
                printf("Erro: registro nao encontrado!\n");
        }

        else if(strcmp(num_comando,"2") == 0){
            tam_reg = strlen(argumento);
            chave_reg = strtok(copia, "|");
            chave = atoi(chave_reg);
            printf("inserindo a chave %d\n",chave);
            printf("offset: %d\n",offset);
            fseek(dados, 0, SEEK_END);
            offset = ftell(dados);
            printf("offset depois: %d\n",offset);
            bola = insercao(raiz, chave, offset, &offset_filho_pro, &filho_d_pro, &chave_pro);
            if(bola == 1){ //Quando troca a raiz
                printf("Entrei no if == 1 com a chave %d\n",chave);
                inicializa_pagina(&novapag);
                printf("Nova raiz: %d", novapag.RRN);
                novapag.chave[0] = chave_pro;
                novapag.byteoffset[0] = offset_filho_pro;
                novapag.filho[0] = raiz;
                novapag.filho[1] = filho_d_pro;
                novapag.num_chaves++;
                escreve_pagina(novapag.RRN, &novapag);
                raiz = novapag.RRN;

                fseek(btree, 0, SEEK_SET);
                fwrite(&raiz, sizeof(int), 1, btree);
            }
            if(bola != 3){
                printf("Entrei no if != 3 com a chave %d\n",chave);
                printf("insercao do registro de chave %d (%d bytes)\n", chave, tam_reg);
                fseek(dados, 0, SEEK_END);
                offset = ftell(dados);
                printf("Offset: %d\n", offset);
                fwrite(&tam_reg, sizeof(short int), 1, dados);
                fwrite(argumento, sizeof(char), tam_reg, dados);
            }
        }
        //strcpy(buffer_op,"");
        flag = fgets(buffer_op, 256, comandos);
    }
    fclose(btree);
    fclose(dados);
}

void imprimir_arvore(){
    pagina pag;
    int i;

    if((btree = fopen("btree.dat","r+b")) == NULL){
        printf("Erro ao abrir a arvore \n");
        exit(1);
    }

    int rrn_raiz;
    fread(&rrn_raiz, sizeof(int), 1, btree);
    printf("\n");

    while(fread(&pag, sizeof(pagina), 1, btree) > 0){

        if(rrn_raiz == pag.RRN){
           printf("\n- - - - Pagina Raiz - - - -");
        }

        printf("\nPagina %d\n",pag.RRN);
        printf("Chaves: ");
        for(i = 0; i < pag.num_chaves;i++){
            printf("%d | ",pag.chave[i]);
        }
        printf("\nOffsets: ");

        for(i = 0; i < pag.num_chaves; i++){
            printf("%d | ",pag.byteoffset[i]);
        }

        printf("\nFilhos: ");

        for(i = 0; i < pag.num_chaves + 1;i++){
            printf("%d | ",pag.filho[i]);
        }
        printf("\n");
        if(rrn_raiz == pag.RRN){
           printf("- - - - - - - - - - - - - - \n");
        }

    }
    fclose(btree);
}

int main(int argc, char **argv) {
    if (argc < 2){
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso: ");
        fprintf(stderr, "$ %s (-c|-e|-p) nome_arquivo\n", argv[0]);
        exit(1);

    }
    if (strcmp(argv[1], "-c") == 0) {
        printf("-> Modo de criacao ativado...nome_arquivo = %s\n", argv[2]);
        criar_arvore(argv[2]);

    } else if (strcmp(argv[1], "-e") == 0) {
        printf("-> Modo de execucao de operacoes ativado ...nome_arquivo = %s nome_arquivo_operacoes = %s\n", argv[2], argv[3]);
        gerenciador_op(argv[2], argv[3]);

    }else if(strcmp(argv[1], "-p") == 0) {
        printf("-> Modo de impressao ativado\n");
        imprimir_arvore();

    } else {
       fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }
    return 0;
}
//att: 10/08 arrumar criar arvore e poder escrever qual arq .dat quer abrir
