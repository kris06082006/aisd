#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tree.h"

#define BATCH_SIZE 1000
#define MAX_KEYS 1000000

void mix_keys(char **keys, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = keys[i];
        keys[i] = keys[j];
        keys[j] = temp;
    }
}

int main(void) {
    struct timespec start, end;
    FILE *csv = NULL, *delcsv = NULL, *keyfile = NULL;
    Tree *tree = NULL;
    char **keys = NULL;
    int count = 0;
    Err status;
// timing for insert
    status = init_tree(&tree);
    if (status != ERR_OK) {
        fprintf(stderr, "Ошибка инициализации дерева: %s\n", error_message(status));
        goto cleanup;
    }

    keyfile = fopen("keys.txt", "r");
    if (!keyfile) {
        perror("Не удалось открыть файл с ключами");
        goto cleanup;
    }

    keys = malloc(MAX_KEYS * sizeof(char*));
    if (!keys) {
        fprintf(stderr, "Ошибка выделения памяти для ключей\n");
        goto cleanup;
    }

    csv = fopen("insert_times.csv", "w");
    if (!csv) {
        perror("Не удалось открыть CSV-файл для вставки");
        goto cleanup;
    }

    char key[128];
    clock_gettime(CLOCK_MONOTONIC, &start);
    while (fgets(key, sizeof(key), keyfile) && count < MAX_KEYS) {
        key[strcspn(key, "\n")] = '\0';
        keys[count] = strdup(key);
        if (!keys[count]) {
            fprintf(stderr, "Ошибка копирования ключа\n");
            goto cleanup;
        }

        status = insert_node(tree, key, "0");
        if (status != ERR_OK) {
            fprintf(stderr, "Ошибка вставки ключа %s: %s\n", key, error_message(status));
        }
        count++;

        if (count % BATCH_SIZE == 0) {
            clock_gettime(CLOCK_MONOTONIC, &end);
            long duration = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            fprintf(csv, "%d,%ld\n", count, duration);
            fflush(csv);
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
    }
    fclose(csv);
    csv = NULL;

    mix_keys(keys, count);

// timing for deletion   
    delcsv = fopen("delete_times.csv", "w");
    if (!delcsv) {
        perror("Не удалось открыть CSV-файл для удаления");
        goto cleanup;
    }

   
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < count; ++i) {
        if (!keys[i]) continue;

        TreeNode *node = search_node_by_key(tree, keys[i], &status);
        if (!node || status != ERR_OK) {
            fprintf(stderr, "Ключ не найден: %s (ошибка: %s)\n", 
                   keys[i], error_message(status));
            continue;
        }

        status = delete_node(tree, node);
        if (status != ERR_OK) {
            fprintf(stderr, "Ошибка удаления ключа %s: %s\n", 
                   keys[i], error_message(status));
            continue;
        }



        if ((i + 1) % BATCH_SIZE == 0 || i == count - 1) {
            clock_gettime(CLOCK_MONOTONIC, &end);
            long duration = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            fprintf(delcsv, "%d,%ld\n", count - i - 1 + 1000, duration);
            fflush(delcsv);
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
    }
    free_tree(tree);
    tree = NULL;

    // timing for find
    csv = fopen("find_times.csv", "w");
    if (!csv) {
        perror("Не удалось открыть CSV-файл для поиска");
        goto cleanup;
    }

    status = init_tree(&tree);
    if (status != ERR_OK) {
        fprintf(stderr, "Ошибка инициализации дерева для поиска: %s\n", error_message(status));
        goto cleanup;
    }

    for (int i = 0; i < count; ++i) {
        status = insert_node(tree, keys[i], "0");
        if (status != ERR_OK) {
            fprintf(stderr, "Ошибка повторной вставки (поиск): %s\n", error_message(status));
            goto cleanup;
        }

        if ((i + 1) % BATCH_SIZE == 0) {
          
            mix_keys(&keys[i - BATCH_SIZE + 1], BATCH_SIZE);

            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j = i - BATCH_SIZE + 1; j <= i; ++j) {
                TreeNode *node = search_node_by_key(tree, keys[j], &status);
                if (!node || status != ERR_OK) {
                    fprintf(stderr, "Ошибка поиска ключа %s: %s\n", keys[j], error_message(status));
                }
            }
            clock_gettime(CLOCK_MONOTONIC, &end);
            long duration = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            fprintf(csv, "%d,%ld\n", i + 1, duration);
            fflush(csv);
        }
    }
    fclose(csv);
    csv = NULL;
    free_tree(tree);
    tree = NULL;

    // timing for traverse
    csv = fopen("traverse_times.csv", "w");
    if (!csv) {
        perror("Не удалось открыть CSV-файл для обхода");
        goto cleanup;
    }

    status = init_tree(&tree);
    if (status != ERR_OK) {
        fprintf(stderr, "Ошибка инициализации дерева для обхода: %s\n", error_message(status));
        goto cleanup;
    }

    for (int i = 0; i < count; ++i) {
        status = insert_node(tree, keys[i], "0");
        if (status != ERR_OK) {
            fprintf(stderr, "Ошибка вставки (обход): %s\n", error_message(status));
            goto cleanup;
        }

        if ((i + 1) % BATCH_SIZE == 0) {
            clock_gettime(CLOCK_MONOTONIC, &start);
            tree_traversal_for_timing(tree);
            clock_gettime(CLOCK_MONOTONIC, &end);
            long duration = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
            fprintf(csv, "%d,%ld\n", i + 1, duration);
            fflush(csv);
        }
    }
    fclose(csv);
    csv = NULL;
    free_tree(tree);
    tree = NULL;


cleanup:
   
    if (keys) {
        for (int i = 0; i < count; ++i) {
            if (keys[i]) free(keys[i]);
        }
        free(keys);
    }
    if (tree) free_tree(tree);
    if (keyfile) fclose(keyfile);
    if (csv) fclose(csv);
    if (delcsv) fclose(delcsv);

    return 0;
}
