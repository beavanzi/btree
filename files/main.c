#include "general.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s -c nome_arquivo ou $ %s -p\n", argv[0], argv[0]);
        exit(1);
    }
    if (strcmp(argv[1], "-p") == 0) {
        printf("Modo de impressao ativado ... \n");
        printTree(argv[2]);
        printf("Impressao realizada com sucesso!\n"); 
    } else if (strcmp(argv[1], "-c") == 0) {
        if (!(argv[2])){
            printf("ERRO: Informe um arquivo para entrada de chaves!");
            exit(1);
        }
        printf("Modo de criacao da arvore-B ativado ... nome do arquivo = %s\n\n", argv[2]);
        createTree(argv[2]);
        printf("Execução finalizada com sucesso!\n");
    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }
    return 0;
}


