#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "cases.h"
#include "graph.h"
#include "algorithms.h"

Err adding_node(Graph* graph) {
    char* name = NULL; 
    char* err = NULL; 
    unsigned port = 0;
    Err status = ERR_OK;

    name = readline("Enter name: ");
    if (name == NULL) return ERR_EOF;

    printf("Enter port: ");
    status = input_unsigned_int(&port);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    status = add_node(graph, name, port);
    err = error_message(status);
    printf("%s", err);

    free(name);
    return status;
}

Err adding_edge(Graph* graph) {
    char* err = NULL; 
    size_t from = 0, to = 0;
    Err status = ERR_OK;

    unsigned* ports = malloc(sizeof(unsigned)); 
    size_t ports_size = 0; 
    
    printf("Enter index of 'from' vertex: ");
    status = input_unsigned_int((unsigned*)&from);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    printf("Enter index of 'to' vertex: "); 
    status = input_unsigned_int((unsigned*)&to);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    while (status == ERR_OK) {
        printf("Enter new available for this route port (enter 0, to stop): "); 
        status = input_unsigned_int(&(ports[ports_size++]));
        if (status == ERR_EOF) return status; 
        if (ports[ports_size - 1] == 0) break;  
        unsigned* tmp = realloc(ports, sizeof(unsigned)*(ports_size+1)); 
        if (tmp == NULL) { 
            free(ports); 
            return ERR_MEM; 
        } 
        ports = tmp; 
    } 

    status = add_edge(graph, from, to, ports, ports_size);
    err = error_message(status);
    printf("%s", err);

    free(ports);
    return status;
}

Err deleting_edge(Graph* graph) {
    char* err = NULL; 
    size_t from = 0, to = 0;
    Err status = ERR_OK;

    printf("Enter index of 'from' vertex: ");
    status = input_unsigned_int((unsigned*)&from);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    printf("Enter index of 'to' vertex: "); 
    status = input_unsigned_int((unsigned*)&to);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    status = delete_edge(graph, from, to);
    err = error_message(status);
    printf("%s", err);

    return status;
}

Err deleting_node(Graph* graph) {
    char* err = NULL; 
    unsigned index = 0;
    Err status = ERR_OK;

    printf("Enter index: ");
    status = input_unsigned_int(&index);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    status = delete_node(graph, index);
    err = error_message(status);
    printf("%s", err);

    return status;
}
 
Err index_output(Graph* graph) { 
    if (graph == NULL || graph->vec == NULL) {
        return ERR_NOT_INIT; 
    } 

    for (size_t i = 0; i < graph->size; i++) {
        Node node = graph->vec[i]; 
        printf("[%zu] - %s (port = %u):", i, node.name, node.port); 
        Edge* edge = node.edge; 
        while (edge) {
            printf(" %zu", edge->to); 
            edge = edge->next; 
        }
        printf("\n");  
    } 

    return ERR_OK; 
}

Err graphical_output(Graph* graph) { 
    FILE* file = fopen("graph.dot", "w"); 
    if (file == NULL) {
        perror("fopen() failed");
        return ERR_OPEN;  
    } 

    Err status = fill_dot(graph, file);
    char *err = error_message(status);
    printf("%s", err);
    if (status != ERR_OK) return status;
    
    fclose(file);  
   
    int result = system("dot -Tpng graph.dot -o graph.png");
    if (result != 0) {
        printf("%sGraphviz launch error\n", error_message(ERR_SYS)); 
        return ERR_SYS;
    }
   
    return ERR_OK;
}

Err searching_edge(Graph* graph) {
    char* err = NULL; 
    size_t from = 0, to = 0;
    Err status = ERR_OK;

    printf("Enter index of 'from' vertex: ");
    status = input_unsigned_int((unsigned*)&from);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    printf("Enter index of 'to' vertex: "); 
    status = input_unsigned_int((unsigned*)&to);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    Edge* edge = search_edge(graph, from, to);
    if (edge == NULL) {
        printf("No such edge"); 
        return ERR_FIND; 
    } 

    if (edge->ports == NULL) {
        printf("There is no availabla ports!\n"); 
    } 
    
    printf("Ports available on this connection:"); 
    for (size_t i = 0; i < edge->ports_size; i++) {
        printf(" %u", edge->ports[i]); 
    } 
    printf("\n");

    return ERR_OK;
}

