#include "functions.h"
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX

typedef enum Color {
    white = 0, 
    black
} Color; 

static bool connection(Edge* edge, unsigned port) {
    for (size_t i = 0; i < edge->ports_size; i++) {
        if (edge->ports[i] == port) return true; 
    } 
    return false; 
} 

Node* BFS(Graph* graph, size_t start, unsigned goal_port) {
    if (graph == NULL || graph->vec == NULL) return NULL;

    Node* node = search_node(graph, start); 
    if (node == NULL) return NULL; 

    Node** queue = malloc(sizeof(Node*)*graph->size);
    if (queue == NULL) return NULL; 

    Color* color = calloc(graph->size, sizeof(Color));
    if (color == NULL) {
        free(queue);
        return NULL;  
    } 

    unsigned communication_port = node->port; 
    size_t head = 0; 
    size_t tail = 1;    
    queue[0] = node; 

    while (head != tail) {
        Node* now_node = queue[head++]; 
        if (now_node->port == goal_port) {
            free(queue); 
            free(color); 
            return now_node; 
        } 

        Edge* edge = now_node->edge; 
        while (edge) {
            if (color[edge->to] == white && connection(edge, communication_port) == true) {
                queue[tail++] = search_node(graph, edge->to); 
                color[edge->to] = black; 
            } 
            edge = edge->next; 
        } 
    } 

    free(queue); 
    free(color); 
    
    return NULL; 
} 

// Bellman: 

static size_t* restore_path(int* prev, size_t start, size_t target) {
    size_t len = 1; 
    size_t node = target; 
    while (node != start) {
        len += 1; 
        node = prev[node]; 
    }     
    
    size_t* ans = malloc(sizeof(size_t) * len);  
    if (ans != NULL) {
        node = target; 
        size_t i = len - 1; 
        while (node != start) {
            ans[i--] = node; 
            node = prev[node]; 
        } 
        ans[0] = start;  
    } 
    return ans; 
}

size_t* bellman(Graph* graph, size_t start, size_t target) {
    if (graph == NULL || graph->vec == NULL) {
        return NULL; 
    } 
    
    Node* start_node = search_node(graph, start); 
    if (start_node == NULL) {
        return NULL; 
    } 

    unsigned* dist = malloc(sizeof(unsigned) * graph->size); 
    int* prev = malloc(sizeof(int) * graph->size); 
    if (dist == NULL || prev == NULL) {
        free(prev); 
        free(dist); 
        return NULL;
    } 

    size_t communication_port = start_node->port; 

    for (size_t i = 0; i < graph->size; i++) {
        dist[i] = INF;
        prev[i] = -1; 
    } 

    dist[start] = 0; 

    for (size_t i = 1; i < graph->size; i++) {
        for (size_t u = 0; u < graph->size; u++) {
            Edge* edge = graph->vec[u].edge; 
            while (edge != NULL) {
                size_t to = edge->to;                
                if (dist[u] != INF && dist[u] + 1 < dist[to] && connection(edge, communication_port)) {
                    dist[to] = dist[u] + 1; 
                    prev[to] = u; 
                } 
                edge = edge->next;  
            }    
        }
    }  
   
    size_t* ans = restore_path(prev, start, target);  

    free(dist); 
    free(prev); 

    return ans; 
}


