#include <stdio.h>
#include <stdlib.h>
#include "../header/huffman.h"
#include "../header/binary_tree.h"

int main()
{
    fila_t * fila;
    symbol_t * symbol;
    tree_t* t;
    char code[100] = {0};
    char* code_array [256] = {0};
    int i;

    fila = read_file("bolo.txt");

    t = create_huffmanTree(fila);

    export_huffmanTree("tree.dot", t);

    generate_code_array(tree_get_root(t), code_array, code, 0);

    for (i=0; i<256; i++){
        if(code_array[i])
            printf("%c -- %s\n", i, code_array[i]);
    }

    compress_file("bolo.jc", "bolo.txt", code_array);

    /*while(!fila_vazia(fila)) {
        symbol = node_get_data(dequeue(fila));
        printf("Valor: %c\nQuantidade: %d\n", get_val(symbol), get_qty(symbol));
    }*/

    return 0;
}
