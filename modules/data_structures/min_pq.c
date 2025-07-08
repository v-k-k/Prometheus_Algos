#include "min_pq.h"

void min_heapify(MinPriorityQueue* pq, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->heap[left].distance < pq->heap[smallest].distance) {
        smallest = left;
    }

    if (right < pq->size && pq->heap[right].distance < pq->heap[smallest].distance) {
        smallest = right;
    }

    if (smallest != idx) {
        // Update positions in the pos array
        pq->pos[pq->heap[smallest].vertex] = idx;
        pq->pos[pq->heap[idx].vertex] = smallest;

        swapPQNodes(&pq->heap[smallest], &pq->heap[idx]);
        min_heapify(pq, smallest);
    }
}

MinPriorityQueue* init_pq(int capacity) {
    MinPriorityQueue* pq = (MinPriorityQueue*)malloc(sizeof(MinPriorityQueue));
    if (!pq) {
        perror("Failed to allocate MinPriorityQueue");
        exit(EXIT_FAILURE);
    }
    pq->capacity = capacity;
    pq->size = 0;
    pq->heap = (PQNode*)malloc(capacity * sizeof(PQNode));
    pq->pos = (int*)malloc(capacity * sizeof(int)); // Initialize positions
    if (!pq->heap || !pq->pos) {
        perror("Failed to allocate PQ heap or pos array");
        free(pq->heap);
        free(pq->pos);
        free(pq);
        exit(EXIT_FAILURE);
    }
    // Initialize all positions to -1 or some invalid index
    for(int i = 0; i < capacity; ++i) {
        pq->pos[i] = -1;
    }
    return pq;
}

bool is_empty_pq(MinPriorityQueue* pq) {
    return pq->size == 0;
}

void insert_pq(MinPriorityQueue* pq, int vertex, int distance) {
    if (pq->size == pq->capacity) {
        fprintf(stderr, "Priority Queue is full!\n");
        exit(EXIT_FAILURE);
    }

    int i = pq->size;
    pq->heap[i].vertex = vertex;
    pq->heap[i].distance = distance;
    pq->pos[vertex] = i; // Store its position

    // Heapify up (bubble up)
    while (i != 0 && pq->heap[(i - 1) / 2].distance > pq->heap[i].distance) {
        pq->pos[pq->heap[i].vertex] = (i - 1) / 2;
        pq->pos[pq->heap[(i - 1) / 2].vertex] = i;
        swapPQNodes(&pq->heap[i], &pq->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    pq->size++;
}

PQNode extract_min_pq(MinPriorityQueue* pq) {
    if (is_empty_pq(pq)) {
        fprintf(stderr, "Priority Queue is empty, cannot extract min!\n");
        exit(EXIT_FAILURE);
    }

    PQNode root = pq->heap[0];

    // Move the last element to the root
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->pos[pq->heap[0].vertex] = 0;
    pq->pos[root.vertex] = -1; // Mark extracted vertex as not in PQ

    pq->size--;
    min_heapify(pq, 0); // Heapify down from the root

    return root;
}

void decrease_key_pq(MinPriorityQueue* pq, int vertex, int new_distance) {
    // Get the current index of the vertex in the heap
    int i = pq->pos[vertex];

    // Update the distance
    pq->heap[i].distance = new_distance;

    // Heapify up (bubble up) if needed
    while (i != 0 && pq->heap[(i - 1) / 2].distance > pq->heap[i].distance) {
        pq->pos[pq->heap[i].vertex] = (i - 1) / 2;
        pq->pos[pq->heap[(i - 1) / 2].vertex] = i;
        swapPQNodes(&pq->heap[i], &pq->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void free_pq(MinPriorityQueue* pq) {
    free(pq->heap);
    free(pq->pos);
    free(pq);
}
