//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023
//
// Graph EXAMPLE : This file was created to test specific functions
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"


int main(void)
{

    printf("Example 4\n");

    //graph 3
    Graph *g03 = GraphCreate(6, 0, 1);
    GraphAddWeightedEdge(g03, 1, 2, 3);
    GraphAddWeightedEdge(g03, 1, 4, 5);
    GraphAddWeightedEdge(g03, 3, 4, 10);
    printf("The third graph:\n");
    GraphDisplay(g03);
    printf("Remove edge (1,2)\n");
    GraphRemoveEdge(g03, 1, 2); // this was added to test the remove edge function
    GraphDisplay(g03);
    int check_invariants3 =  GraphCheckInvariants(g03); // this was added to test the check invariants function
    printf("Check invariants (0 = invalid, 1 = valid): %d\n", check_invariants3);


    //graph 3 copy
    Graph *g03Copy = GraphCopy(g03);
    printf("The copy of the third graph:\n");
    GraphDisplay(g03Copy); // this was added to test the copy function
    GraphDestroy(&g03Copy);


    //graph from file
    FILE *f = fopen("./GRAPHS/SWmediumDG.txt", "r");
    if (f == NULL) {
        perror("fopen");
        exit(2);
    }
    Graph* graphFile = GraphFromFile(f); // this was added to test the file function
    fclose(f);
    printf("The graph from file:\n");
    GraphDisplay(graphFile);
    int check_invariantsfile =  GraphCheckInvariants(graphFile); 
    printf("Check invariants (0 = invalid, 1 = valid): %d\n", check_invariantsfile);
    GraphDestroy(&graphFile);


    //graph 4 (directed)
    Graph *dig04 = GraphCreate(6, 1, 1);
    GraphAddWeightedEdge(dig04, 1, 2, 3);
    GraphAddWeightedEdge(dig04, 1, 4, 5);
    GraphAddWeightedEdge(dig04, 3, 4, 10);
    GraphAddWeightedEdge(dig04, 1, 5, 10);
    printf("The fourth graph:\n");
    GraphDisplay(dig04);
    int check_invariants4 =  GraphCheckInvariants(dig04); 
    printf("Check invariants (0 = invalid, 1 = valid): %d\n", check_invariants4);

    GraphDestroy(&g03);
    GraphDestroy(&dig04);
    return 0;
}
