#include <stdio.h>
#include <stdlib.h>
#include "../header/huffman.h"

#define TAM_SYM 256

struct symbol {
    int val;    //  Valor
    int qty;    //  Quantidade
};

fila_t * read_file(char *filename)
{
    int aux;
    int * symbol_array;
    unsigned char temp;
    symbol_t * symbol;
    fila_t * fila = cria_fila();

    FILE *fp = fopen(filename, "rb");   // Leitura binária para pegar todos os símbolos do arquivo sem formatação

    if(fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(-1);
    }

    symbol_array = calloc(TAM_SYM, sizeof(int));   // 256 é a quantidade possível de símbolos, todos inicializados em zero
    if(symbol_array == NULL){
        perror("huffman: read_file - symbol_array calloc: ");
        exit(EXIT_FAILURE);
    }

    while(fread(&temp, sizeof(unsigned char), 1, fp) == 1) {
        symbol_array[temp]++;
    }

    for(aux = 0; aux < TAM_SYM; aux++) {
        if(symbol_array[aux] != 0){
            symbol = malloc(sizeof(symbol_t));
            if(symbol == NULL){
                perror("huffman: read_file - symbol malloc: ");
                exit(EXIT_FAILURE);
            }

            symbol->val = aux;                       // A posição é o símbolo
            symbol->qty = symbol_array[aux];         // Cada posição tem a quantidade de vezes que o símbolo aparece
            enqueue(symbol, fila);
        }
    }

    fclose(fp);
    free(symbol_array);

    return fila;
}

int get_val(symbol_t * symbol)
{
    return symbol->val;
}

int get_qty(symbol_t * symbol)
{
    return symbol->qty;
}
