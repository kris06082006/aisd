#ifndef ALGORITHMS_H 
#define ALGORITHMS_H 

#include "graph.h"

#include <stddef.h>

Node* BFS(Graph* graph, size_t start, unsigned goal_port);  
size_t* bellman(Graph* graph, size_t start, size_t target); 

#endif

