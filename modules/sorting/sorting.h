#ifndef SORTING_H
#define SORTING_H
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../helpers/simple_dynamic_string/sds.h"
#include "../../helpers/tools/utils.h"

int* counting_sort(int* int_array, int n, int max);
void sort_int(int* sample, int n, int*** order);
void sort_strings(sds* sample, int n);

void merge_sort(int arr[], int temp[], int left, int right, int* inversion_count);
int count_inversions(int arr[], int n, int start_idx);
int sortByLast(int array[], int p, int r, int* lastCounter);
int sortByFirst(int array[], int p, int r, int* firstCounter);
int sortByMedian(int array[], int p, int r, int* medianCounter);

#endif