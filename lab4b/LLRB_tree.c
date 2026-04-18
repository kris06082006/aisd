
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LLRB_tree.h"

TreeNode elist_node = {    
    .key = NULL,
    .infos = NULL,
    .left = &elist_node,
    .right = &elist_node,
    .parent = NULL,
    .color = BLACK
};

TreeNode *Elist = &elist_node; 
void tree_traversal_for_timing(TreeNode *node){
    if (node == NULL || node == Elist) return;

    tree_traversal_for_timing(node->left);
    tree_traversal_for_timing(node->right);

    InfoNode *info = node->infos;
    while (info){
        info = info->next;
    }
}

void tree_traversal(TreeNode *node){
    if (node == NULL || node == Elist) return;
    
    tree_traversal(node->left);
    tree_traversal(node->right);
    
    if (node->color == RED) printf("Color: RED  , ");
    else printf("Color: BLACK, ");
    printf("Key: %s, Info: ", node->key);
    InfoNode *info = node->infos;
    while (info){
        printf("%u ", info->info);
        info = info->next;
    }
    printf("\n");
}

Err init_tree(Tree **tree){
    if (*tree != NULL) return ERR_INIT;
    *tree = malloc(sizeof(Tree));
    if (*tree == NULL) return ERR_MEM;
    (*tree)->root = NULL;
    return ERR_OK;
}

Err import_from_file(Tree **tree, const char *filename){
    if (*tree != NULL) {free_tree(*tree); *tree = NULL;}

    FILE *file = fopen(filename, "r");
    if (file == NULL) return ERR_OPEN;

    Err status = init_tree(tree);
    if (status != ERR_OK) return status;
    
    char *key = NULL;
    unsigned int info;
    size_t key_len = 0;

    while (getline(&key, &key_len, file) != -1 && fscanf(file, "%u\n", &info) == 1){
        key[strcspn(key, "\n")] = '\0';

        status = insert_node(*tree, key, info);
        if (status != ERR_OK) break;
    }
    if (!feof(file) && status == ERR_OK) status = ERR_READ;

    free(key);

    if (status != ERR_OK) {free_tree(*tree); *tree = NULL;}
    fclose(file);
    return status;
}

TreeNode *create_node(char *key, unsigned int info, TreeNode *parent){
    TreeNode *new_node = malloc(sizeof(TreeNode));
    if (new_node == NULL) return NULL;
    new_node->key = strdup(key); 
    if (new_node->key == NULL) {free(new_node); return NULL;}

    InfoNode *new_info = malloc(sizeof(InfoNode));
    if (new_info == NULL) {free(new_node->key); free(new_node); return NULL;}

    new_info->info = info;
    new_info->next = NULL;

    new_node->infos = new_info;
    new_node->left = Elist;
    new_node->right = Elist;
    new_node->parent = parent;
    new_node->color = RED;
    return new_node;
}

Err insert_node(Tree *tree, char *key, unsigned int info){
    if (!tree) return ERR_NOT_INIT;

    //insert in root
    if (tree->root == NULL){
        tree->root = create_node(key, info, NULL);
        if (tree->root == NULL) return ERR_MEM;
        tree->root->color = BLACK;
        return ERR_OK;
    }
    
    TreeNode *current = tree->root;
    TreeNode *parent = NULL;
    int cmp;
    while (current != Elist){
        cmp = strcmp(key, current->key);
        if (cmp == 0){
            InfoNode *new_info = malloc(sizeof(InfoNode));
            if (new_info == NULL) return ERR_MEM;

            new_info->info = info; 
            new_info->next = NULL;
            InfoNode *tmp = current->infos;
            while (tmp->next != NULL){
                tmp = tmp->next;
            }
            tmp->next = new_info;
            return ERR_OK;
        }
        parent = current;
        if (cmp < 0) current = current->left;
        else current = current->right;
    }

    current = create_node(key, info, parent);
    if (current == NULL) return ERR_MEM;

    if (parent != NULL && cmp < 0){
        parent->left = current;    
    }
    else if (parent != NULL && cmp > 0){
        parent->right = current;
    }
    //balance
    balance(tree, parent);    
    return ERR_OK;
}

	
void balance(Tree *tree, TreeNode *node){
    if (node == NULL) return;
    if (node->right->color == RED && node->left->color != RED) {
        node = left_rotate(tree, node);
        if (node == NULL) {tree->root->color = BLACK; return;}
        balance(tree, node);
    }
    if (node->parent && node->left->color == RED && node->color == RED){
        node = right_rotate(tree, node->parent);
        if (node == NULL) {tree->root->color = BLACK; return;}
        balance(tree, node);
    }
    if (node->left->color == RED && node->right->color == RED){
        node = change_colors(tree, node);
        if (node == NULL) {tree->root->color = BLACK; return;}
        balance(tree, node);
    } 
 
}


TreeNode *change_colors(Tree *tree, TreeNode *node){
    node->color = RED;
    node->left->color = BLACK;
    node->right->color = BLACK;

   if (node->parent == NULL) tree->root = node;
    return node->parent;
}

