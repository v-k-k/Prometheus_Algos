#include "graph.h"

struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    if (graph == NULL) {
        perror("Failed to allocate memory for Graph");
        exit(EXIT_FAILURE);
    }
    graph->numVertices = numVertices;

    graph->adjLists = (struct AdjUNode**)malloc((numVertices + 1) * sizeof(struct AdjUNode*));
    if (graph->adjLists == NULL) {
        perror("Failed to allocate memory for adjLists");
        free(graph); // Clean up graph struct if adjLists fails
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= numVertices; i++) {
        graph->adjLists[i] = NULL;
    }

    // Initialize Kosaraju related arrays
    graph->visited = (int*)calloc(numVertices + 1, sizeof(int)); // Calloc initializes to 0
    if (graph->visited == NULL) {
        perror("Failed to allocate memory for visited array");
        // Clean up previous allocations
        free(graph->adjLists);
        free(graph);
        exit(EXIT_FAILURE);
    }

    graph->dfsOrder = (int*)malloc((numVertices + 1) * sizeof(int));
    if (graph->dfsOrder == NULL) {
        perror("Failed to allocate memory for dfsOrder array");
        // Clean up previous allocations
        free(graph->visited);
        free(graph->adjLists);
        free(graph);
        exit(EXIT_FAILURE);
    }
    // Initialize dfsOrderIndex. We fill dfsOrder from numVertices down to 1.
    graph->dfsOrderIndex = numVertices;

    return graph;
}

struct Graph* getTransposeGraph(struct Graph* originalGraph) {
    // Note: createGraph handles allocation for visited and dfsOrder arrays,
    // which are not strictly needed for the transposed graph's *functionality*
    // but are part of its structure. They will be re-used/reset for the second DFS.
    struct Graph* transposedGraph = createGraph(originalGraph->numVertices);
    if (transposedGraph == NULL) {
        perror("Failed to create transposed graph");
        exit(EXIT_FAILURE);
    }

    // Iterate through all vertices and their adjacency lists in the original graph
    for (int v = 1; v <= originalGraph->numVertices; v++) {
        struct AdjUNode* temp = originalGraph->adjLists[v];
        while (temp) {
            // Add an edge from destination to source in the transposed graph
            // This reverses the direction of the original edge (v -> temp->dest)
            addEdge(transposedGraph, temp->dest, v);
            temp = temp->next;
        }
    }
    return transposedGraph;
}

int findMaxVertex(int* edges, int numEdges, int elementsPerEdge) {
    int maxVertex = 0; // Initialize with 0, as vertex IDs are typically positive.
    for (int i = 0; i < numEdges; i++) {
        // Access source vertex: (i * elementsPerEdge + 0)
        int u = edges[i * elementsPerEdge + 0];
        // Access destination vertex: (i * elementsPerEdge + 1)
        int v = edges[i * elementsPerEdge + 1];

        // Update maxVertex if a larger ID is found
        if (u > maxVertex) {
            maxVertex = u;
        }
        if (v > maxVertex) {
            maxVertex = v;
        }
    }
    return maxVertex;
}

