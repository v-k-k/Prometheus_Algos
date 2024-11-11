#include "sorting.h"

void merge(int arr[], int temp[], int left, int mid, int right, int* inversion_count) {
    int i = left, j = mid + 1, k = left;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            *inversion_count += (mid - i + 1); // All remaining elements in the left half are inversions
        }
    }

    // Copy remaining elements from the left subarray if any
    while (i <= mid) 
        temp[k++] = arr[i++];

    // Copy remaining elements from the right subarray if any
    while (j <= right)
        temp[k++] = arr[j++];

    // Copy the merged subarray into the original array
    for (i = left; i <= right; i++)
        arr[i] = temp[i];
}

void merge_sort(int arr[], int temp[], int left, int right, int* inversion_count) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort(arr, temp, left, mid, inversion_count);         // Left half
        merge_sort(arr, temp, mid + 1, right, inversion_count);    // Right half        
        merge(arr, temp, left, mid, right, inversion_count);       // Merge the two halves
    }
}

int count_inversions(int arr[], int n, int start_idx) {
    int* temp = (int*)malloc(n * sizeof(int)); // Temporary array for merge
    int inversion_count = 0;
    
    merge_sort(arr, temp, start_idx, n - 1, &inversion_count);
    
    free(temp);
    return inversion_count;
}
