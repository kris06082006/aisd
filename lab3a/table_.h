#ifndef TABLE_H
#define TABLE_H

#include "functions.h"

typedef struct KeySpace {
    unsigned int key;
    int release;
    unsigned int info;
    struct KeySpace *next;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
} Table;

Err init_table(Table **table);
Err add_elem(Table *table, unsigned int key, unsigned int info);
KeySpace *find_last_release(Table *table, unsigned int key);
Table *find_certain_release(Table *table, unsigned int key, int release, Err *status);
Err delete_all_release(Table *table, unsigned int key);
Err delete_certain_release(Table *table, unsigned int key, int release);
Table *find_all_release(Table *table, unsigned int key, Err *status);
Err print_table(Table *table);
void free_table(Table *table);
Err import_from_file(Table **table, const char *filename);

#endif
