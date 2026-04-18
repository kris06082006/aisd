#include "graph.h"
#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SIZE 10

Graph* graph_init() {
    Graph* graph = malloc(sizeof(Graph)); 
    if (graph == NULL) { 
        return NULL; 
    }

    graph->size = 0;
    graph->capacity = START_SIZE;  
    graph->vec = calloc(START_SIZE, sizeof(Node)); 

    return graph; 
} 

Err add_node(Graph* graph, char* name, unsigned port) {
    if (graph == NULL || graph->vec == NULL) {
        return ERR_NOT_INIT; 
    } 

    if (graph->size == graph->capacity) {
        graph->capacity *= 2; 
        Node* tmp = realloc(graph->vec, graph->capacity*(sizeof(Node))); 
        if (tmp == NULL) {
            return ERR_MEM; 
        } 
        graph->vec = tmp; 
    } 

    Node* new_node = &(graph->vec[graph->size]); 

    new_node->port = port; 
    new_node->name = strdup(name);
    if (new_node->name == NULL) {
        return ERR_MEM;  
    }   
    
    new_node->index = graph->size; 
    graph->size += 1;     

    return ERR_OK; 
} 

Err add_edge(Graph* graph, size_t from, size_t to, unsigned* ports, size_t ports_size) {
    if (graph == NULL || graph->vec == NULL) {
        return ERR_NOT_INIT; 
    } 
    if (ports == NULL || from == to) {
        return ERR_GARBAGE; 
    } 

    Node* from_node = search_node(graph, from); 
    if (from_node == NULL || search_node(graph, to) == NULL || search_edge(graph, from, to) != NULL) {
        return ERR_GARBAGE; 
    } 

    Edge* new_edge = calloc(1, sizeof(Edge)); 
    if (new_edge == NULL) {
        return ERR_MEM; 
    } 
    
    new_edge->ports_size = ports_size; 
    new_edge->ports = malloc(sizeof(unsigned)*ports_size); 
    if (new_edge->ports == NULL && ports_size != 0) {
        free(new_edge); 
        return ERR_MEM; 
    } 
    
    for (size_t i = 0; i < ports_size; i++) {
        new_edge->ports[i] = ports[i]; 
    } 

    new_edge->to   = to;  
    new_edge->next = from_node->edge; 

    from_node->edge = new_edge; 
    return ERR_OK; 
} 

Err delete_node(Graph* graph, size_t node_index) {
    Node* del_node; 
    if (graph == NULL || graph->vec == NULL) {
        return ERR_NOT_INIT; 
    } 
    if ((del_node = search_node(graph, node_index)) == NULL) {
        return ERR_GARBAGE; 
    } 

    Edge* tmp;     
    while (del_node->edge) {
        tmp = del_node->edge; 
        del_node->edge = del_node->edge->next; 
        free(tmp->ports); 
        free(tmp); 
    } 
    free(del_node->name); 

    Node* switch_node = search_node(graph, graph->size - 1); 
    memmove(del_node, switch_node, sizeof(Node));   
 
    for (size_t i = 0; i < graph->size; i++) {
        delete_edge(graph, i, node_index); 
    } 

    for (size_t i = 0; i < graph->size; i++) {
        Edge* del_edge = search_edge(graph, i, graph->size - 1);
        if (del_edge != NULL) {
            del_edge->to = node_index; 
        } 
    } 

    graph->size -= 1;
    return ERR_OK;  
} 

Err delete_edge(Graph* graph, size_t from, size_t to) {
    Node* node;
    if (graph == NULL || graph->vec == NULL) {
        return ERR_NOT_INIT; 
    } 

    if ((node = search_node(graph, from)) == NULL || node->edge == NULL) {
        return ERR_GARBAGE; 
    } 

    if (node->edge->to == to) { 
        Edge* tmp = node->edge; 
        node->edge = node->edge->next; 
        if (tmp->ports_size != 0) {
            free(tmp->ports); 
        } 
        free(tmp); 
        return ERR_OK;
    } 
    
    Edge* edge = node->edge; 
    while (edge->next) {
        if (edge->next->to == to) {
            Edge* tmp = edge->next; 
            edge->next = edge->next->next; 
            free(tmp->ports); 
            free(tmp); 
            return ERR_OK;
        } 
        edge = edge->next;  
    } 

    return ERR_GARBAGE; 
} 

Node* search_node(Graph* graph, size_t index) {
    return (graph == NULL || graph->vec == NULL || graph->size <= index) ? NULL : graph->vec + index; 
} 

Edge* search_edge(Graph* graph, size_t from, size_t to) {
    Node* node = search_node(graph, from); 
    if (node == NULL) {
        return NULL; 
    } 

    Edge* edge = node->edge; 
    while (edge) {
        if (edge->to == to) { 
            return edge; 
        } 
        edge = edge->next; 
    } 

    return NULL; 
}

void free_graph(Graph* graph) {
    if (graph == NULL) return ; 
    if (graph->vec == NULL) {
        free(graph); 
        return ; 
    } 

    for (size_t i = 0; i < graph->size; i++) {
        for (size_t j = 0; j < graph->size; j++) {
            delete_edge(graph, i, j); 
        }
    } 

    for (size_t i = graph->size; i > 0; i--) {
        delete_node(graph, i-1); 
    }    

    free(graph->vec);  
    free(graph); 
}  

 
Err fill_dot(Graph* graph, FILE* file) {
    fprintf(file, "digraph Network {\n"); 
    fprintf(file, "\trankdir=\"LR\"; // top to bottom\n"); 
    fprintf(file, "\tnode [shape=circle, fontcolor=\"black\", wportth=1.2];\n"); 
    fprintf(file, "\tedge [penwportth=2];\n"); 

    for (size_t i = 0; i < graph->size; i++) {
        Node node = graph->vec[i];
        Edge* edge = node.edge; 
        if (edge != NULL) {
            while (edge != NULL) {
                Node from_node = graph->vec[edge->to]; 
                fprintf(file, "\t\"%s\" -> \"%s\" [color=\"#40826D\"];\n", node.name, from_node.name); 
                edge = edge->next; 
            } 
        } else {
            fprintf(file, "\t\"%s\"; \n", node.name);
        }  
    } 
    fprintf(file, "}\n");  

    return ERR_OK; 
}

