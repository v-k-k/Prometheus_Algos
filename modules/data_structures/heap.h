#ifndef HEAP_H
#define HEAP_H

#include "../../helpers/tools/utils.h"

void Heap_init(Heap* heap);
void Heap_destroy(Heap* heap);

// Functions for basic heap operations
int Delete(int A[], int n); 
void Heapify(int A[], int n);

// Functions for more complicated heap operations
void MaxHeapify(int* array, size_t array_len, int start);

void MinHeapify(int* array, size_t array_len, int start);

void BuildMaxHeap(int* array, size_t array_len);

void BuildMinHeap(int* array, size_t array_len);

int ExtractMax(IntArray* heap_array);

int ExtractMin(IntArray* heap_array);

int AddElementAndFindMedian(Heap* heap_instance, int new_element, IntArray* medians_out);

#endif