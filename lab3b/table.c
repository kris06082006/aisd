#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "table.h"

Err is_table_full(Table *table){
    if (table == NULL) return ERR_NOT_INIT;
    for (unsigned int i = 0; i < table->msize; i++){
        if (table->ks[i] == NULL) return ERR_OK;
    }
    return ERR_OVERFLOW;
}

Err resize_table(Table **table) {
    if ((*table)->csize == UINT_MAX) return ERR_OVERFLOW;
    static unsigned int numbers[27] = {
        11, 23, 47, 97, 199, 409, 823, 1741, 3469, 6947,
        13997, 28001, 56003, 200171, 400417, 
	800837, 1600817, 3201017, 6400007, 12800129, 25600357, 
	51200837, 102402407, 204804917, 409610017, 819200171, UINT_MAX
    };
    unsigned int new_msize = 0;    
    for (int i = 0; i < 13; i++){
        if (numbers[i] > (*table)->msize){
            new_msize = numbers[i];
            break;
        }
    }
    Table *new_table = NULL;
    Err status = init_table(&new_table, new_msize);
    if (new_table == NULL) return status;
    
    for (unsigned int i = 0; i < (*table)->msize; i++){
        KeySpace *current = (*table)->ks[i];
        while (current != NULL){
            char *new_key = strdup(current->key);
            char *new_info = strdup(current->info);
            if (new_key == NULL || new_info == NULL){
                free(new_key);
                free(new_info);
                free_table(new_table);
                return ERR_MEM;
            }
            status = add_elem(new_table, new_key, new_info);
            if (status != ERR_OK){
                free(new_key);
                free(new_info);
                free_table(new_table);
                return status;
            }
            current = current->next;
        }
    }
    free_table(*table);
    *table = new_table;
    return ERR_OK;
    
}

Err init_table(Table **table, unsigned int size){
    if (*table != NULL) return ERR_INIT;
    *table = malloc(sizeof(Table));
    if (*table == NULL) return ERR_MEM;

    (*table)->msize = size;
    (*table)->csize = 0;
    (*table)->ks = calloc(size, sizeof(KeySpace*));
    if ((*table)->ks == NULL){
        free(*table);
        *table = NULL;
        return ERR_MEM;
    }
    return ERR_OK;
}

unsigned long long hash(char *key, unsigned int msize) {
    unsigned long long hash = INT_MAX;
    for (int i = 0; key[i]; i++){
        hash = 37*hash + (unsigned char)key[i]; 
    }
    return hash % msize;
}

Err add_elem(Table *table, char *key, char *info){
    unsigned long long index = hash(key, table->msize);
    unsigned int max_release = find_last_release(table, index, key);    

    KeySpace *new_item = malloc(sizeof(KeySpace));
    if (new_item == NULL) return ERR_MEM;
    new_item->key = key;
    new_item->release = max_release + 1;
    new_item->info = info;

    new_item->next = table->ks[index];
    table->ks[index] = new_item;
    table->csize++;

    return ERR_OK;
}

unsigned int find_last_release(Table *table, unsigned int index, char* key){
    KeySpace *current = table->ks[index]; 
    unsigned int max_release = 0;
    while (current != NULL){
        if (strcmp(current->key, key) == 0 && current->release > max_release){
            max_release = current->release;
        }
        current = current->next;
    }
    return max_release;
}

KeySpace **find_certain_release(Table *table, char *key, int release, Err *status){
    if (table == NULL){
        *status = ERR_NOT_INIT;
        return NULL;
    }
    else if (table->csize == 0){
        *status = ERR_EMPTY;
        return NULL;
    }
    unsigned long long index = hash(key, table->msize);

    KeySpace *current = table->ks[index];
    while (current != NULL){
        if (strcmp(current->key, key) == 0 && current->release == release){
            KeySpace **vector = malloc(sizeof(KeySpace*));
    		if (vector == NULL){
        		*status = ERR_MEM;
        		return NULL;
    		}
            
            KeySpace *item = malloc(sizeof(KeySpace));
            if (item == NULL){
                free(vector);
                *status = ERR_MEM;
                return NULL;
            }
            item->key = strdup(current->key);
            if (item->key == NULL){
                free(item);
                free(vector);
                *status = ERR_MEM;
                return NULL;
            }
            item->release = current->release;
            item->info = strdup(current->info);
            if (item->info == NULL){
                free(item->key);
                free(item);
                free(vector);
                *status = ERR_MEM;
                return NULL;
            }
            item->next = NULL;

            vector[0] = item;
            *status = ERR_OK;
            return vector;
        }
        current = current->next;
    }
    *status = ERR_FIND;
    return NULL;
}

