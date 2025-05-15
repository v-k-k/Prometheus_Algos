#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../structs.h"


sds generate_password(sds *sample, int n);

char* fileNameFromUrl(const char* url);

void sort_with_corresponding(int **arrays, int rows, int cols, int N);

int* copy_without_first(int* array, int size);

int** copy_arrays(int *arrays[], int rows, int cols);

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

void convertToSDS(char **baseArray, size_t numElements, sds *sdsArray);

#endif