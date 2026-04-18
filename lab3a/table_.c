#include <stdio.h>
#include <stdlib.h>

#include "table_.h"

Err init_table(Table **table){
    if (*table != NULL) return ERR_INIT;

    *table = malloc(sizeof(Table));
    if (table == NULL) return ERR_MEM;
    (*table)->ks = NULL;
    return ERR_OK;
}

Err add_elem(Table *table, unsigned int key, unsigned int info){
    if (table == NULL) return ERR_NOT_INIT;

    int new_release = 1;
    KeySpace *elem = find_last_release(table, key);
    if (elem != NULL) new_release = elem->release + 1;

    KeySpace *new_elem = malloc(sizeof(KeySpace));
    if (new_elem == NULL) return ERR_MEM;
    new_elem->key = key;
    new_elem->info = info;
    new_elem->release = new_release;
    new_elem->next = table->ks;
    table->ks = new_elem;       

    return ERR_OK;
}

KeySpace *find_last_release(Table *table, unsigned int key){
    KeySpace *current = table->ks; 
    while (current != NULL){
        if (current->key == key) return current;
        current = current->next;
    }
    return NULL;
}

Table *find_certain_release(Table *table, unsigned int key, int release, Err *status){
    if (table == NULL){
        *status = ERR_NOT_INIT;
        return NULL;
    }
    else if (table->ks == NULL){
        *status = ERR_EMPTY;
        return NULL;
    }

    Table *new_table = malloc(sizeof(Table));
    if (new_table == NULL){
        *status = ERR_MEM;
        return NULL;
    }
    new_table->ks = NULL;
    
    KeySpace *current = table->ks;
    while (current != NULL){
        if (current->key == key && current->release == release) {
            KeySpace *new_item = malloc(sizeof(KeySpace));
            if (new_item == NULL){
                free(new_table);
                return NULL;
            }
            new_item->release = current->release;
            new_item->key = current->key;
            new_item->info = current->info;

            new_item->next = new_table->ks;
            new_table->ks = new_item;
            return new_table;
        }
        current = current->next;
    }
    free(new_table);
    return NULL;
}

Err delete_all_release(Table *table, unsigned int key){
    if (table == NULL) return ERR_NOT_INIT;
    else if (table->ks == NULL) return ERR_EMPTY;

    if (table->ks == NULL) return ERR_EMPTY;
    int c = 0;
    KeySpace *prev = NULL;
    KeySpace *current = table->ks;
    while (current != NULL){
        if (current->key == key){
            KeySpace *tmp = current;
            if (prev == NULL){
                table->ks = current->next;
            }
            else {
                prev->next = current->next;
            }
            current = current->next;
            free(tmp);
            c += 1;
        }
        else{
            prev = current;
            current = current->next;
        }
    }
    if (c == 0) return ERR_FIND;
    else return ERR_OK;     
}

Err delete_certain_release(Table *table, unsigned int key, int release){
    if (table == NULL) return ERR_NOT_INIT;
    else if (table->ks == NULL) return ERR_EMPTY;

    if (table->ks == NULL) return ERR_EMPTY;
    KeySpace *prev = NULL;
    KeySpace *current = table->ks;
    while (current != NULL){
        if (current->key == key && current->release == release){
            if (prev == NULL){
                table->ks = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current); 
            return ERR_OK;
        }
        prev = current;
        current = current->next;
    }
    return ERR_FIND;
}

Table *find_all_release(Table *table, unsigned int key, Err *status) {
    if (table == NULL){
        *status = ERR_NOT_INIT;
        return NULL;
    }
    else if (table->ks == NULL){
        *status = ERR_EMPTY;
        return NULL;
    }

    Table *new_table = malloc(sizeof(Table));
    if (new_table == NULL) {
        *status = ERR_MEM;
        return NULL;
    }
    new_table->ks = NULL;
    
    KeySpace *current =  table->ks;

    while(current != NULL){
        if (current->key == key){
            KeySpace *new_item = malloc(sizeof(KeySpace));
            if (new_item == NULL){
                free(new_table);
                *status = ERR_MEM;
                return NULL;
            }
            new_item->key = current->key;
            new_item->release = current->release;
            new_item->info = current->info;

            new_item->next = new_table->ks;
            new_table->ks = new_item;
        }
        current = current->next;
    }
    return new_table;    
}

Err print_table(Table *table){
    if (table == NULL) return ERR_NOT_INIT;

    printf("+------------+---------+------------+\n");
    printf("|    Key     | Release |    Info    |\n");
    printf("+------------+---------+------------+\n");
    KeySpace *current = table->ks;
    while (current != NULL) {
        printf("| %10u | %7d | %10u |\n", current->key, current->release, current->info);
        current = current->next;
    }

    printf("+------------+---------+------------+\n");
    return ERR_OK;
}

void free_table(Table *table){
    if (table == NULL) return;

    KeySpace *current = table->ks;
    while (current != NULL){
        KeySpace *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(table);
}

Err import_from_file(Table **table, const char *filename){
    free_table(*table);
    *table = NULL;

    FILE *file = fopen(filename, "r");
    if (file == NULL) return ERR_OPEN;
    init_table(table);
    if (table == NULL){
        fclose(file);
        return ERR_MEM;
    }
    
    unsigned int key, info;
    int release;
    int status;
    while (fscanf(file, "%u %d %u", &key, &release, &info) == 3){
        status = add_elem(*table, key, info);
        if (status != ERR_OK){
            free_table(*table);
            fclose(file);
            return status;    
        }
    }
    if (feof(file)){
        fclose(file);
        return ERR_OK;
    }
    else{
        free_table(*table);
        fclose(file);
        *table = NULL;
        return ERR_SCANF;
    }
}
