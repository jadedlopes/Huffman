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

//----------- Private Functions ----------------
static void export_run_tree (FILE* fp, node_t* node);
//----------------------------------------------

fila_t * read_file(char *filename)
{
    int aux;
    int * symbol_array;
    unsigned char temp;
    symbol_t * symbol;
    node_t* node;
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
            node = create_t_node(symbol);
            enqueue(node, fila);
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
    symbol_t* s;
    node_t* n;
    node_t* n2;
    node_t* f;

    while(fila_tamanho(Q)>1){
        s = malloc(sizeof(symbol_t));

        n = dequeue(Q);
        n2 = dequeue(Q);
        s->qty = get_qty(node_get_data(n)) + get_qty(node_get_data(n2));

        f = create_t_node(s);
        tree_add_node(tree, f, n);
        tree_add_node(tree, f, n2);

        enqueue(f, Q);
    }

    n = dequeue(Q);

    tree_add_root(tree, n);

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

void export_huffmanTree (const char* filename, tree_t* t){
    if (filename == NULL || t == NULL){
        perror("Erro huffman -> export_huffmanTree: ponteiros invalidoss");
    }

    FILE* fp = fopen(filename, "w");
    if (fp == NULL){
        perror("huffman -> export_huffmanTree: file open");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "graph {\n");

    export_run_tree(fp, tree_get_root(t));

    fprintf(fp, "}\n");

    fclose(fp);
}

static void export_run_tree (FILE* fp, node_t* node){
    if (node_is_leaf(node)){
        return;
    }
    symbol_t* s, *s1, *s2;
    s = node_get_data(node);
    s1 = node_get_data(node_get_right(node));
    s2 = node_get_data(node_get_left(node));
    if (node_is_leaf(node_get_right(node))){
        fprintf(fp, "\tInterm_%d -- '%c'_%d [label= %d];\n", s->qty, s1->val, s1->qty, 1);
    }else{
        fprintf(fp, "\tInterm_%d -- Interm_%d [label= %d];\n", s->qty, s1->qty, 1);
    }
    if (node_is_leaf(node_get_left(node))){
        fprintf(fp, "\tInterm_%d -- '%c'_%d [label= %d];\n", s->qty, s2->val, s2->qty, 0);
    }else{
        fprintf(fp, "\tInterm_%d -- Interm_%d [label= %d];\n", s->qty, s2->qty, 0);
    }


    export_run_tree (fp, node_get_left(node));
    export_run_tree (fp, node_get_right(node));
}


//char** generate_code_array (tree_t* t){
//}

//void free_huffmanTree(tree_t* t){
//}
