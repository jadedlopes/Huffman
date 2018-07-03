#include <stdio.h>
#include <stdlib.h>
#include "../header/huffman.h"
#include "../header/binary_tree.h"

int main()
{
    fila_t * fila;
    symbol_t * symbol;
    tree_t* t;

    fila = read_file("bolo.txt");

    t = create_huffmanTree(fila);

    export_huffmanTree("tree.dot", t);

    /*while(!fila_vazia(fila)) {
        symbol = node_get_data(dequeue(fila));
        printf("Valor: %c\nQuantidade: %d\n", get_val(symbol), get_qty(symbol));
    }*/

    return 0;
}
