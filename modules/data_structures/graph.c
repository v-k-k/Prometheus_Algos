#include "graph.h"

// Checks whether the adjacency list graph->adj_lists[src] already contains an edge src→dst with weight weight
bool edge_exists(WeightedGraph* graph,
                 int src,    // 0-index
                 int dst,    // 0-index
                 int weight)
{
    for (WeightedAdjUNode* e = graph->adj_lists[src]; e; e = e->next) {
        if (e->dest == dst && e->weight == weight)
            return 1;
    }
    return 0;
}

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

WeightedGraph* createWeightedGraph(int num_vertices) {
    WeightedGraph* graph = (WeightedGraph*)malloc(sizeof(WeightedGraph));
    if (!graph) {
        perror("Failed to allocate graph");
        exit(EXIT_FAILURE);
    }
    graph->num_vertices = num_vertices;

    // Allocate memory for the array of adjacency lists
    graph->adj_lists = (WeightedAdjUNode**)malloc(num_vertices * sizeof(WeightedAdjUNode*));
    if (!graph->adj_lists) {
        perror("Failed to allocate adjacency lists");
        free(graph);
        exit(EXIT_FAILURE);
    }

    // Initialize each adjacency list as empty
    for (int i = 0; i < num_vertices; i++) {
        graph->adj_lists[i] = NULL;
    }
    return graph;
}

void addWeightedEdge(WeightedGraph* graph, int src, int dest, int weight) {
    // Adjust src and dest to be 0-indexed for array access
    int adjusted_src = src - 1;
    int adjusted_dest = dest - 1;

    // Basic validation: ensure adjusted indices are within bounds
    if (adjusted_src < 0 || adjusted_src >= graph->num_vertices ||
        adjusted_dest < 0 || adjusted_dest >= graph->num_vertices) {
        fprintf(stderr, "Error: Vertex index out of bounds. num_vertices: %d, src: %d, dest: %d\n",
                graph->num_vertices, src, dest);
        exit(EXIT_FAILURE);
    }

    WeightedAdjUNode* new_node = (WeightedAdjUNode*)malloc(sizeof(WeightedAdjUNode));
    if (!new_node) {
        perror("Failed to allocate new edge node");
        exit(EXIT_FAILURE);
    }

    new_node->dest = adjusted_dest; // Store the 0-indexed destination
    new_node->weight = weight;
    new_node->next = graph->adj_lists[adjusted_src]; // Access adj_lists with 0-indexed source
    graph->adj_lists[adjusted_src] = new_node;

    // IMPORTANT CONSIDERATION for UNDIRECTED GRAPHS:
    // If this is an UNDIRECTED graph, you must add an edge in the reverse direction as well.
    // If it's a DIRECTED graph, this single addition is sufficient.
    // Assuming 'WeightedGraph' implies it might be undirected, add the reverse edge.
    // If it's strictly directed, remove the following block.
    /*WeightedAdjUNode* reverse_node = (WeightedAdjUNode*)malloc(sizeof(WeightedAdjUNode));
    if (!reverse_node) {
        perror("Failed to allocate new reverse edge node");
        exit(EXIT_FAILURE);
    }
    reverse_node->dest = adjusted_src; // Destination is the original source (0-indexed)
    reverse_node->weight = weight;
    reverse_node->next = graph->adj_lists[adjusted_dest];
    graph->adj_lists[adjusted_dest] = reverse_node;*/
}

void addWeightedEdgeUnique(WeightedGraph* graph,
                     int src,
                     int dest,
                     int weight)
{
    // Switch to 0-index
    int u = src  - 1;
    int v = dest - 1;

    // Index validity check
    if (u < 0 || u >= graph->num_vertices ||
        v < 0 || v >= graph->num_vertices) {
        fprintf(stderr,
                "Error: Vertex index out of bounds. "
                "num_vertices=%d, src=%d, dest=%d\n",
                graph->num_vertices, src, dest);
        exit(EXIT_FAILURE);
    }

    // If the edge already exists — do nothing
    if (!edge_exists(graph, u, v, weight)) {
        // insert edge u → v
        WeightedAdjUNode* node = malloc(sizeof(WeightedAdjUNode));
        if (!node) {
            perror("Failed to allocate new edge node");
            exit(EXIT_FAILURE);
        }
        node->dest   = v;
        node->weight = weight;
        node->next   = graph->adj_lists[u];
        graph->adj_lists[u] = node;
    }

    // For an undirected graph, also add v → u if it doesn't already exist
    if (!edge_exists(graph, v, u, weight)) {
        WeightedAdjUNode* rev = malloc(sizeof(WeightedAdjUNode));
        if (!rev) {
            perror("Failed to allocate reverse edge node");
            exit(EXIT_FAILURE);
        }
        rev->dest   = u;
        rev->weight = weight;
        rev->next   = graph->adj_lists[v];
        graph->adj_lists[v] = rev;
    }
}

