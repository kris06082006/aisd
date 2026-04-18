#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

Err tree_traversal_for_timing(Tree *tree){
    if (tree == NULL) return ERR_NOT_INIT;
    if (tree->root == NULL) return ERR_EMPTY;

    TreeNode *current = tree->root;
    while (current->left || current->right) {
        if (current->left)
            current = current->left;
        else
            current = current->right;
    }

    while (current) {
        current = current->next;
    }

    return ERR_OK;
}

Err tree_traversal(Tree *tree){
    if (tree == NULL) return ERR_NOT_INIT;
    if (tree->root == NULL) return ERR_EMPTY;

    TreeNode *current = tree->root;
    while (current->left || current->right){
        if (current->left) current = current->left;
        else current = current->right;
    }
    while (current){
        printf("Key: %s, Info: ", current->key);
        InfoNode *info = current->infos;
        while (info){
            printf("%s ", info->info);
            info = info->next;
        }
        printf("\n");
        current = current->next;
    }
    return ERR_OK;
}

Err tree_traversal_out_of_range(Tree *tree, char *min, char *max){
    if (tree == NULL) return ERR_NOT_INIT;
    if (tree->root == NULL) return ERR_EMPTY;

    TreeNode *current = tree->root;
    while (current->left) current = current->left;

    while (current){
        if (strcmp(current->key, min) > 0 && strcmp(current->key, max) < 0){
            printf("Key: %s, Info: ", current->key);
            InfoNode *info = current->infos;
            while (info){
              printf("%s ", info->info);
              info = info->next;
            }
            printf("\n");
        }
        current = current->next;
    }
    return ERR_OK;
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
    char *info = NULL;
    size_t key_len = 0;
    size_t info_len = 0;

    while (getline(&key, &key_len, file) != -1 && getline(&info, &info_len, file) != -1){
        key[strcspn(key, "\n")] = '\0';
        info[strcspn(info, "\n")] = '\0';

        status = insert_node(*tree, key, info);
        if (status != ERR_OK) break;
    }
    if (!feof(file) && status == ERR_OK) status = ERR_READ;

    free(key);
    free(info);

    if (status != ERR_OK) {free_tree(*tree); *tree = NULL;}
    fclose(file);
    return status;
}

TreeNode *create_node(char *key, char *info, TreeNode *parent){
    TreeNode *new_node = malloc(sizeof(TreeNode));
    if (new_node == NULL) return NULL;
    new_node->key = strdup(key);
    if (new_node->key == NULL) return NULL;

    InfoNode *new_info = malloc(sizeof(InfoNode));
    if (new_info == NULL){
        free(new_node->key);
        free(new_node);
        return NULL;
    }
    new_info->info = strdup(info);
    if (new_info->info == NULL) {
        free(new_info);
        free(new_node->key);
        free(new_node);
        return NULL;
    }
    new_info->next = NULL;

    new_node->infos = new_info;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->parent = parent;
    return new_node;

}

Err insert_node(Tree *tree, char *key, char *info){
    if (tree == NULL) return ERR_NOT_INIT;

    // 1.insert in root
    if (tree->root == NULL) {
        tree->root = create_node(key, info, NULL);
        if (tree->root == NULL) return ERR_MEM;
        return ERR_OK;
    }

    // find place for insert
    TreeNode *parent = NULL;
    TreeNode *current = tree->root;
    int cmp;
    while (current != NULL){
        cmp = strcmp(key, current->key);
        if (cmp == 0){
            InfoNode *new_info = malloc(sizeof(InfoNode));
            if (new_info == NULL) return ERR_MEM;

            new_info->info = strdup(info); 
            if (new_info->info == NULL){
                free(new_info);
                return ERR_MEM;
            }
            new_info->next = current->infos;
            current->infos = new_info;
            return ERR_OK;
        }
        parent = current;
        if (cmp < 0) current = current->left;
        else current = current->right;
    }

    TreeNode *new_node = create_node(key, info, parent);
    if (new_node == NULL) return ERR_MEM;
    // 2.insert in right brahch
    if (cmp > 0){
        parent->right = new_node;

        //linkage
        new_node->next = parent;
        new_node->prev = parent->prev;
        if (parent->prev) parent->prev->next = new_node;
        parent->prev = new_node;
        return ERR_OK;
    }
    //3.insert in left branch
    else{
        parent->left = new_node;

        //linkage 3.1. without right brother
        if (!parent->right){
            new_node->next = parent;
            new_node->prev = parent->prev;
            if (parent->prev) parent->prev->next = new_node;
            parent->prev = new_node;
            return ERR_OK;
        }

        //linkage 3.2. with right brother
        else{
            TreeNode *leftmost_in_right = parent->right;
            while (leftmost_in_right->left || leftmost_in_right->right){
                if (leftmost_in_right->left) leftmost_in_right = leftmost_in_right->left;
                else leftmost_in_right = leftmost_in_right->right;
            }
            new_node->next = leftmost_in_right;
            new_node->prev = leftmost_in_right->prev;
            if (leftmost_in_right->prev) leftmost_in_right->prev->next = new_node;
            leftmost_in_right->prev = new_node;
            
            return ERR_OK;
        }
    }
}

