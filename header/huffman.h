#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED
#include "fila.h"
#include "../header/binary_tree.h"

typedef struct symbol symbol_t;

fila_t * read_file(char *filename);

int get_val(symbol_t * symbol);

int get_qty(symbol_t * symbol);

tree_t* create_huffmanTree (fila_t* Q);
int comp_symbol (void* s1, void* s2);
void export_huffmanTree (const char* filename, tree_t* t);

#endif // HUFFMAN_H_INCLUDED