void freeWeightedGraph(WeightedGraph* graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        WeightedAdjUNode* current = graph->adj_lists[i];
        while (current != NULL) {
            WeightedAdjUNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adj_lists);
    free(graph);
}

int* dijkstraSlow(WeightedGraph* graph, int source) {
    int num_vertices = graph->num_vertices;

    // Adjust the source to be 0-indexed for internal array access
    int adjusted_source = source - 1;
    if (adjusted_source < 0 || adjusted_source >= num_vertices) {
        fprintf(stderr, "Error: Source vertex %d is out of bounds (1 to %d).\n", source, num_vertices);
        exit(EXIT_FAILURE);
    }

    int* dist = (int*)malloc(num_vertices * sizeof(int));      // Stores shortest distance from source
    int* prev = (int*)malloc(num_vertices * sizeof(int));      // Stores predecessor in shortest path
    bool* visited = (bool*)malloc(num_vertices * sizeof(bool)); // True if vertex is finalized

    if (!dist || !prev || !visited) {
        perror("Failed to allocate memory for Dijkstra arrays");
        // Clean up any successfully allocated memory before exiting
        free(dist);
        free(prev);
        free(visited);
        exit(EXIT_FAILURE);
    }

    // Initialize dist, prev, and visited arrays
    for (int i = 0; i < num_vertices; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1; // -1 indicates no predecessor (or is the source)
        visited[i] = false;
    }

    // Distance from adjusted_source to itself is 0
    dist[adjusted_source] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < num_vertices; count++) { // Loop for num_vertices times to ensure all reachable nodes are processed
        // Pick the unvisited vertex with minimum distance
        int u = findMinDistanceVertex(dist, visited, num_vertices);

        // If no unvisited vertex can be reached (all remaining are INF or all visited), break
        if (u == -1) {
            break;
        }

        // Mark the picked vertex as visited
        visited[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex
        WeightedAdjUNode* current_edge = graph->adj_lists[u]; // u is already 0-indexed
        while (current_edge != NULL) {
            int v = current_edge->dest; // v is already 0-indexed from addWeightedEdge
            int weight = current_edge->weight;

            // Relaxation step: If 'v' is not visited and new path through 'u' is shorter
            // Use long long for the sum to prevent potential overflow before comparison
            if (!visited[v] && dist[u] != INT_MAX && (long long)dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u; // Set 'u' as predecessor of 'v' (both are 0-indexed)
            }
            current_edge = current_edge->next;
        }
    }

    // Clean up temporary allocated memory (prev and visited)
    free(prev);
    free(visited);

    // Return the distances array. Caller is responsible for freeing it.
    return dist;
}

DijkstraResult* dijkstraOptimized(WeightedGraph* graph, int source) {
    int num_vertices      = graph->num_vertices;
    int adjusted_source   = source - 1;

    // Allocate result structure
    DijkstraResult* result = (DijkstraResult*)malloc(sizeof(DijkstraResult));
    if (!result) {
        perror("Failed to allocate DijkstraResult");
        exit(EXIT_FAILURE);
    }

    // Allocate arrays for distances and path counts
    result->distances       = malloc(num_vertices * sizeof(int));
    result->path_counts     = malloc(num_vertices * sizeof(int));
    bool* finalized         = calloc(num_vertices, sizeof(bool));

    for (int i = 0; i < num_vertices; i++) {
        result->distances[i]   = INT_MAX;
        result->path_counts[i] = 0;
    }

    // Distance from source to itself is 0, and there's 1 way to reach it
    result->distances[adjusted_source]   = 0;
    result->path_counts[adjusted_source] = 1;

    // Initialize the priority queue
    MinPriorityQueue* pq = init_pq(num_vertices);
    insert_pq(pq, adjusted_source, 0);

    while (!is_empty_pq(pq)) {
        PQNode current = extract_min_pq(pq);
        int u          = current.vertex;
        int d          = current.distance;

        if (finalized[u] || d != result->distances[u])
            continue;

        finalized[u] = true;

        for (WeightedAdjUNode* edge = graph->adj_lists[u]; edge; edge = edge->next) {
            int v  = edge->dest;    // siblingVertex
            int w  = edge->weight;  // weight
            int nd = d + w;         // new_dist

            // Relax the edge (u, v)
            if (nd < result->distances[v]) {
                // Found a shorter path to `v`
                result->distances[v]   = nd;
                result->path_counts[v] = result->path_counts[u];

                if (!finalized[v]) {
                    // Insert if `v` (siblingVertex) not already in the queue; otherwise, decrease its key
                    if (pq->pos[v] == -1)
                        insert_pq(pq, v, nd);
                    else
                        decrease_key_pq(pq, v, nd);
                }
            }
            else if (nd == result->distances[v] && !finalized[v]) {
                // Found another shortest path of the same length to `v`
                result->path_counts[v] += result->path_counts[u];
            }
        }
    }
    // Clean up the priority queue memory
    free_pq(pq);
    free(finalized);

    // Return the DijkstraResult structure. Caller is responsible for freeing its contents.
    return result;
}