TreeNode *left_rotate(Tree *tree, TreeNode *node){
    TreeNode *child = node->right;
    TreeNode *child_left = child->left;

    child->color = node->color;
    node->color = RED;

    child->left = node;
    node->right = child_left;
    child_left->parent = node;

    child->parent = node->parent;
    if (node->parent && node->parent->left == node) node->parent->left = child;
    else if (node->parent && node->parent->right == node) node->parent->right = child;
    node->parent = child;

    if (child->parent == NULL) tree->root = child;

    return child;
}

TreeNode *right_rotate(Tree *tree, TreeNode *node){
    TreeNode *child = node->left;
    TreeNode *child_right = child->right;

    child->color = node->color;
    node->color = RED;

    child->right = node;
    node->left = child_right;
    child_right->parent = node;

    child->parent = node->parent;
    if (node->parent && node->parent->left == node) node->parent->left = child;
    else if (node->parent && node->parent->right == node) node->parent->right = child;
    node->parent = child;

    if (child->parent == NULL) tree->root = child;

    return child;
}

TreeNode* move_red_left(Tree *tree, TreeNode *node) {
    change_colors(tree, node);
    if (node->right->left->color == RED) { 
        node->right = right_rotate(tree, node->right);
        node = left_rotate(tree, node);  
        change_colors(tree, node);    
    }
    return node;                       
}

TreeNode* move_red_right(Tree *tree, TreeNode *node) {
    change_colors(tree, node);
    if (node->left->left->color == RED) {
        node = right_rotate(tree, node);
        change_colors(tree, node);
    }
    return node;
}

TreeNode* delete_node(Tree *tree, TreeNode *node, const char *key) {
    if (strcmp(key, node->key) < 0) {
        if (node->left != Elist) {
            if (node->left->color != RED && node->left->left->color != RED)
                node = move_red_left(tree, node);
            node->left = delete_node(tree, node->left, key);
        }
    } else {
        if (node->left->color == RED) {
            node = right_rotate(tree, node);
        }

        if (strcmp(key, node->key) == 0 && node->right == Elist) {
            free(node->key);
            free_infos(node->infos);
            free(node);
            return Elist;
        }

        if (node->right->color != RED && node->right->left->color != RED)
            node = move_red_right(tree, node);

        if (strcmp(key, node->key) == 0) {
           
            TreeNode *min = node->right;
            while (min->left != Elist) min = min->left;

            char *new_key = strdup(min->key);
            if (!new_key) return node;
            InfoNode *new_infos = copy_infos(min->infos);
            if (!new_infos) {
                free(new_key);
                return node;
            }
            free(node->key);
            free_infos(node->infos);
            node->key = new_key;
            node->infos = new_infos;

          
            node->right = delete_min(tree, node->right);
        } else {
            node->right = delete_node(tree, node->right, key);
        }
    }

    balance(tree, node);
    return node;
}

TreeNode *delete_min(Tree *tree, TreeNode *node) {
    if (node->left == Elist) {
        free(node->key);
        free_infos(node->infos);
        free(node);
        return Elist;
    }

    if (node->left->color != RED && node->left->left->color != RED) {
        node = move_red_left(tree, node);
    }

    node->left = delete_min(tree, node->left);
    balance(tree, node);
    return node;
}

InfoNode *copy_infos(InfoNode *infos){
    InfoNode *new_infos = NULL;
    InfoNode *last = NULL;

    while (infos != NULL){
        InfoNode *new_node = malloc(sizeof(InfoNode));
        if (new_node == NULL){
            free_infos(new_infos);
            return NULL;
        }
        new_node->info = infos->info;
        new_node->next = NULL;
        
        if (last == NULL) new_infos = new_node;
        else last->next = new_node;
        last = new_node;   
        
        infos = infos->next;
        
    }
    return new_infos;
} 

void print_node(TreeNode *node){
    if (node == NULL) return;

    if (node->color == RED) printf("Color: RED  , ");
    else printf("Color: BLACK, ");
    printf("Key: %s, ", node->key);
    InfoNode *current = node->infos;
    printf("Info: ");
    while (current != NULL){
        printf("%u ", current->info);
        current = current->next;
    }
}

Err print_tree_as_tree(Tree *tree){
    if (tree == NULL) return ERR_NOT_INIT;
    if (tree->root == NULL) return ERR_EMPTY;

    Stack stack;
    stack.top = NULL;
    push(&stack, tree->root, 0, ' ');
    
    TreeNode *node;
    int space;
    char L_or_R;
    Err status = ERR_OK;

    while (stack.top){
        pop(&stack, &node, &space, &L_or_R);
        for (int i = 0; i < space; i++) printf(" ");
        printf("--> ");
        if (L_or_R == 'L') printf("[L] ");
        else if(L_or_R == 'R') printf("[R] ");
        print_node(node);
        printf("\n");
        
        if (node->right && node->right != Elist) status = push(&stack, node->right, space + INDENT_STEP, 'R') ;
        if (status != ERR_OK) goto clean_and_exit;
        if (node->left && node->left != Elist) status = push(&stack, node->left, space + INDENT_STEP, 'L'); 
        if (status != ERR_OK) goto clean_and_exit; 
    }
    clean_and_exit:
        free_stack(&stack);
        return status;
    return ERR_OK;
}

