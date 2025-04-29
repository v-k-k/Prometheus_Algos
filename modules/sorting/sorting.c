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

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int getMedianElement(int array[], int start, int end) {
    int swapValueIndex;
    if (end - start + 1 == 2) {
        swapValueIndex = start;
    } else {
        int middle = (start + end) / 2;
        int helperArray[] = { array[start], array[middle], array[end] };
        
        int maxHelper = helperArray[0], minHelper = helperArray[0];
        for (int i = 1; i < 3; i++) {
            if (helperArray[i] > maxHelper) maxHelper = helperArray[i];
            if (helperArray[i] < minHelper) minHelper = helperArray[i];
        }

        if (array[start] != maxHelper && array[start] != minHelper)
            swapValueIndex = start;
        else if (array[middle] != maxHelper && array[middle] != minHelper)
            swapValueIndex = middle;
        else
            swapValueIndex = end;
    }
    swap(&array[swapValueIndex], &array[end]);
    return array[end];
}

int partition(int array[], int start, int end, int* counter, int median) {
    int pivot = (median == INT_MIN) ? array[end] : median;
    int position = start - 1;

    for (int i = start; i < end; i++) {
        if (array[i] <= pivot) {
            position++;
            swap(&array[position], &array[i]);
        }
    }
    swap(&array[++position], &array[end]);
    *counter += end - start;
    return position;
}

int sortByLast(int array[], int p, int r, int* lastCounter) {
    if (p < r) {
        int q = partition(array, p, r, lastCounter, INT_MIN);
        sortByLast(array, p, q - 1, lastCounter);
        sortByLast(array, q + 1, r, lastCounter);
    }
    return *lastCounter;
}

int sortByFirst(int array[], int p, int r, int* firstCounter) {
    if (p < r) {
        swap(&array[p], &array[r]);
        int q = partition(array, p, r, firstCounter, INT_MIN);
        sortByFirst(array, p, q - 1, firstCounter);
        sortByFirst(array, q + 1, r, firstCounter);
    }
    return *firstCounter;
}

int sortByMedian(int array[], int p, int r, int* medianCounter) {
    if (p < r) {
        int medianValue = getMedianElement(array, p, r);
        int q = partition(array, p, r, medianCounter, medianValue);
        sortByMedian(array, p, q - 1, medianCounter);
        sortByMedian(array, q + 1, r, medianCounter);
    }
    return *medianCounter;
}
