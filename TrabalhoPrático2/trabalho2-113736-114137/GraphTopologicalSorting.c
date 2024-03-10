//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Topological Sorting
//

#include "GraphTopologicalSorting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersQueue.h"
#include "instrumentation.h"

struct _GraphTopoSort
{
  int *marked;                    // Aux array
  unsigned int *numIncomingEdges; // Aux array
  unsigned int *vertexSequence;   // The result
  int validResult;                // 0 or 1
  unsigned int numVertices;       // From the graph
  Graph *graph;
};

// AUXILIARY FUNCTION
// Allocate memory for the struct
// And for its array fields
// Initialize all struct fields
//
static GraphTopoSort *_create(Graph *g)
{
  assert(g != NULL);

  GraphTopoSort *p = NULL;

  // Allocate memory for the struct
  p = (GraphTopoSort *)malloc(sizeof(GraphTopoSort));
  assert(p != NULL);

  // Initialize all struct fields
  p->numVertices = GraphGetNumVertices(g);

  p->graph = g;

  // Allocate memory and initialize to zero
  p->marked = (int *)calloc(p->numVertices, sizeof(int));
  assert(p->marked != NULL);

  // Allocate memory and initialize to zero
  p->numIncomingEdges = (unsigned int *)calloc(p->numVertices, sizeof(int));
  assert(p->numIncomingEdges != NULL);

  // Allocate memory
  p->vertexSequence = (unsigned int *)malloc(p->numVertices * sizeof(int));
  assert(p->vertexSequence != NULL);

  // Initialize to zero
  p->validResult = 0;

  return p;
}

//
// Computing the topological sorting, if any, using the 1st algorithm:
// 1 - Create a copy of the graph
// 2 - Successively identify vertices without incoming edges and remove their
//     outgoing edges
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort * GraphTopoSortComputeV1(Graph * g) { // uses the copy of the graph
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort * topoSort = _create(g);

  // Build the topological sorting

  // COMPLETED

  Graph * copy = GraphCopy(g);

  // Varuable to store the number of vertices to use in the while loop
  int n_vertices = topoSort -> numVertices;
  // Counter to later compare with the number of vertices
  unsigned int counter = 0;

  while (n_vertices > 0) {

    int found = 0;

    for (unsigned int i = 0; i < topoSort -> numVertices; i++) {

      // If the vertex has no incoming edges and is not marked
      if (GraphGetVertexInDegree(copy, i) == 0 && !topoSort -> marked[i]) {
        // Remove outgoing edges
        unsigned int * adjacents = GraphGetAdjacentsTo(copy, i);
        for (unsigned int j = 1; j <= adjacents[0]; j++) {
          GraphRemoveEdge(copy, i, adjacents[j]);
        }

        free(adjacents);
        topoSort -> vertexSequence[counter++] = i;
        topoSort -> marked[i] = 1;
        n_vertices--;
        found = 1;
        break;
      }
    }
    // If no vertex was found, the graph is not a DAG
    if (!found) {
      topoSort -> validResult = 0;
      GraphDestroy(&copy);
      return topoSort;
    }
  }

  // If the counter is equal to the number of vertices, the sorting is valid
  if (topoSort -> numVertices == counter) {
    topoSort -> validResult = 1;
  }

  // Housekeeping
  GraphDestroy(&copy);
  return topoSort;
}
//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort * GraphTopoSortComputeV2(Graph * g) { // uses the aux array numIncomingEdges

  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort * topoSort = _create(g);

  // Build the topological sorting

  // COMPLETED

  // Initializing the array with the number of incoming edges for each vertex
  for (unsigned int i = 0; i < topoSort -> numVertices; i++) {
    topoSort -> numIncomingEdges[i] = GraphGetVertexInDegree(g, i);
  }

  // Variable to store the number of vertices to use in the while loop
  int n_vertices = topoSort -> numVertices;
  // Counter to later compare with the number of vertices
  unsigned int counter = 0;
  while (n_vertices > 0) {

    int found = 0;
    for (unsigned int i = 0; i < topoSort -> numVertices; i++) {
      // If the vertex has no incoming edges and is not marked
      if (topoSort -> numIncomingEdges[i] == 0 && !topoSort -> marked[i]) {
        // Remove outgoing edges
        unsigned int * adjacents = GraphGetAdjacentsTo(g, i);
        for (unsigned int j = 1; j <= adjacents[0]; j++) {
          topoSort -> numIncomingEdges[adjacents[j]]--;
        }
        free(adjacents);
        topoSort -> marked[i] = 1;
        topoSort -> vertexSequence[counter++] = i;
        n_vertices--;
        found = 1;
        break;
      }
    }
    // If no vertex was found, the graph is not a DAG
    if (!found) {
      topoSort -> validResult = 0;
      return topoSort;
    }
  }

  if (topoSort -> numVertices == counter) {
    topoSort -> validResult = 1;
  } 

  return topoSort;
}

