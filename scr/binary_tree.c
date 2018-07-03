#include <stdlib.h>
#include <stdio.h>

#include "../header/binary_tree.h"

#define TRUE 1
#define FALSE 0

struct tree {
    node_t* root;
    int (*comp)(void *, void *);
};

struct tree_node {
    node_t* father;
    node_t* right;
    node_t* left;
    void* data;
    int leaf;           //1 - é folha, 0 - não é folha
};

//---------- Private Functions ------------
static void node_set_right (node_t* node, node_t* son);
static void node_set_left (node_t* node, node_t* son);
//------------------------------------------


tree_t* create_tree (int (*c)(void*, void*)){
    tree_t* t = malloc(sizeof(tree_t));
    if (t == NULL){
        printf("Error: binary_tree: create_tree - malloc\n");
        exit(-1);
    }

    t->root = NULL;
    t->comp = c;

    return t;
}

node_t* create_t_node(void* data){
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL){
        perror("Error binary_tree -> create_t_node - malloc\n");
    }

    node->father = NULL;
    node->right = NULL;
    node->left = NULL;
    node->leaf = 1;
    node->data = data;

    return node;
}

void tree_add_node (tree_t* t, node_t* father, node_t* node){
    if(node == NULL || father == NULL || (father->right != NULL && father->left != NULL)){
        perror("Error binary_tree -> tree_add_leaf: ponteiros invalidos");
        exit(EXIT_FAILURE);
    }

    int comp;

    if (father->leaf){
        node_set_right(father, node);
        return;
    }

    if(father->right != NULL){
        comp = t->comp(node_get_data(node), node_get_data(father->right));

        if (comp){
            node_set_left(father, node);
        }else{
            node_t* n = father->right;
            node_set_right(father, node);
            node_set_left(father, n);
        }
    }else{
        comp = t->comp(node_get_data(father->left), node_get_data(node));

        if (comp){
            node_set_right(father, node);
        }else{
            node_t* n = father->left;
            node_set_right(father, n);
            node_set_left(father, node);
        }
    }

    return;
}

void tree_add_root(tree_t* tree, node_t* node){
    if(tree == NULL || node==NULL){
        perror("Error binary_tree -> tree_add_root: ponteiros invalidos");
        exit(EXIT_FAILURE);
    }

    if(tree->root == NULL){
        tree->root = node;

        return;

    }

    if(node->right != NULL && node->left != NULL){
        perror("Erro binary_tree -> tree_add_root: não foi possivel adicionar raizn");
        return;
    }

    int comp;

    if(node->right != NULL){
        comp = tree->comp(node_get_data(tree->root), node_get_data(node->right));

        if (comp){
            node_set_left(node, tree->root);
        }else{
            node_t* n = node->right;
            node_set_right(node, tree->root);
            node_set_left(node, n);
        }
    }else{
        comp = tree->comp(node_get_data(node->left), node_get_data(tree->root));

        if (comp){
            node_set_right(node, tree->root);
        }else{
            node_t* n = node->left;
            node_set_right(node, n);
            node_set_left(node, tree->root);
        }
    }
    tree->root = node;
}

static void node_set_right (node_t* node, node_t* son){
    if(son == NULL || node == NULL){
        perror("Error binary_tree -> node_set_right: ponteiros invalidos");
        exit(EXIT_FAILURE);
    }

    node->right = son;
    son->father = node;
    if (node_is_leaf(node))
        node_set_leaf(node, 0);
}

static void node_set_left (node_t* node, node_t* son){
    if(son == NULL || node == NULL){
        perror("Error binary_tree -> node_set_left: ponteiros invalidos");
        exit(EXIT_FAILURE);
    }

    node->left = son;
    son->father = node;
    if (node_is_leaf(node))
        node_set_leaf(node, 0);
}

void node_set_leaf (node_t* node, int leaf){
    if(node == NULL){
        perror("Error binary_tree -> node_set_leaf: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    node->leaf = leaf;
}

node_t* tree_get_root(tree_t* tree){
    if(tree == NULL){
        perror("Error binary_tree -> tree_get_root: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    return tree->root;
}

node_t* node_get_father(node_t* node){
    if(node == NULL){
        perror("Error binary_tree -> node_get_father: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    return node->father;
}

node_t* node_get_right(node_t* node){
    if(node == NULL){
        perror("Error binary_tree -> node_get_right: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    return node->right;
}

node_t* node_get_left(node_t* node){
    if(node == NULL){
        perror("Error binary_tree -> node_get_left: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    return node->left;
}

int node_is_leaf(node_t* node){
    if(node == NULL){
        perror("Error binary_tree -> node_get_leaf: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    return node->leaf;
}

void* node_get_data(node_t* node){
    if(node == NULL){
        perror("Error binary_tree -> node_get_data: ponteiro invalido");
        exit(EXIT_FAILURE);
    }

    return node->data;
}