Err delete_all_release(Table *table, char *key){
    if (table == NULL) return ERR_NOT_INIT;
    else if (table->csize == 0) return ERR_EMPTY;
    unsigned long long index = hash(key, table->msize);
    KeySpace *current =  table->ks[index];
    KeySpace *prev = NULL;
    int deleted = 0;
    
    while (current != NULL){
        if (strcmp(current->key, key) == 0){
            if (prev == NULL) table->ks[index] = current->next;
            else prev->next = current->next;
            
            KeySpace *tmp = current;
            current = current->next;
            
            free(tmp->key);
            free(tmp->info);
            free(tmp);

            deleted++;
            table->csize--;            
        }
        else {
            prev = current;
            current = current->next;
        }
    }
    if (deleted == 0) return ERR_FIND;
    return ERR_OK; 
}

Err delete_certain_release(Table *table, char *key, int release){
    if (table == NULL) return ERR_NOT_INIT;
    if (table->csize == 0) return ERR_EMPTY;
    unsigned long long index = hash(key, table->msize);
    KeySpace *current =  table->ks[index];
    KeySpace *prev = NULL;

    while (current != NULL){
        if (strcmp(current->key, key) == 0 && current->release == release){
            if (prev == NULL) table->ks[index] = current->next;
            else prev->next = current->next;

            free(current->key);
            free(current->info);
            free(current);

            table->csize--;
            return ERR_OK;
        }
        else {
            prev = current;
            current = current->next;
        }
    }
    return ERR_FIND;
}

KeySpace **find_all_release(Table *table, char *key, int *count, Err *status) {
    if (table == NULL){
        *status = ERR_NOT_INIT;
        return NULL;
    }
    else if (table->csize == 0){
        *status = ERR_EMPTY;
        return NULL;
    }
    unsigned long long index = hash(key, table->msize);
    KeySpace *current = table->ks[index];
    *count = 0;
    
    while (current != NULL){
        if (strcmp(current->key, key) == 0) (*count)++;
        current = current->next;
    }
    if (*count == 0){
        *status = ERR_FIND;
        return NULL;
    }
    
    KeySpace **vector = malloc(*count * sizeof(KeySpace*));
    if (vector == NULL){
        *status = ERR_MEM;
        return NULL;
    }

    current = table->ks[index];
    unsigned int i = 0;
    while (current != NULL){
        if (strcmp(current->key, key) == 0){
            KeySpace *item = malloc(sizeof(KeySpace));
            if (item == NULL){
                free_vector(vector, i); 
                *status = ERR_MEM;
                return NULL;
            }
            item->key = strdup(current->key);
            if (item->key == NULL){
                free(item);
                free_vector(vector, i);
                *status = ERR_MEM;
                return NULL;
            }
            item->release = current->release;
            item->info = strdup(current->info);
            if (item->info == NULL){
                free(item->key);
                free(item);
                free_vector(vector, i);
                *status = ERR_MEM;
                return NULL;
            }
            item->next = NULL;
            
            vector[i++] = item;
        }
        current = current->next;
    }
    *status = ERR_OK;
    return vector;
}

Err print_table(Table *table) {
    if (table == NULL) return ERR_NOT_INIT;
    printf("Hash Table has size: %d, items: %d\n\n", table->msize, table->csize);

    for (int i = 0; i < table->msize; i++) {
        KeySpace *current = table->ks[i];
        while (current != NULL) {
            printf("Index:   %d\n", i);
            printf("Key:     %s\n", current->key);
            printf("Release: %d\n", current->release);
            printf("Info:    %s\n", current->info);
            printf("-------------------------------------\n");
            current = current->next;
        }
    }
}

