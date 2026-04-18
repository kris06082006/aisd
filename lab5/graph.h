#ifndef GRAPH_H
#define GRAPH_H

#include "functions.h"
#include <stdio.h>

typedef struct Edge { 
    size_t to;  
    unsigned* ports;
    size_t ports_size; 
    struct Edge* next; 
} Edge; 

typedef struct Node {
    size_t index; 
    char* name; 
    unsigned port; 
    Edge* edge; 
} Node; 

typedef struct Graph {
    size_t size; 
    size_t capacity; 
    Node* vec; 
} Graph; 

Graph* graph_init(); 
Err add_node(Graph* graph, char* name, unsigned port);  
Err add_edge(Graph* graph, size_t from, size_t to, unsigned* ports, size_t ports_size); 
Err delete_node(Graph* graph, size_t node_index); 
Err delete_edge(Graph* graph, size_t from, size_t to); 
Node* search_node(Graph* graph, size_t index); 
Edge* search_edge(Graph* graph, size_t from, size_t to); 
void free_graph(Graph* graph);
Err fill_dot(Graph* graph, FILE* file);  

#endif

