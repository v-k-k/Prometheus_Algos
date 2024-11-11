#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include "../simple_dynamic_string/sds.h"
#include "../structs.h"

void sort_with_corresponding(int **arrays, int rows, int cols, int N);

int* copy_without_first(int* array, int size);

int** copy_arrays(int *arrays[], int rows, int cols);

#endif