void free_node_recurse(TreeNode *node) {
    if (node == NULL || node == Elist) return;
    free_node_recurse(node->left);
    free_node_recurse(node->right);

    free(node->key);
    free_infos(node->infos);
    free(node);
}

void free_tree(Tree *tree) {
    if (tree == NULL) return;
    free_node_recurse(tree->root);
    free(tree);
}

void free_infos(InfoNode *infos){
    if(infos == NULL) return;
    InfoNode *current = infos;
    InfoNode *next;
    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
}

InfoNode *search_elem_by_num(TreeNode *node, unsigned int num, Err *status){
    unsigned int count = 1;
    InfoNode *current = node->infos;
    while (current != NULL){
       if (count == num) {*status = ERR_OK; return current;}
        current = current->next;
        count++; 
    }
    *status = ERR_FIND;
    return NULL;
}      

TreeNode *search_node_by_key(Tree *tree, char *key, Err *status){
    if (tree == NULL) {*status = ERR_NOT_INIT; return NULL;}
    if (tree->root == NULL) {*status = ERR_EMPTY; return NULL;}
    TreeNode *current = tree->root;
    while (current != NULL && current != Elist){
        int cmp = strcmp(key, current->key);
        if (cmp == 0) return current;
        if (cmp > 0) current = current->right;
        else current = current->left; 
    }
    *status = ERR_FIND;
    return NULL;
}

TreeNode *special_search(Tree *tree, TreeNode *node){
    TreeNode *current;
    TreeNode *max_min;
    int cmp = strcmp(node->key, tree->root->key);
    if (node->left == Elist && cmp > 0){ 
        current = node;
        max_min = node->parent;
        while (max_min != NULL){
            if (max_min->right == current) return max_min;
            current = max_min;
            max_min = max_min->parent;
        }
    }

    max_min = node->left;
    if (max_min == Elist) return NULL;
    current = max_min->right;
    while (current != Elist){
        max_min = current;
        current = current->right;
    }

    return max_min;
}

Err push(Stack *stack, TreeNode *node, int space, char L_or_R){
    StackNode *stack_node = malloc(sizeof(StackNode));
    if (!stack_node) return ERR_MEM;

    stack_node->node = node;
    stack_node->space = space;
    stack_node->L_or_R = L_or_R;
    stack_node->next = stack->top;
    stack->top = stack_node;
    return ERR_OK;
}

void pop(Stack *stack, TreeNode **node, int *space, char *L_or_R){
    StackNode *stack_node = stack->top;
    *node = stack_node->node;
    *space = stack_node->space;
    *L_or_R = stack_node->L_or_R;
    stack->top = stack_node->next;
    free(stack_node);
}

void free_stack(Stack *stack){
    StackNode *current = stack->top;
    StackNode *next;
    while (current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    stack->top = NULL;
}

Err export_to_dot(Tree *tree, const char *filename){
    if (tree == NULL) return ERR_NOT_INIT;
    if (tree->root == NULL) return ERR_EMPTY;
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) return ERR_OPEN;
    if (fprintf(file, "digraph Tree {\n") < 0 || fprintf(file, "    node [shape=circle];\n") < 0) {fclose(file); return ERR_WRITE;}
    Err status = export_to_dot_recurse(tree->root, file);
    if (status != ERR_OK) {fclose(file); return status;}

    if (fprintf(file, "}\n") < 0) status = ERR_WRITE;
    fclose(file);
    return status;
}

Err export_to_dot_recurse(TreeNode *node, FILE *file){
    if (node == NULL || node == Elist) return ERR_OK;
    if (fprintf(file, "    \"%p\" [label=\"%s", (void*)node, node->key) < 0)
        return ERR_WRITE;
        
    InfoNode *info = node->infos;
    int count = 0;  
    while (info){
        if (fprintf(file, "\\n[%d] %u", count, info->info) < 0) return ERR_WRITE;
        info = info->next;
        count++;
    }
    const char *fill = (node->color == RED) ? "red" : "black";
    if (fprintf(file, "\", style=filled, fillcolor=%s, fontcolor=white];\n", fill) < 0)
        return ERR_WRITE;

    if (node->left != Elist && 
        fprintf(file, "    \"%p\" -> \"%p\" [label=\"left\" color=\"green\"];\n", (void*)node, (void*)node->left) < 0) 
            return ERR_WRITE;
    if (node->right != Elist && 
        fprintf(file, "    \"%p\" -> \"%p\" [label=\"right\" color=\"blue\"];\n", (void*)node, (void*)node->right) < 0) 
            return ERR_WRITE;

    Err status;
    if ((status = export_to_dot_recurse(node->left, file)) != ERR_OK) return status;
    if ((status = export_to_dot_recurse(node->right, file)) != ERR_OK) return status;
    return ERR_OK;
}
