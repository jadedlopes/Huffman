#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED
#include "fila.h"

typedef struct symbol symbol_t;

fila_t * read_file(char *filename);

int get_val(symbol_t * symbol);

int get_qty(symbol_t * symbol);


#endif // HUFFMAN_H_INCLUDED
