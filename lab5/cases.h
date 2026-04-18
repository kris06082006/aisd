#ifndef CASES_H
#define CASES_H 

#include "functions.h"
#include "graph.h"

Err adding_edge(Graph* graph);  
Err adding_node(Graph* graph); 
Err deleting_edge(Graph* graph); 
Err deleting_node(Graph* graph);
Err searching_node(Graph* graph); 
Err searching_edge(Graph* graph);
Err changing_node(Graph* graph); 
Err changing_edge(Graph* graph); 
Err index_output(Graph* graph); 
Err graphical_output(Graph* graph); 
Err find_nearest(Graph* graph); 
Err shortest_path(Graph* graph); 

#endif 