//
// Computing the topological sorting, if any, using the 3rd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort *GraphTopoSortComputeV3(Graph *g){ // uses a queue
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort *topoSort = _create(g);

  // Build the topological sorting

  // TO BE COMPLETED

  // Create an array to store the incoming edges count for each vertex
  unsigned int *incomingEdgesCount = (unsigned int *)calloc(topoSort->numVertices, sizeof(unsigned int));

  // Initialize the incoming edges count array
  for (unsigned int i = 0; i < topoSort->numVertices; ++i) {
    incomingEdgesCount[i] = GraphGetVertexInDegree(g, i);
  }

  // Create a queue
  Queue *queue = QueueCreate(topoSort->numVertices);
  unsigned int counter = 0;

  // Loop to identify vertices without incoming edges and remove outgoing edges
  int found = 0;
  while (counter < topoSort->numVertices) {
    found = 0;
    for (unsigned int i = 0; i < topoSort->numVertices; ++i) {
      if (incomingEdgesCount[i] == 0 && !topoSort->marked[i]) {
        // Remove outgoing edges
        unsigned int *adjacents = GraphGetAdjacentsTo(g, i);
        for (unsigned int j = 1; j <= adjacents[0]; ++j) {
          --incomingEdgesCount[adjacents[j]];
        }
        free(adjacents);
        topoSort->marked[i] = 1;                          // Mark the vertex as visited
        topoSort->vertexSequence[counter++] = i;          // Add the vertex to the sorting sequence
        QueueEnqueue(queue, i);                           // Add the vertex to the queue
        found = 1;
        break;
      }
    }
    // If no vertex was found, the graph is not a DAG
    if (!found) {
      topoSort->validResult = 0;
      free(incomingEdgesCount);
      QueueDestroy(&queue);
      return topoSort;
    }
  }

  unsigned int queueSize = QueueSize(queue);
  if (queueSize  == counter) {
    topoSort -> validResult = 1;
  } 
  else {
    GraphTopoSortDestroy(&topoSort);
  }

  // Clean up
  free(incomingEdgesCount);
  QueueDestroy(&queue);

  return topoSort;
}

//---------------------------------------------------------------------------------------

void GraphTopoSortDestroy(GraphTopoSort **p)
{
  assert(*p != NULL);

  GraphTopoSort *aux = *p;

  free(aux->marked);
  free(aux->numIncomingEdges);
  free(aux->vertexSequence);

  free(*p);
  *p = NULL;
}

//
// A valid sorting was computed?
//
int GraphTopoSortIsValid(const GraphTopoSort *p) { return p->validResult; }

//
// Getting an array containing the topological sequence of vertex indices
// Or NULL, if no sequence could be computed
// MEMORY IS ALLOCATED FOR THE RESULTING ARRAY
//
unsigned int *GraphTopoSortGetSequence(const GraphTopoSort *p)
{
  assert(p != NULL);

  // COMPLETED

  // If the sorting is not valid, return NULL
  if (p->validResult == 0)
  {
    return NULL;
  }

  // If the sorting is valid, return the sequence
  return p->vertexSequence;
}

// DISPLAYING on the console

//
// The toplogical sequence of vertex indices, if it could be computed
//
void GraphTopoSortDisplaySequence(const GraphTopoSort *p)
{
  assert(p != NULL);

  if (p->validResult == 0)
  {
    printf(" *** The topological sorting could not be computed!! *** \n");
    return;
  }

  printf("Topological Sorting - Vertex indices:\n");
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++)
  {
    printf("%d ", p->vertexSequence[i]);
  }
  printf("\n");
}

//
// The toplogical sequence of vertex indices, if it could be computed
// Followed by the digraph displayed using the adjecency lists
// Adjacency lists are presented in topologic sorted order
//
void GraphTopoSortDisplay(const GraphTopoSort *p)
{
  assert(p != NULL);

  // The topological order
  GraphTopoSortDisplaySequence(p);

  if (p->validResult == 0)
  {
    return;
  }

  // The Digraph
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++)
  {
    GraphListAdjacents(p->graph, p->vertexSequence[i]);
  }
  printf("\n");
}
