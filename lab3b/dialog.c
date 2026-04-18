#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "dialog.h"

Err dialog(){
    Table *table = NULL;
    int opt;
    Err status;
    while (1) {
        printf("\n--- Меню ---\n");
        printf("1. Initialization a table\n");
        printf("2. Add an element by key\n");
        printf("3. Find an element by key and release\n");
        printf("4. Find all elements by key\n");
        printf("5. Delete an elem by key and release\n");
        printf("6. Delete all elements by key\n");
        printf("7. Import a table from a file\n");
        printf("8. Export a table in a file\n");
        printf("9. Print a table\n");
        printf("0. Exit\n");
        printf("Choose the option number: ");
    
        status = input_int(&opt);
        char *err = error_message(status);
        printf("%s", err);
        if (status == ERR_EOF) goto clean_and_exit;
    
        switch (opt){
            case 1:
                status = initialization(&table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 2: // Add an element by key
                status = adding(&table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 3: // Find an element by key and release
                status = finding_certain_release(table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 4: // Find all elements by key
                status = finding_all_release(table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 5: // Delete an element by key and release
                status = deleting_certain_release(table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 6: // Delete all elements by key
                status = deleting_all_release(table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 7: //Import a table from a file  
                status = import_from_bin_file(&table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 8: //Export a table in a file
                status = export_in_bin_file(table);
                if (status == ERR_EOF) goto clean_and_exit;
                break;
            case 9:
                printing(table);
                break;
            case 0:
                printf("Exiting the program.\n");
                free_table(table);
                return ERR_OK;
            default:
                printf("Invalid choice. Try again.\n");
                break;      
                
        }
    }
    clean_and_exit:
        free_table(table);
        return ERR_EOF;
}
