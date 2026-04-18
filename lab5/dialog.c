#include "dialog.h"
#include "functions.h"
#include "graph.h"
#include "cases.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

Err dialog() {
    Graph* graph = graph_init();
    if (graph == NULL) {
        return ERR_MEM; 
    } 

    int option; 
    Err status = ERR_OK;

    while (status != ERR_EOF) {
        printf("\n--- Menu ---\n");
        printf("1.  Add new vertex to system\n"); 
        printf("2.  Add new connection between vertexes\n");
        printf("3.  Info about vertex by index in the system\n");
        printf("4.  Info about connection between two vertexes\n");
        printf("5.  Delete vertex\n");
        printf("6.  Delete connection\n");
        printf("7.  Change name and id of vertex\n"); 
        printf("8.  Change ports of connections\n");
        printf("9.  Print graph in 'Person' - 'Index' style\n"); 
        printf("10. Visualization of graph\n"); 
        printf("11. Find nearest computer with setted port\n");  
        printf("12. Find shortest path between two computers\n"); 
        printf("0.  Exit\n");
        printf("Choose the option number: ");
    
        status = input_int(&option);
        char *err = error_message(status);
        printf("%s", err);
        if (status == ERR_EOF) goto clean_and_exit;
    
        switch (option) {
        
        case 1: 
            status = adding_node(graph);  
            if (status == ERR_EOF) goto clean_and_exit;
            break; 

        case 2: 
            status = adding_edge(graph);  
            if (status == ERR_EOF) goto clean_and_exit;
            break; 
       
        case 3: 
            status = searching_node(graph);  
            if (status == ERR_EOF) goto clean_and_exit;
            break;         
        
        case 4: 
            status = searching_edge(graph);  
            if (status == ERR_EOF) goto clean_and_exit;
            break; 
 
        case 5: 
            status = deleting_node(graph);  
            if (status == ERR_EOF) goto clean_and_exit;
            break;    
     
        case 6: 
            status = deleting_edge(graph);  
            if (status == ERR_EOF) goto clean_and_exit;
            break;         

        case 7: 
            status = changing_node(graph); 
            if (status == ERR_EOF) goto clean_and_exit;
            break; 

        case 8: 
            status = changing_edge(graph);
            if (status == ERR_EOF) goto clean_and_exit; 
            break; 

        case 9: 
            status = index_output(graph); 
            if (status == ERR_EOF) goto clean_and_exit;
            break; 

        case 10: 
            status = graphical_output(graph);
            if (status == ERR_EOF) goto clean_and_exit; 
            break;   

        case 11: 
            status = find_nearest(graph); 
            if (status == ERR_EOF) goto clean_and_exit; 
            break; 

        case 12: 
            status = shortest_path(graph); 
            if (status == ERR_EOF) goto clean_and_exit; 
            break; 

        case 0:
            printf("Exiting the program.\n");
            free_graph(graph);
            return ERR_OK;
        
        default:
            printf("Invalid choice. Try again.\n");
            break;      
        }
    }

clean_and_exit:
    free_graph(graph);
    return ERR_EOF;
}

