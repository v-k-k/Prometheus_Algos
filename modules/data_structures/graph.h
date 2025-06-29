#ifndef GRAPH_H
#define GRAPH_H

#include "../../helpers/tools/utils.h"

// Function to create a graph with V vertices (MODIFIED to initialize new fields)
struct Graph* createGraph(int numVertices);

// Function to create a transpose graph (NEW)
struct Graph* getTransposeGraph(struct Graph* originalGraph);

/**
 * @brief Finds the maximum vertex number from a list of edges.
 * This function is designed to work with a 1D array of integers
 * that stores edges as consecutive pairs (src, dest).
 *
 * @param edges A pointer to the beginning of the contiguous integer array
 * containing the source and destination vertices for all edges.
 * @param numEdges The total number of edges in the `edges` array.
 * @param elementsPerEdge The number of integers per edge (e.g., 2 for src and dest).
 * @return The highest vertex ID found in the `edges` list.
 */
int findMaxVertex(int* edges, int numEdges, int elementsPerEdge);

/**
 * @brief Adds a directed edge from a source vertex to a destination vertex in the graph.
 * The edge is added to the adjacency list of the source vertex.
 *
 * @param graph A pointer to the Graph structure.
 * @param src The source vertex of the directed edge.
 * @param dest The destination vertex of the directed edge.
 */
void addEdge(struct Graph* graph, int src, int dest);

// And the helper createNode function, which addEdge depends on:
/**
 * @brief Creates a new adjacency list node.
 *
 * @param dest The destination vertex for this node.
 * @return A pointer to the newly created AdjUNode. Exits on memory allocation failure.
 */
struct AdjUNode* createNode(int dest);

// DFS function for Kosaraju's first pass (fills dfsOrder) (NEW)
void dfs_first_pass(struct Graph* graph, int v);

// DFS function for Kosaraju's second pass (counts SCC size) (NEW)
int dfs_second_pass(struct Graph* graph, int v);

// Main function to count Strong Connected Components sizes (Kosaraju's Algorithm) (NEW)
// Returns a dynamically allocated array of SCC sizes, sorted.
// The caller is responsible for freeing this array.
int* countStrongConnectedComponentsSize(struct Graph* originalGraph, int* numComponents);

// Function to free graph memory (MODIFIED to free new fields)
void freeGraph(struct Graph* graph);

#endif