Err delete_node(Tree *tree, TreeNode *node){
    if (tree == NULL) return ERR_NOT_INIT;
    if (node == NULL) return ERR_FIND;
    else if (tree->root == NULL) return ERR_EMPTY;
    //1.delete leaf
    if (!node->left && !node->right){
        if (node->prev) node->prev->next = node->next; 
        if (node->next) node->next->prev = node->prev;

        if (!node->parent) tree->root = NULL;
        else if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL; 

        free_infos(node->infos); 
        free(node->key);
        free(node);
        return ERR_OK;
    }
    //2.delete node witn 1 child
    if (!node->left || !node->right){
    TreeNode *child;
    if (!node->right) child = node->left;
    else child = node->right;

    child->parent = node->parent;

    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;

    if (!node->parent) tree->root = child;
    else if (node->parent->left == node) node->parent->left = child;
    else node->parent->right = child;

    free_infos(node->infos); 
        free(node->key);
        free(node);
        return ERR_OK;
    }
    //3. delete node with 2 children
    TreeNode *leftmost_in_right = node->right;
    while (leftmost_in_right->left) leftmost_in_right = leftmost_in_right->left;

    InfoNode *new_infos = copy_infos(leftmost_in_right->infos);
    if (new_infos == NULL) return ERR_MEM;
    char *new_key = strdup(leftmost_in_right->key);
    if (new_key == NULL){
        free_infos(new_infos);
        return ERR_MEM;
    }
    free_infos(node->infos);
    free(node->key);
    node->infos = new_infos;
    node->key = new_key;

    return delete_node(tree, leftmost_in_right);
}

void free_infos(InfoNode *infos){
    if (infos == NULL) return;
    InfoNode *current = infos;
    InfoNode *next;
    while (current != NULL){
        next = current->next;
        free(current->info);
        free(current);
        current = next;
    }
}

void free_tree(Tree *tree){
    if (tree == NULL) return;
    if (tree->root == NULL) {free(tree); return;}
    TreeNode *current = tree->root;
    TreeNode *next = current->prev;
    while (next != NULL){
        free(current->key);
        free_infos(current->infos);
        free(current);
        current = next;
        next = current->prev;
    }
    free(current->key);
    free_infos(current->infos);
    free(current);
    free(tree);
}

InfoNode *copy_infos(InfoNode *infos){
    InfoNode *new_infos = NULL;

    while (infos != NULL){
        InfoNode *new_node = malloc(sizeof(InfoNode));
        if (new_node == NULL){
            free_infos(new_infos);
            return NULL;
        }
        new_node->info = strdup(infos->info);
        if (new_node->info == NULL){
            free(new_node);
            free_infos(new_infos);
            return NULL;
        } 
        new_node->next = new_infos;
        new_infos = new_node;     
        
        infos = infos->next;
        
    }
    return new_infos;
} 

TreeNode *search_node_by_key(Tree *tree, char *key, Err *status){
    if (tree == NULL) {*status = ERR_NOT_INIT; return NULL;}
    if (tree->root == NULL) {*status = ERR_EMPTY; return NULL;}
    TreeNode *current = tree->root;
    while (current != NULL){
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
    TreeNode *min_max;
    int cmp = strcmp(node->key, tree->root->key);
    if (!node->right && cmp < 0){ 
        current = node;
        min_max = node->parent;
        while (min_max != NULL){
            if (min_max->left == current) return min_max;
            current = min_max;
            min_max = min_max->parent;
        }
    }

    min_max = node->right;
    if (!min_max) return NULL;
    current = min_max->left;
    while (current != NULL){
        min_max = current;
        current = current->left;
    }

    return min_max;
}

void print_node(TreeNode *node){
    if (node == NULL) return;
    printf("Key: %s, ", node->key);
    InfoNode *current = node->infos;
    printf("Info: ");
    while (current != NULL){
        printf("%s ", current->info);
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
    Err status;

    while (stack.top){
        pop(&stack, &node, &space, &L_or_R);
        for (int i = 0; i < space; i++) printf(" ");
        printf("--> ");
        if (L_or_R == 'L') printf("[L] ");
        else if(L_or_R == 'R') printf("[R] ");
        print_node(node);
        printf("\n");
        
        if (node->right) status = push(&stack, node->right, space + INDENT_STEP, 'R') ;
        if (status != ERR_OK) goto clean_and_exit;
        if (node->left) status = push(&stack, node->left, space + INDENT_STEP, 'L'); 
        if (status != ERR_OK) goto clean_and_exit; 
    }
    clean_and_exit:
        free_stack(&stack);
        return status;
    return ERR_OK;
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

    if (fprintf(file, "digraph Tree {\n") < 0 || fprintf(file, "    node [shape=circle];\n") < 0)
        goto close_and_exit;
    
    TreeNode *current = tree->root;
    while (current){
        if (fprintf(file, "    \"%p\" [label=\"%s", (void*)current, current->key) < 0)
            goto close_and_exit;
        
        InfoNode *info = current->infos;
        int count = 0;  
        while (info){
            if (fprintf(file, "\\n[%d] %s", count, info->info) < 0) goto close_and_exit;
            info = info->next;
            count++;
        }
        if (fprintf(file, "\"];\n") < 0) goto close_and_exit;

        if (current->left && 
            fprintf(file, "    \"%p\" -> \"%p\" [label=\"left\" color=\"green\"];\n", (void*)current, (void*)current->left) < 0) 
                goto close_and_exit;
        if (current->right && 
            fprintf(file, "    \"%p\" -> \"%p\" [label=\"right\" color=\"blue\"];\n", (void*)current, (void*)current->right) < 0) 
                goto close_and_exit;
        if (current->next &&
            fprintf(file, "    \"%p\" -> \"%p\" [label=\"next\" color=\"red\" style=\"dashed\"];\n", 
                   (void*)current, (void*)current->next) < 0)
                goto close_and_exit;
        
        if (current->prev &&
            fprintf(file, "    \"%p\" -> \"%p\" [label=\"prev\" color=\"orange\" style=\"dashed\"];\n", 
                   (void*)current, (void*)current->prev) < 0)
                goto close_and_exit;
        current = current->prev;
    }
    fprintf(file, "}\n");
    fclose(file);
    return ERR_OK;

    close_and_exit:
        fclose(file);
        return ERR_WRITE;
}
