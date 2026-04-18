#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "cases.h"

void initialization(Tree **tree){
    Err status = init_tree(tree);
    char *err = error_message(status);
    printf("%s", err);
    return;
}

Err adding(Tree *tree){ // Add an element by key
    char *key, *info, *err;
    Err status = ERR_OK;
    
    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    printf("Enter info: ");
    info = readline("");
    if (info == NULL){
        free(key);
        return ERR_EOF;
    }

    status = insert_node(tree, key, info);
    err = error_message(status);
    printf("%s", err);

    free(key);
    free(info);
    return status;
}

Err searching(Tree *tree) { // Search elemens by key
    char *key, *err;
    Err status = ERR_OK;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    TreeNode *node = search_node_by_key(tree, key, &status);
    if (node != NULL) print_node(node);
    else {err = error_message(status); printf("%s", err);}

    free(key);
    return status;
}

Err special_searching(Tree *tree){ // Special search elements (min max key after original key)
    char *key, *err;
    Err status = ERR_OK;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    TreeNode *node = search_node_by_key(tree, key, &status);
    if (node != NULL){ 
        TreeNode *new_node = special_search(tree, node);
        if (new_node != NULL) print_node(new_node);
        else {err = error_message(ERR_FIND); printf("%s", err);}
    }
    else {err = error_message(status); printf("%s", err);}

    free(key);
    return status;   

}

Err deletion(Tree *tree){ // Delete an element by key
    char *key, *err;
    Err status = ERR_OK;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;
    
    TreeNode *node = search_node_by_key(tree, key, &status);
    if (node == NULL){
        err = error_message(status);
        printf("%s", err);
        return status;
    }

    status = delete_node(tree, node);
    err = error_message(status);
    printf("%s", err);
    
    free(key);
    return status;
}

Err import_from_txt(Tree **tree){ // Import tree from a txt file
    Err status = ERR_OK;
    char *err;
    char *filename = readline("Enter file for reading: ");
    if (filename == NULL) return ERR_EOF;

    status = import_from_file(tree, filename);
    err = error_message(status);
    printf("%s", err);

    free(filename);
    return status;

}

Err output_tree_as_tree(Tree *tree){ //Output tree as tree
    Err status = print_tree_as_tree(tree);
    char *err = error_message(status);
    printf("%s", err);
    return status;
}

Err graphical_output_tree(Tree *tree){ //Graphical output tree
    Err status = export_to_dot(tree, "tree1.dot");
    char *err = error_message(status);
    printf("%s", err);
    if (status != ERR_OK) return status;
    
    int result = system("dot -Tpng tree1.dot -o tree1.png");
    if (result != 0) {
        printf("%sGraphviz launch error\n", error_message(ERR_SYS)); 
        return ERR_SYS;
    }
    
    return ERR_OK;
}
   
void traversal(Tree *tree){
    Err status = tree_traversal(tree);
    char *err = error_message(status);
    printf("%s", err);
    return;
}

Err special_traversal(Tree *tree){
    char *min, *max, *err;

    printf("Enter min: ");
    min = readline("");
    if (min == NULL) return ERR_EOF;

    printf("Enter max: ");
    max = readline("");
    if (max == NULL){
        free(min);
        return ERR_EOF;
    }
    Err status = tree_traversal_out_of_range(tree, min, max);    
    err = error_message(status);
    printf("%s", err);

    free(min);
    free(max);
    return status;
}      
