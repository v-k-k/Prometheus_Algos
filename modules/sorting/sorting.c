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

// Returns a 2D array with old and sorted indexes for the items in intArray
int* counting_sort(int* int_array, int n, int max) {
    int* sorted_indices = (int*)malloc(n * sizeof(int));
    if (!sorted_indices) { perror("Memory allocation failed"); exit(EXIT_FAILURE); }
    int* counters_array = (int*)malloc((max + 1) * sizeof(int));
    if (!counters_array) { perror("Memory allocation failed"); free(sorted_indices); exit(EXIT_FAILURE); }
    memset(counters_array, 0, (max + 1) * sizeof(int));

    for (int i = 0; i < n; i++) {
        counters_array[int_array[i]]++;
    }
    for (int i = 1; i <= max; i++) {
        counters_array[i] += counters_array[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int value = int_array[i];
        sorted_indices[counters_array[value] - 1] = i; // Store original index
        counters_array[value]--;
    }

    free(counters_array);
    return sorted_indices;
}

void sort_int(int* sample, int n, int*** order) {
    if (n == 0) return;
    int max_val = sample[0];
    for (int i = 1; i < n; i++) {
        if (sample[i] > max_val) {
            max_val = sample[i];
        }
    }
    int digits = (max_val == 0) ? 1 : (int)floor(log10(max_val)) + 1;

    for (int i = 1; i <= digits; i++) {
        int* prepared_array = (int*)malloc(n * sizeof(int));
        if (!prepared_array) { perror("Memory allocation failed"); exit(EXIT_FAILURE); }
        int* cloned_sample = (int*)malloc(n * sizeof(int));
        if (!cloned_sample) { perror("Memory allocation failed"); free(prepared_array); exit(EXIT_FAILURE); }
        memcpy(cloned_sample, sample, n * sizeof(int));

        int current_max = -1;
        for(int j = 0; j < n; j++) {
            int digit = get_digit_by_position(sample[j], i);
            prepared_array[j] = digit;
            if (digit > current_max) {
                current_max = digit;
            }
        }
        if (current_max == -1) current_max = 0;

        int* sorted_indices = counting_sort(prepared_array, n, current_max);
        int* temp_sample = (int*)malloc(n * sizeof(int));
        if (!temp_sample) { perror("Memory allocation failed"); free(prepared_array); free(cloned_sample); free(sorted_indices); exit(EXIT_FAILURE); }
        for (int j = 0; j < n; j++) {
            temp_sample[sorted_indices[j]] = cloned_sample[j];
        }
        memcpy(sample, temp_sample, n * sizeof(int));
        free(temp_sample);

        for (int j = 0; j < n; j++) {
            int idx = 0;
            int order_len = 0;
            while ((*order)[j][order_len] != INT_MIN) {
                order_len++;
            }
            for (int k = 0; k < order_len; k++) {
                if ((*order)[j][k + 1] == INT_MIN) {
                    idx = k;
                    break;
                }
            }
            // This part needs to be re-evaluated based on the new counting_sort output
            for (int k = 0; k < n; k++) {
                if ((*order)[k][0] == j) { // Find the original index j
                    int new_order_len = 0;
                    while ((*order)[k][new_order_len] != INT_MIN) {
                        new_order_len++;
                    }
                    (*order)[k] = (int*)realloc((*order)[k], (new_order_len + 2) * sizeof(int));
                    if (!(*order)[k]) { perror("Memory reallocation failed"); exit(EXIT_FAILURE); }
                    // The sorted_indices array gives the new position of the element at index k
                    (*order)[k][new_order_len] = sorted_indices[k];
                    (*order)[k][new_order_len + 1] = INT_MIN;
                    break;
                }
            }
        }

        free(prepared_array);
        free(cloned_sample);
        free(sorted_indices);
    }
}

void sort_strings(sds* sample, int n) {
    if (n == 0) return;
    size_t max_len = 0;
    for (int i = 0; i < n; i++) {
        if (sample[i] != NULL) {
            size_t len = sdslen(sample[i]);
            if (len > max_len) max_len = len;
        }
    }

    for (long long i = (long long)max_len - 1; i >= 0; i--) {
        sds* cloned_sample[n];
        for (int j = 0; j < n; j++) 
            cloned_sample[j] = sdsdup(sample[j]);

        int ascii_sample[n];
        int current_max = -1;
        for (int j = 0; j < n; j++) {
            if (sample[j] != NULL && i < sdslen(sample[j])) {
                ascii_sample[j] = (unsigned char)sample[j][i];
                if (ascii_sample[j] > current_max) current_max = ascii_sample[j];
            } else {
                ascii_sample[j] = -1; // Treat shorter strings as having a smaller character
                if (-1 > current_max) current_max = -1;
            }
        }
        if (current_max == -1) current_max = 255; // Adjust max for -1 case

        int* sorted_indices = counting_sort(ascii_sample, n, current_max > 0 ? current_max : 255);

        sds* temp_sample[n];
        for (int j = 0; j < n; j++) 
            temp_sample[j] = sdsdup(cloned_sample[j]);

        for (int j = 0; j < n; j++) {
            sdsfree(sample[j]);
            sample[j] = sdsdup(temp_sample[sorted_indices[j]]);
        }

        for (int j = 0; j < n; j++) {
            sdsfree(cloned_sample[j]);
            sdsfree(temp_sample[j]);
        }
        free(sorted_indices);
    }
}

