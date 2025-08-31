#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include "../structs.h"

static int* global_distances = NULL;
static int global_distances_size = 0;

int compare_by_distance(const void* a, const void* b);

void swap(int* a, int* b);

void appendToPositiveIntArray(int* a, int n, int value);

sds generate_password(sds *sample, int n);

char* fileNameFromUrl(const char* url);

void sort_with_corresponding(int **arrays, int rows, int cols, int N);

int* copy_without_first(int* array, int size);

int** copy_arrays(int *arrays[], int rows, int cols);

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

void convertToSDS(char **baseArray, size_t numElements, sds *sdsArray);

void IntArray_init(IntArray* arr);

int IntArray_append(IntArray* arr, int value);

int IntArray_resize(IntArray* arr, size_t new_size);

void IntArray_destroy(IntArray* arr);

void printPreorder(struct BTreeNode* node);

void printInorder(struct BTreeNode *node);

// Function to print the graph
void printGraph(struct Graph* graph);

// Comparison function for qsort
int compareInts(const void *a, const void *b);

// Helper function to read the text file
char* read_text_file(const char *filename);

// Function to find the vertex with the minimum distance value,
// from the set of vertices not yet included in the shortest path tree.
int findMinDistanceVertex(int* dist, bool* visited, int num_vertices);

// Function to print the shortest path from source to a target vertex
void printPath(int* prev, int current_vertex);

// Helper function to swap two PQNodes
void swapPQNodes(PQNode* a, PQNode* b);

// Helper function to track the unique path`s in optimized Dijkstra's algorithm
void add_encoded_path(PathSet* set, const char* path_str);

// Function to check if a path already exists in the PathSet
bool path_exists(PathSet* path_set, const char* path);

#endif