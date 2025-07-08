#ifndef STRUCTS_H
#define STRUCTS_H

#include "simple_dynamic_string/sds.h"

// Helper structure to represent a dynamic set of paths per vertex
typedef struct PathSet {
    char** entries;
    int size;
    int capacity;
} PathSet;

// Structure to return results of optimized Dijkstra's algorithm
// Contains distances from the source to each vertex and the number of paths to each vertex.
typedef struct {
    int* distances;
    int* path_counts;
    // int* predecessors; // If you need to reconstruct *a* path, include prev here
} DijkstraResult;

// Node to store in the priority queue (vertex and its distance)
typedef struct {
    int vertex;
    int distance;
} PQNode;

// Min-Priority Queue structure (using a binary heap)
typedef struct {
    PQNode* heap;      // Array to store heap elements
    int* pos;          // Position of vertex in heap (for decrease_key)
    int capacity;      // Maximum capacity of the heap
    int size;          // Current number of elements in the heap
} MinPriorityQueue;

// Structure to represent an edge in the adjacency list
typedef struct WeightedAdjUNode {
    int dest;          // Destination vertex
    int weight;        // Weight of the edge
    struct WeightedAdjUNode* next; // Pointer to the next edge in the list
} WeightedAdjUNode;

// Structure to represent the graph
typedef struct WeightedGraph {
    int num_vertices;      // Number of vertices in the graph
    WeightedAdjUNode** adj_lists;  // Array of pointers to EdgeNodes (adjacency lists)
} WeightedGraph;

// Structure for a node in the adjacency list (unchanged from prior, but included for context)
struct AdjUNode {
    int dest;
    struct AdjUNode* next;
};

// Structure for the adjacency list (MODIFIED to include Kosaraju-specific fields)
struct Graph {
    int numVertices;
    struct AdjUNode** adjLists;
    // For Kosaraju's algorithm:
    int* visited;          // Array to track visited status for DFS
    int* dfsOrder;         // Array to store DFS finishing order (nodes by finishing time)
    int dfsOrderIndex;     // Current index for dfsOrder array (used as a stack pointer)
};

 // Definition of your BTreeNode struct
struct BTreeNode {
    struct BTreeNode *lchild;
    int data;
    struct BTreeNode *rchild;
};

// DECLARATION of the global root pointer
// 'extern' tells the compiler that 'root' exists somewhere else,
// and its definition will be provided by another compilation unit (bst.c file).
extern struct BTreeNode *root;

struct f_list {
    sds name;
    sds content;
    struct f_list* next;

    void (*show_content)(struct f_list* head);
};
 
struct MemoryStruct {
  char *memory;
  size_t size;
};

// Structure to store elements of the first array along with their original indices
typedef struct {
    int value;
    int index;
} Element;

typedef struct {
    int user;
    int *inversions;
} ExpextedUserInversions;

// Parameter structure for `TestFiveFilmsSequences`.
struct TestParams {
    sds testKey;
    int userIndex;
    int comparUserIndex;
    int expectedUserInversions;
};

// Structure to represent a key-value pair (mimicking Dictionary<int, List<string>>)
typedef struct {
    int key;
    char **values;
    int value_count;
} IntStringListPair;

// Structure to represent a dictionary (mimicking Dictionary<int, List<string>>)
typedef struct {
    IntStringListPair *pairs;
    int capacity;
    int count;
} IntStringListDictionary;

// Structure to represent a string-integer pair (mimicking Dictionary<string, int>)
typedef struct {
    char *key;
    int value;
} StringIntPair;

// Structure to represent a dictionary (mimicking Dictionary<string, int>)
typedef struct {
    StringIntPair *pairs;
    int capacity;
    int count;
} StringIntDictionary;

// Structure to represent a tuple of two string arrays (mimicking Tuple<string[], string[]>)
typedef struct {
    char **array1;
    int array1_count;
    char **array2;
    int array2_count;
} StringArrayTuple;

typedef struct {
    int* array;
    size_t size;
    size_t capacity;
} IntArray;

typedef struct {
    IntArray Hlow; 
    IntArray Hhigh; 
} Heap;

#endif