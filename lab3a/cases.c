#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "cases.h"

void initialization(Table **table){
    char *err;
    Err status = init_table(table);
    err = error_message(status);
    printf("%s",err);
}

Err adding(Table *table){ // Add an element by key
    unsigned int key, info;
    Err status = ERR_OK;
    char *err;
    
    printf("Enter key: ");
    status = input_unsigned_int(&key);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    printf("Enter info: ");
    status = input_unsigned_int(&info);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    status = add_elem(table, key, info);
    err = error_message(status);
    printf("%s",err);
    return status;
}

Err finding_certain_release(Table *table) {// Find an element by key and release
    unsigned int key;
    int release;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    status = input_unsigned_int(&key);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    printf("Enter release: ");
    status = input_int(&release);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    Table *elem = find_certain_release(table, key, release, &status);
    if (elem != NULL) {
        print_table(elem);
        free_table(elem);
    }
    else {
        err = error_message(status);
        printf("%s",err);
    }
      
    return status;
}

Err finding_all_release(Table *table){ // Find all elements by key
    unsigned int key;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    status = input_unsigned_int(&key);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    Table *new_table = find_all_release(table, key, &status);
    if (new_table != NULL) {
        print_table(new_table);
        free_table(new_table);
    }
    else {
        err = error_message(status);
        printf("%s",err);
    }
    return status;
}

Err deleting_certain_release(Table *table){ // Delete an element by key and release
    unsigned int key;
    int release;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    status = input_unsigned_int(&key);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    printf("Enter release: ");
    status = input_int(&release);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    status = delete_certain_release(table, key, release);
    err = error_message(status);
    printf("%s",err);
    return status;
}

Err deleting_all_release(Table *table){ // Delete all elements by key
    unsigned int key;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    status = input_unsigned_int(&key);
    err = error_message(status);
    printf("%s",err);
    if (status == ERR_EOF) return status;

    status = delete_all_release(table, key);
    err = error_message(status);
    printf("%s",err);
    return status;
}

Err import_from_txt(Table **table){ // Import a table from a file
    char *err;
    char *filename = readline("Enter file for reading: ");
    if (filename == NULL) return ERR_EOF;

    Err status = import_from_file(table, filename);
    err = error_message(status);
    printf("%s",err);

    free(filename);
    return status;
}

void printing(Table *table){
    char *err;
    Err status = print_table(table);
    err = error_message(status);
    printf("%s",err);
    return;
}
         
