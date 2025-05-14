#ifndef SORTING_H
#define SORTING_H
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// Finds the single digit in an integer value by its digit-position
int get_digit_by_position(int source_num, int position);

// Returns a 2D array with old and sorted indexes for the items in intArray
int** counting_sort(int* int_array, int n);
void sort_int(int* sample, int n, int*** order);
char get_max_occurrence_character(char** sample, int n);
char* generate_password(char** sample, int n);
void sort_strings(char** sample, int n);

void merge_sort(int arr[], int temp[], int left, int right, int* inversion_count);
int count_inversions(int arr[], int n, int start_idx);
int sortByLast(int array[], int p, int r, int* lastCounter);
int sortByFirst(int array[], int p, int r, int* firstCounter);
int sortByMedian(int array[], int p, int r, int* medianCounter);

#endif