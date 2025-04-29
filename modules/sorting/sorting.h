#ifndef SORTING_H
#define SORTING_H
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

void merge_sort(int arr[], int temp[], int left, int right, int* inversion_count);
int count_inversions(int arr[], int n, int start_idx);
int sortByLast(int array[], int p, int r, int* lastCounter);
int sortByFirst(int array[], int p, int r, int* firstCounter);
int sortByMedian(int array[], int p, int r, int* medianCounter);

#endif