#ifndef TREE_H
#define TREE_H

#include "functions.h"

#define INDENT_STEP 6

typedef struct InfoNode{
    char *info;
    struct InfoNode *next;
} InfoNode;

typedef struct TreeNode {
    char *key;
    InfoNode *infos;

    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
    struct TreeNode *prev;
    struct TreeNode *next;
} TreeNode;

typedef struct Tree{
    TreeNode *root;
} Tree;

typedef struct StackNode {
    TreeNode *node;
    int space;
    char L_or_R;
    struct StackNode *next;
} StackNode;

typedef struct Stack {
    StackNode *top;
} Stack;

Err tree_traversal_for_timing(Tree *tree);
Err tree_traversal(Tree *tree);
Err tree_traversal_out_of_range(Tree *tree, char *min, char *max);
Err init_tree(Tree **tree);
Err import_from_file(Tree **tree, const char *filename);
TreeNode *create_node(char *key, char *info, TreeNode *parent);
Err insert_node(Tree *tree, char *key, char *info);
Err delete_node(Tree *tree, TreeNode *node);
void free_infos(InfoNode *infos);
void free_tree(Tree *tree);
InfoNode *copy_infos(InfoNode *infos);
TreeNode *search_node_by_key(Tree *tree, char *key, Err *status);
TreeNode *special_search(Tree *tree, TreeNode *node);
void print_node(TreeNode *node);
Err print_tree_as_tree(Tree *tree);
Err push(Stack *stack, TreeNode *node, int space, char L_or_R);
void pop(Stack *stack, TreeNode **node, int *space, char *L_or_R);
void free_stack(Stack *stack);
Err export_to_dot(Tree *tree, const char *filename);

#endif
