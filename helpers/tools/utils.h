#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include "../structs.h"

void sort_with_corresponding(int **arrays, int rows, int cols, int N);

int* copy_without_first(int* array, int size);

int** copy_arrays(int *arrays[], int rows, int cols);

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif