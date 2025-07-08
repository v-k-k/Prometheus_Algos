#ifndef MINPQ_H
#define MINPQ_H

#include "../../helpers/tools/utils.h"

// Min-heapify function (to maintain heap property after insertion/deletion)
void min_heapify(MinPriorityQueue* pq, int idx);

// Function to initialize the priority queue
MinPriorityQueue* init_pq(int capacity);

// Function to check if priority queue is empty
bool is_empty_pq(MinPriorityQueue* pq);

// Function to insert a new element into the priority queue (or update if exists)
// This simplified insert always adds, then heapifies up.
// For Dijkstra, `decrease_key` is more common if element might already be there.
void insert_pq(MinPriorityQueue* pq, int vertex, int distance);

// Function to extract the minimum element (root) from the priority queue
PQNode extract_min_pq(MinPriorityQueue* pq);

// Function to decrease the distance value of a vertex in the priority queue
// This is crucial for Dijkstra's efficiency
void decrease_key_pq(MinPriorityQueue* pq, int vertex, int new_distance);

// Function to free the priority queue memory
void free_pq(MinPriorityQueue* pq);

#endif