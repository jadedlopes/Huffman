#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/huffman.h"
#include "../header/binary_tree.h"

#define TAM_SYM 256
#define TRUE 1
#define FALSE 0

//#define DEBUG

struct symbol {
    int val;    //  Valor
    int qty;    //  Quantidade
};

//----------- Private Functions ----------------
static void export_run_tree (FILE* fp, node_t* node);
static tree_t* code_to_tree (FILE* fi);
//----------------------------------------------

fila_t * read_file(char *filename)
{
    int aux;
    int * symbol_array;
    unsigned char temp;
    symbol_t * symbol;
    node_t* node;
    fila_t * fila = cria_fila();

    FILE *fp = fopen(filename, "rb");   // Leitura bin�ria para pegar todos os s�mbolos do arquivo sem formata��o

    if(fp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(-1);
    }

    symbol_array = calloc(TAM_SYM, sizeof(int));   // 256 � a quantidade poss�vel de s�mbolos, todos inicializados em zero
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

            symbol->val = aux;                       // A posi��o � o s�mbolo
            symbol->qty = symbol_array[aux];         // Cada posi��o tem a quantidade de vezes que o s�mbolo aparece
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

        #ifdef DEBUG
            printf("s -- %c ; s -- %c\n", get_val(node_get_data(n)), get_val(node_get_data(n2)));
        #endif // DEBUG

        enqueue(f, Q);
    }

    n = dequeue(Q);

    tree_add_root(tree, n);

    libera_fila(Q);

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
        fprintf(fp, "\t\"%p\" -- \"%c\" [label= %d];\n", s, s1->val, 1);
    }else{
        fprintf(fp, "\t\"%p\" -- \"%p\" [label= %d];\n", s, s1, 1);
    }
    if (node_is_leaf(node_get_left(node))){
        fprintf(fp, "\t\"%p\" -- \"%c\" [label= %d];\n", s, s2->val, 0);
    }else{
        fprintf(fp, "\t\"%p\" -- \"%p\" [label= %d];\n", s, s2, 0);
    }


    export_run_tree (fp, node_get_left(node));
    export_run_tree (fp, node_get_right(node));
}


void generate_code_array (node_t* node, char** code_array, char* symbol_code, int aux){
    if (node == NULL){
        return;
    }

    if (node_is_leaf(node)){
        code_array[get_val(node_get_data(node))] = strncpy(malloc(strlen(symbol_code)+1), symbol_code, strlen(symbol_code)+1);
    }

    symbol_code[aux] = '0';
    symbol_code[aux+1] = 0;
    generate_code_array(node_get_left(node), code_array, symbol_code, aux+1);

    symbol_code[aux] = '1';
    symbol_code[aux+1] = 0;
    generate_code_array(node_get_right(node), code_array, symbol_code, aux+1);
}

void compress_file (char* outputFile, char* inputFile , char** codes){
    FILE* fo = fopen(outputFile, "w");
    FILE* fi = fopen(inputFile, "rb");
    if (fo == NULL || fi == NULL){
        perror("huffman -> compress_file: fopen");
        exit(EXIT_FAILURE);
    }

    int c;

    for (c = 0; c< TAM_SYM; c++){
        if (codes[c]){
            fprintf(fo, "%c;%s;", c, codes[c]);
        }
    }

    fprintf(fo, " |");

    while((c = getc(fi))!= EOF){
        fprintf(fo, "%s", codes[c]);
    }

    fclose(fo);
    fclose(fi);

}

