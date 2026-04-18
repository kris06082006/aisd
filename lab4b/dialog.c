#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "dialog.h"

Err dialog(){
    Tree *tree = NULL;
    int opt; 
    Err status;
    while (1) {
        printf("\n--- Меню ---\n");
        printf("1. Initialization a bin tree\n");
        printf("2. Add an element by key\n");
        printf("3. Search elements by key\n");
        printf("4. Special search elements (max key is smaller than the original)\n");
        printf("5. Delete all elements by key\n");
        printf("6. Import a tree from a txt file\n");
        printf("7. Print a bin tree as tree\n");
        printf("8. Graphical output tree\n");
        printf("9. Threaded traversal\n");
        printf("0. Exit\n");
        printf("Choose the option number: ");
    
        status = input_int(&opt);
        char *err = error_message(status);
        printf("%s", err);
        if (status == ERR_EOF) goto clean_and_exit;
    
        switch (opt){
            case 1:
                initialization(&tree);
                break;
            case 2: // Add an element by key
                status = adding(tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 3: // Search elements by key
                status = searching(tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 4: // Special search
		        status = special_searching(tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break; 
            case 5: // Delete an element by key
		        status = deletion(tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break; 
            case 6: // Import a tree from txt file
		        status = import_from_txt(&tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 7: // Print a bin tree as tree
               	status = output_tree_as_tree(tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 8: // Graphical output tree
                status = graphical_output_tree(tree);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 9: // Tree traversal
                traversal(tree);
                break;
            case 0:
                printf("Exiting the program.\n");
                free_tree(tree);
                return ERR_OK;
            default:
                printf("Invalid choice. Try again.\n");
                break;      
                
        }
    }
    clean_and_exit:
        free_tree(tree);
        return ERR_EOF;
}