void addEdge(struct Graph* graph, int src, int dest) {
    // Input validation: ensure src and dest are within valid vertex range
    // (assuming 1-based indexing for vertices, hence <= numVertices)
    if (src <= 0 || src > graph->numVertices || dest <= 0 || dest > graph->numVertices) {
        fprintf(stderr, "Error: Attempted to add invalid edge (%d -> %d). Vertices must be between 1 and %d.\n",
                src, dest, graph->numVertices);
        // Depending on desired behavior, you might exit, return, or log here.
        return;
    }

    // Create a new adjacency list node for the destination vertex
    struct AdjUNode* newNode = createNode(dest); // createNode handles its own malloc/error checking

    // Add the new node to the beginning of the adjacency list for the source vertex
    // This is an efficient way to add to a singly linked list.
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

struct AdjUNode* createNode(int dest) {
    struct AdjUNode* newNode = (struct AdjUNode*)malloc(sizeof(struct AdjUNode));
    if (newNode == NULL) {
        perror("Failed to allocate memory for AdjUNode");
        exit(EXIT_FAILURE); // Critical error, terminate program
    }
    newNode->dest = dest;
    newNode->next = NULL; // Initialize next pointer to NULL
    return newNode;
}

void dfs_first_pass(struct Graph* graph, int v) {
    graph->visited[v] = 1; // Mark the current vertex as visited

    struct AdjUNode* temp = graph->adjLists[v];
    while (temp) {
        if (!graph->visited[temp->dest]) {
            // Recursively call DFS for unvisited neighbors
            dfs_first_pass(graph, temp->dest);
        }
        temp = temp->next;
    }
    // After all descendants of v have been explored and v itself is finished,
    // add v to the dfsOrder array. We fill it from the end to implicitly store
    // nodes in decreasing order of their finishing times.
    graph->dfsOrder[graph->dfsOrderIndex--] = v;
}

int dfs_second_pass(struct Graph* graph, int v) {
    graph->visited[v] = 1; // Mark the current vertex as visited within this SCC
    int componentSize = 1; // Start with the current node itself

    struct AdjUNode* temp = graph->adjLists[v];
    while (temp) {
        if (!graph->visited[temp->dest]) {
            // Recursively explore unvisited neighbors and add their component sizes
            componentSize += dfs_second_pass(graph, temp->dest);
        }
        temp = temp->next;
    }
    return componentSize; // Return the total size of the SCC found
}

int* countStrongConnectedComponentsSize(struct Graph* originalGraph, int* numComponents) {
    if (originalGraph == NULL) {
        *numComponents = 0;
        return NULL;
    }

    // --- Phase 1: First DFS Pass on Original Graph ---
    // Reset the visited array for the first pass.
    // The `+ 1` is important because vertices are 1-indexed.
    memset(originalGraph->visited, 0, (originalGraph->numVertices + 1) * sizeof(int));
    // Reset dfsOrderIndex for filling. We fill from numVertices down to 1.
    originalGraph->dfsOrderIndex = originalGraph->numVertices;

    // Perform DFS on all unvisited nodes to ensure all nodes are covered
    for (int i = 1; i <= originalGraph->numVertices; i++) {
        if (!originalGraph->visited[i]) {
            dfs_first_pass(originalGraph, i);
        }
    }

    // --- Phase 2: Create Transpose Graph ---
    struct Graph* transposedGraph = getTransposeGraph(originalGraph);
    if (transposedGraph == NULL) { // Check for allocation failure
        *numComponents = 0;
        return NULL;
    }

    // --- Phase 3: Second DFS Pass on Transposed Graph ---
    // Reset the visited array for the second pass (operating on the transposed graph).
    memset(transposedGraph->visited, 0, (transposedGraph->numVertices + 1) * sizeof(int));

    // Dynamic array to store SCC sizes. Start with a small capacity and reallocate.
    int* sccSizes = NULL;
    int sccCapacity = 10; // Initial capacity for the collection of SCC sizes
    int sccCount = 0;     // Actual number of SCCs found
    sccSizes = (int*)malloc(sccCapacity * sizeof(int));
    if (sccSizes == NULL) {
        perror("Failed to allocate memory for SCC sizes collection");
        freeGraph(transposedGraph); // Clean up the transposed graph
        *numComponents = 0;
        return NULL;
    }

    // Iterate through nodes in the order determined by the first DFS (decreasing finishing times)
    // The dfsOrder array stores nodes from index 1 to numVertices
    for (int i = 1; i <= originalGraph->numVertices; i++) {
        int v = originalGraph->dfsOrder[i]; // Get the next node from the DFS finishing order

        // If the node hasn't been visited in the transposed graph, it's the root of a new SCC
        if (!transposedGraph->visited[v]) {
            // Perform DFS on the transposed graph to find all nodes in this SCC
            int currentSCCSize = dfs_second_pass(transposedGraph, v);

            // Add the found SCC size to our collection
            if (sccCount >= sccCapacity) {
                // Double the capacity if needed
                sccCapacity *= 2;
                int* temp = (int*)realloc(sccSizes, sccCapacity * sizeof(int));
                if (temp == NULL) {
                    perror("Failed to reallocate memory for SCC sizes");
                    free(sccSizes); // Free the existing part
                    freeGraph(transposedGraph);
                    *numComponents = 0;
                    return NULL;
                }
                sccSizes = temp;
            }
            sccSizes[sccCount++] = currentSCCSize;
        }
    }

    // Sort the collected SCC sizes in ascending order
    qsort(sccSizes, sccCount, sizeof(int), compareInts);

    // Pass the total number of SCCs back to the caller
    *numComponents = sccCount;

    // Free the transposed graph as it is no longer needed
    freeGraph(transposedGraph);

    // Return the dynamically allocated array of SCC sizes.
    // It is the caller's responsibility to free this array.
    return sccSizes;
}

void freeGraph(struct Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i <= graph->numVertices; i++) {
        struct AdjUNode* current = graph->adjLists[i];
        while (current != NULL) {
            struct AdjUNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(graph->adjLists);
    // Free the new arrays added for Kosaraju's algorithm
    free(graph->visited);
    free(graph->dfsOrder);
    free(graph);
}
