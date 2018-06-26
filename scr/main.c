#include <stdio.h>
#include <stdlib.h>
#include "../header/huffman.h"

int main()
{
    fila_t * fila;
    symbol_t * symbol;

    fila = read_file("bolo.txt");

    while(!fila_vazia(fila)) {
        symbol = dequeue(fila);
        printf("Valor: %d\nQuantidade: %d\n", get_val(symbol), get_qty(symbol));
    }

    return 0;
}