static tree_t* code_to_tree (FILE* fi){            // usar em decompress
    char buffer[2] = {0};
    int i = 2;
    char symb_val;
    symbol_t* s;
    node_t* node, *n;
    tree_t* t = create_tree(comp_symbol);

    s = malloc(sizeof(symbol_t));
    if (s == NULL){
        perror("huffman -> code_to_tree: malloc root");
        exit(EXIT_FAILURE);
    }
    s->val = 0;
    node = create_t_node(s);
    tree_add_root(t, node);

    fread(buffer, sizeof(char), 2, fi);

    while(buffer[0] != ' ' || buffer[1] != '|'){

        if (buffer[1] == ';' && i == 2){
            symb_val = buffer[0];
            #ifdef DEBUG
                printf("symb: %c\n", symb_val);
            #endif // DEBUG
            i--;
        }else{
            while (buffer[0] != ';'){
                if (buffer[0] == '0'){
                    if (node_is_leaf(node)){
                        s = malloc(sizeof(symbol_t));
                        if (s == NULL){
                            perror("huffman -> code_to_tree: malloc leaf esq");
                            exit(EXIT_FAILURE);
                        }
                        s->qty = 1;
                        n = create_t_node(s);
                        tree_add_node(t, node, n);

                        node = node_get_right(node);
                    }else if ((n = node_get_left(node)) != NULL){
                        node = n;
                    }else{
                        if (get_qty(node_get_data(node_get_right(node))) == 1){
                            node = node_get_right(node);
                        }else{
                            s = malloc(sizeof(symbol_t));
                            if (s == NULL){
                                perror("huffman -> code_to_tree: malloc esq");
                                exit(EXIT_FAILURE);
                            }
                            s->qty = 1;
                            n = create_t_node(s);
                            tree_add_node(t, node, n);

                            node = node_get_left(node);
                        }
                    }
                }else{
                    if (node_is_leaf(node)){
                        s = malloc(sizeof(symbol_t));
                        if (s == NULL){
                            perror("huffman -> code_to_tree: malloc leaf dir");
                            exit(EXIT_FAILURE);
                        }
                        s->qty = 0;
                        n = create_t_node(s);
                        tree_add_node(t, node, n);

                        node = node_get_right(node);
                    }else if (get_qty(node_get_data(node_get_right(node))) == 0){
                        node = node_get_right(node);
                    }else{
                        s = malloc(sizeof(symbol_t));
                        if (s == NULL){
                            perror("huffman -> code_to_tree: malloc dir");
                            exit(EXIT_FAILURE);
                        }
                        s->qty = 0;
                        n = create_t_node(s);
                        tree_add_node(t, node, n);

                        node = node_get_right(node);
                    }
                }
                buffer[0] = fgetc(fi);
            }
            s = node_get_data(node);
            s->val = symb_val;
            i++;
            node = tree_get_root(t);
        }

        fread(buffer, sizeof(char), i, fi);
    }

    return t;

}

void decompress (char* outputFile, char* inputFile){
    FILE* fo = fopen(outputFile, "w");
    FILE* fi = fopen(inputFile, "rb");
    if (fo == NULL || fi == NULL){
        perror("huffman -> decompress: fopen");
        exit(EXIT_FAILURE);
    }

    node_t* node;
    char c;

    tree_t* t = code_to_tree(fi);   // J� deixa o ponteiro de leitura do arquivo ap�s os c�digos, pronto para decodificar

    node = tree_get_root(t);
    while((c = fgetc(fi)) != EOF){
        if (c == '0'){
            node = node_get_left(node);
        }else{
            node = node_get_right(node);
        }

        if (node_is_leaf(node)){
            fprintf(fo, "%c", get_val(node_get_data(node)));
            node = tree_get_root(t);
        }
    }

    free_huffmanTree(t);
    fclose(fi);
    fclose(fo);
}

void free_huffmanTree(tree_t* t){
    if(t == NULL){
        perror("huffman->free_huffmanTree: ");
        exit(EXIT_FAILURE);
    }

    free_huffmanNode(tree_get_root(t));     // Libera todos os n�s para depois liberar a �rvore
    free(t);


}

void free_huffmanNode(node_t *n){

    if (n == NULL)
        return;

    free_huffmanNode(node_get_left(n));
    free_huffmanNode(node_get_right(n));

    free(node_get_data(n));

    free(n);
}
