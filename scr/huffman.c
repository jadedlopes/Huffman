#include <stdio.h>
#include <stdlib.h>
#include "../header/huffman.h"

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

    symbol_array = calloc(sizeof(int), 256);   // 256 é a quantidade possível de símbolos, todos inicializados em zero

    while(fread(&temp, sizeof(unsigned char), 1, fp) == 1) {
        symbol_array[temp]++;

    }

    for(aux = 0; aux < 256; aux++) {
        if(symbol_array[aux] != 0){
            symbol = malloc(sizeof(symbol_t));
            symbol->val = aux;                       // A posição é o símbolo
            symbol->qty = symbol_array[aux];         // Cada posição tem a quantidade de vezes que o símbolo aparece
            enqueue(symbol, fila);
        }
    }

    fclose(fp);

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


