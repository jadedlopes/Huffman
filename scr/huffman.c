#include <stdio.h>
#include <stdlib.h>
#include "../header/huffman.h"
#include "../header/binary_tree.h"

#define TAM_SYM 256
#define TRUE 1
#define FALSE 0

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

tree_t* create_huffmanTree (fila_t* Q){
    if (Q == NULL){
        perror("Erro huffman->create_huffmanTree: ponteiro invalido");
        exit(-1);
    }

    tree_t* tree = create_tree(comp_symbol);
    symbol_t* s1, *s2;
    node_t* n;
    node_t* n2;

    symbol_t* s = malloc(sizeof(symbol_t));

    s1 = dequeue(Q);
    s2 = dequeue(Q);
    s->qty = s1->qty + s2->qty;
    n2 = create_t_node(s1);

    n = create_t_node(s);
    tree_add_root(tree, n);

    tree_add_node(tree, n, n2);

    n2 = create_t_node(s2);

    tree_add_node(tree, n, n2);

    while(!fila_vazia(Q)){
        s = malloc(sizeof(symbol_t));

        s1 = dequeue(Q);
        s2 = node_get_data(tree_get_root(tree));
        s->qty = s1->qty + s2->qty;
        n2 = create_t_node(s1);

        n = create_t_node(s);

        tree_add_root(tree, n);

        tree_add_node(tree, n, n2);
    }

    return tree;
}

int comp_symbol (void* s1, void* s2){
    symbol_t* symb1 = s1;
    symbol_t* symb2 = s2;

    if (symb1->qty >= symb2->qty){
        return TRUE;
    }else{
        return FALSE;
    }
}
