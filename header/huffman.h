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
void generate_code_array (node_t* node, char** code_array, char* symbol_code, int aux);

void free_huffmanNode(node_t *n);
void free_huffmanTree(tree_t* t);
void compress_file (char* outputFile, char* inputFile , char** codes);

#endif // HUFFMAN_H_INCLUDED