void print_vector(KeySpace **vector, int size) {
    printf("Vector has %d items:\n\n", size);

    for (int i = 0; i < size; i++) {
        if (vector[i]) {
            printf("Index:   %d\n", i);
            printf("Key:     %s\n", vector[i]->key);
            printf("Release: %d\n", vector[i]->release);
            printf("Info:    %s\n", vector[i]->info);
            printf("-------------------------------------\n");
        }
    }
}



void free_table(Table *table){
    if (table == NULL) return;
    for (int i = 0; i < table->msize; i++){
        KeySpace *current = table->ks[i];
        while (current != NULL){    
            KeySpace *tmp = current;
            current = current->next;
            free(tmp->key);
            free(tmp->info);
            free(tmp);
        }
    }
    free(table->ks);
    free(table);
}

void free_vector(KeySpace **vector, int size){ 
    if (vector == NULL) return;
    for (int i = 0; i < size; i++){
        free(vector[i]->key);
        free(vector[i]->info);
        free(vector[i]);
    }
    free(vector);
}

Err import_from_file(Table **table, const char *filename){
    if (*table != NULL) return ERR_INIT;
    int read_count = 0;
    Err status = ERR_OK;
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return ERR_OPEN;

    unsigned int msize, csize;
    char *key = NULL, *info = NULL;

    if (fread(&msize, sizeof(unsigned int), 1, file) != 1){
        fclose(file);
        return ERR_READ;
    }
    if (fread(&csize, sizeof(unsigned int), 1, file) != 1){
        fclose(file);
        return ERR_READ;
    }

    status = init_table(table, msize);
    if (table == NULL){
        fclose(file);
        return ERR_MEM;
    }

    (*table)->msize = msize;
    (*table)->csize = 0;
    for (unsigned int i = 0; i < csize; i++){
        int key_len = 0, release = 0, info_len = 0;
        
        if (fread(&key_len, sizeof(int), 1, file) != 1) {
            status = ERR_READ;
            goto clean_and_close;
        }
        key = calloc(key_len + 1, sizeof(char));
        if (key == NULL) {
            status = ERR_MEM;
            goto clean_and_close;
        }
        if (fread(key, sizeof(char), key_len, file) != key_len){
            status = ERR_READ;
            goto clean_and_close;
        }

        if (fread(&release, sizeof(int), 1, file) != 1){
            status = ERR_READ;
            goto clean_and_close;
        }

        if (fread(&info_len, sizeof(int), 1, file) != 1){
            status = ERR_READ;
            goto clean_and_close;
        }
        info = calloc(info_len + 1, sizeof(char));
        if (info == NULL){
            status = ERR_MEM;
            goto clean_and_close;
        }
        if (fread(info, sizeof(char), info_len, file) != info_len){
            status = ERR_READ;
            goto clean_and_close;
        }
        
        status = add_elem(*table, key, info);
        if (status != ERR_OK) goto clean_and_close;
    }
    fclose(file);
    return ERR_OK;

    clean_and_close:
        free(key);
        free(info);
        free_table(*table);
        *table = NULL;
        
        fclose(file);
        return status;
}

Err export_in_file(Table *table, const char *filename){
    if (table == NULL) return ERR_NOT_INIT;

    FILE *file = fopen(filename, "wb");
    if (file == NULL) return ERR_OPEN;

    if (fwrite(&(table->msize), sizeof(unsigned int), 1, file) != 1) goto close_file;
    if (fwrite(&(table->csize), sizeof(unsigned int), 1, file) != 1) goto close_file;
    
    for (unsigned int i = 0; i < table->msize; i++){
        KeySpace *current = table->ks[i];

        while (current != NULL){
            int key_len = strlen(current->key);
            if (fwrite(&key_len, sizeof(int), 1, file) != 1) goto close_file;
            if (fwrite(current->key, sizeof(char), key_len, file) != key_len) goto close_file;

            if (fwrite(&current->release, sizeof(int), 1, file) != 1) goto close_file;

            int info_len = strlen(current->info);
            if (fwrite(&info_len, sizeof(int), 1, file) != 1) goto close_file;
            if (fwrite(current->info, sizeof(char), info_len, file) != info_len) goto close_file;

            current = current->next;
        }
    }
    fclose(file);
    return ERR_OK;

    close_file:
        fclose(file);
        return ERR_WRITE;
}

