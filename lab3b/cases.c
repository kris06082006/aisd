#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "cases.h"

Err initialization(Table **table){
    Err status = ERR_OK;
    char *err;

    printf("Enter size for hash table: ");
    unsigned int size;
    status = input_unsigned_int(&size);
    err = error_message(status);
    printf("%s", err);
    if (status == ERR_EOF) return status;

    status = init_table(table, size);
    err = error_message(status);
    printf("%s", err);
    return status;
}

Err adding(Table **table){ // Add an element by key
    char *key, *info;
    Err status = ERR_OK;
    char *err;
    
    status = is_table_full(*table);
    if (status == ERR_OVERFLOW){
        status = resize_table(table);
        if (status != ERR_OK) printf("Error of the resizing table.\n");
        else printf("Successful resizing table.\n");
    }
    else if (status != ERR_OK) {
        err = error_message(status);
        printf("%s", err);
        return status;
    }

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    printf("Enter info: ");
    info = readline("");
    if (key == NULL){
        free(info);
        return ERR_EOF;
    }

    status = add_elem(*table, key, info);
    err = error_message(status);
    printf("%s", err);
    if (status != ERR_OK) {
        free(key);
        free(info);
    }
    return status;
}

Err finding_certain_release(Table *table){ // Find an element by key and release
    char *key;
    int release;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    printf("Enter release: ");
    status = input_int(&release);
    err = error_message(status);
    printf("%s", err);
    if (status == ERR_EOF){
        free(key);
        return status;
    }

    KeySpace **elem = find_certain_release(table, key, release, &status);
    if (elem != NULL) {
        print_vector(elem, 1);
        free_vector(elem, 1);
    }
     else {
        err = error_message(status);
        printf("%s", err);
    }
    free(key);
    return status;
}

Err finding_all_release(Table *table){ // Find all elements by key
    char *key;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    int count;
    KeySpace **elems = find_all_release(table, key, &count, &status);
    if (elems != NULL) {
        print_vector(elems, count);
        free_vector(elems, count);
    }
    else {
        err = error_message(status);
        printf("%s", err);
    }
    free(key);
    return status;
}

Err deleting_certain_release(Table *table){ // Delete an element by key and release
    char *key;
    int release;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    printf("Enter release: ");
    status = input_int(&release);
    err = error_message(status);
    printf("%s", err);
    if (status == ERR_EOF){
        free(key);
        return status;
    }
    status = delete_certain_release(table, key, release);
    err = error_message(status);
    printf("%s", err);
    free(key);
    return status;
}

Err deleting_all_release(Table *table){ // Delete all elements by key
    char *key;
    Err status = ERR_OK;
    char *err;

    printf("Enter key: ");
    key = readline("");
    if (key == NULL) return ERR_EOF;

    status = delete_all_release(table, key);
    err = error_message(status);
    printf("%s", err);
    free(key);
    return status;
}

Err import_from_bin_file(Table **table){ //Import a table from a file
    Err status = ERR_OK;
    char *err;

    if (*table) free_table(*table);
    *table = NULL;
    char *filename = readline("Enter file for import: ");
    if (filename == NULL){
        printf("Incorrect filename.\n");
        return status;
    }
    status = import_from_file(table, filename);
    err = error_message(status);
    printf("%s", err);
    free(filename);
    if (status == ERR_EOF) return status;
    return status;
}

Err export_in_bin_file(Table *table) {//Export a table in a file
    Err status = ERR_OK;
    char *err;

    char *filename = readline("Enter file for export: ");
    if (filename == NULL){
        printf("Incorrect filename.\n");
        return status;
    }

    status = export_in_file(table, filename);
    err = error_message(status);
    printf("%s", err);
    free(filename);
    if (status == ERR_EOF) return status;
    return status;
}

void printing(Table *table){
    Err status = print_table(table);
    char *err = error_message(status);
    printf("%s", err);
    return;
}