Err searching_node(Graph* graph) {
    char* err = NULL; 
    size_t index = 0;
    Err status = ERR_OK;

    printf("Enter index of vertex: "); 
    status = input_unsigned_int((unsigned*)&index);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    Node* node = search_node(graph, index);
    if (node == NULL) {
        printf("No such vertex"); 
        return ERR_FIND; 
    } 
    
    printf("Found vertex with index %zu: name - %s, port - %u", index, node->name, node->port); 

    return ERR_OK;
}

Err changing_node(Graph* graph) {
    char* name = NULL; 
    char* err = NULL; 
    unsigned port = 0; 
    size_t index = 0;
    Err status = ERR_OK;

    printf("Enter index of vertex, you would like to change: "); 
    status = input_unsigned_int((unsigned *) &index);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    Node* node = search_node(graph, index);
    if (node == NULL) {
        return ERR_FIND; 
    }  

    printf("Enter new name (old name - %s): ", node->name); 
    name = readline("");
    if (name == NULL) return ERR_EOF;
    
    printf("Enter new port (old port - %u): ", node->port);
    status = input_unsigned_int(&port);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    free(node->name); 
    node->name = name; 
    node->port = port; 

    return status;
}

Err changing_edge(Graph* graph) {
    char* err = NULL; 
    size_t from = 0, to = 0;
    Err status = ERR_OK;
 
    printf("Enter index of 'from' vertex: ");
    status = input_unsigned_int((unsigned*)&from);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    printf("Enter index of 'to' vertex: "); 
    status = input_unsigned_int((unsigned*)&to);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    Edge* edge = search_edge(graph, from, to); 
    if (edge == NULL) {
        return ERR_FIND; 
    } 
    
    unsigned* ports = malloc(sizeof(unsigned)); 
    size_t ports_size = 0; 

    printf("Old ports on this connection: ");   
    for (size_t i = 0; i < edge->ports_size; i++) {
        printf("%d ", edge->ports[i]); 
    } 
    printf("\n"); 

    while (status == ERR_OK) {
        printf("Enter new available for this route port (enter 0, to stop): "); 
        status = input_unsigned_int(&(ports[ports_size++]));
        if (status == ERR_EOF) return status; 
        if (ports[ports_size - 1] == 0) break;  
        unsigned* tmp = realloc(ports, sizeof(unsigned)*(ports_size+1)); 
        if (tmp == NULL) { 
            free(ports); 
            return ERR_MEM; 
        } 
        ports = tmp; 
    } 

    free(edge->ports); 
    edge->ports = ports; 
    edge->ports_size = ports_size; 

    return status;
}

Err find_nearest(Graph* graph) {
    char* err = NULL; 
    size_t index = 0;
    unsigned goal_port = 0; 
    Err status = ERR_OK;

    printf("Enter index of start vertex: "); 
    status = input_unsigned_int((unsigned*)&index);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    printf("Enter index of port, you want to reach: "); 
    status = input_unsigned_int(&goal_port);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    Node* node = BFS(graph, index, goal_port);
    if (node == NULL) {
        printf("No such vertex"); 
        return ERR_FIND; 
    } 
    
    printf("Found vertex with index %zu: name - %s, port - %u", index, node->name, node->port); 

    return ERR_OK;
}

Err shortest_path(Graph* graph) {
    char* err = NULL; 
    size_t from = 0, to = 0;
    Err status = ERR_OK;

    printf("Enter index of 'from' vertex: ");
    status = input_unsigned_int((unsigned*)&from);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    printf("Enter index of 'to' vertex: "); 
    status = input_unsigned_int((unsigned*)&to);
    err = error_message(status);
    printf("%s",err);
    if (status != ERR_OK) return status;

    size_t* ans = bellman(graph, from, to);
    if (ans == NULL) {
        printf("No path!\n"); 
    } 

    printf("The shortest path is: "); 
    size_t i = 0; 
    while (ans[i] != to) {
        printf("%s[%zu] ", graph->vec[ans[i]].name, ans[i]);
        i += 1;  
    } 
    printf("%s[%zu] ", graph->vec[ans[i]].name, ans[i]); 

    free(ans); 
    return status;
}
  
