#include <stdlib.h>
#include <stdio.h>

#include "../header/binary_tree.h"

#define TRUE 1
#define FALSE 0

struct tree {
    node_t* root;
};

struct tree_node {
    node_t* father;
    node_t* right;
    node_t* left;
    void* data;
    int leaf;           //1 - é folha, 0 - não é folha
};


tree_t* create_tree (){
    tree_t* t = malloc(sizeof(tree_t));
    if (t == NULL){
        printf("Error: binary_tree: create_tree - malloc\n");
        exit(-1);
    }

    t->root = NULL;

    return t;
}

node_t* create_t_node(){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL){
        perror("Error: binary_tree: create_t_node - malloc\n");
    }

    node->father = NULL;
    node->right = NULL;
    node->left = NULL;
    node->leaf = 0;

    return node;
}
