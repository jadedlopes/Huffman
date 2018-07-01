#ifndef BINARY_TREE_H_INCLUDED
#define BINARY_TREE_H_INCLUDED

typedef struct tree tree_t;
typedef struct tree_node node_t;

tree_t* create_tree (int (*c)(void*, void*));
node_t* create_t_node(void* data);

void tree_add_node (tree_t* t, node_t* father, node_t* node); // adicionar comparação a arvore
void tree_add_root(tree_t* tree, node_t* node);

void node_set_leaf (node_t* node, int leaf);

node_t* tree_get_root(tree_t* tree);

node_t* node_get_father(node_t* node);
node_t* node_get_right(node_t* node);
node_t* node_get_left(node_t* node);
int node_is_leaf(node_t* node);
void* node_get_data(node_t* node);

int comp_symbol (void* s1, void* s2);

#endif // BINARY_TREE_H_INCLUDED
