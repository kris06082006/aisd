#ifndef LLRB_TREE_H
#define LLRB_TREE_H

#include "functions.h"

#define INDENT_STEP 6

typedef struct InfoNode{
    unsigned int info;
    struct InfoNode *next;
} InfoNode;

typedef enum Color{
    BLACK,
    RED,
} Color;

typedef struct TreeNode{
    char *key;
    InfoNode *infos;

    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;

    Color color;
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

extern TreeNode elist_node;
extern TreeNode *Elist;

Err init_tree(Tree **tree);
Err import_from_file(Tree **tree, const char *filename);
TreeNode *create_node(char *key, unsigned int info, TreeNode *parent);
Err insert_node(Tree *tree, char *key, unsigned int info);
void balance(Tree *tree, TreeNode *node);
TreeNode *change_colors(Tree *tree, TreeNode *node);
TreeNode *left_rotate(Tree *tree, TreeNode *node);
TreeNode *right_rotate(Tree *tree, TreeNode *node);
TreeNode *move_red_right(Tree *tree, TreeNode *node);
TreeNode *move_red_left(Tree *tree, TreeNode *node);
TreeNode *delete_node(Tree *tree, TreeNode *node, const char *key);
TreeNode *delete_min(Tree *tree, TreeNode *node);
void tree_traversal(TreeNode *node);
void tree_traversal_for_timing(TreeNode *node);

void free_infos(InfoNode *infos);
void free_tree(Tree *tree);
InfoNode *copy_infos(InfoNode *infos);
TreeNode *search_node_by_key(Tree *tree, char *key, Err *status);
InfoNode *search_elem_by_num(TreeNode *node, unsigned int num, Err *status);
TreeNode *special_search(Tree *tree, TreeNode *node);
void print_tree(Tree *tree);
void print_node(TreeNode *node);
Err print_tree_as_tree(Tree *tree);
Err export_to_dot(Tree *tree, const char *filename);
Err export_to_dot_recurse(TreeNode *node, FILE *file);

Err push(Stack *stack, TreeNode *node, int space, char L_or_R);
void pop(Stack *stack, TreeNode **node, int *space, char *L_or_R);
void free_stack(Stack *stack);


#endif
