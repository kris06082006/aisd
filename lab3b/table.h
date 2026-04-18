#ifndef TABLE_H
#define TABLE_H

#include "functions.h"

typedef struct KeySpace {
    char *key;
    int release;
    char *info;
    struct KeySpace *next;
} KeySpace;

typedef struct Table {
    KeySpace **ks;
    unsigned int msize;
    unsigned int csize;
} Table;

Err init_table(Table **table, unsigned int size);
unsigned long long hash(char *key, unsigned int msize);
Err add_elem(Table *table, char *key, char *info);
unsigned int find_last_release(Table *table, unsigned int index, char *key);
KeySpace **find_certain_release(Table *table, char *key, int release, Err *status);
KeySpace **find_all_release(Table *table, char *key, int *count, Err *status);
Err delete_all_release(Table *table, char *key);
Err delete_certain_release(Table *table, char *key, int release);
Err print_table(Table *table);
void print_vector(KeySpace **vector, int size);
void free_table(Table *table);
void free_vector(KeySpace **vector, int size);
Err import_from_file(Table **table, const char *filename);
Err export_in_file(Table *table, const char *filename);
Err resize_table(Table **table);
Err is_table_full(Table *table);
#endif
