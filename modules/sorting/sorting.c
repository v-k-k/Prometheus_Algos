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

// Finds the single digit in an integer value by its digit-position
int get_digit_by_position(int source_num, int position) {
    int a = (int)(source_num % (int)pow(10, position));
    int b = (int)(source_num % (int)pow(10, position - 1));
    return (int)((a - b) / pow(10, position - 1));
}

// Returns a 2D array with old and sorted indexes for the items in intArray
int** counting_sort(int* int_array, int n) {
    int* debug = (int*)malloc(n * sizeof(int));
    if (!debug) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int max = INT_MIN;
    int** result = (int**)malloc(n * sizeof(int*));
    if (!result) {
        perror("Memory allocation failed");
        free(debug);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        result[i] = (int*)malloc(3 * sizeof(int));
        if (!result[i]) {
            perror("Memory allocation failed");
            for (int j = 0; j < i; j++) free(result[j]);
            free(result);
            free(debug);
            exit(EXIT_FAILURE);
        }
        result[i][0] = int_array[i];
        result[i][1] = i;
        result[i][2] = INT_MIN;
        if (int_array[i] > max) {
            max = int_array[i];
        }
    }

    int* counters_array = (int*)malloc((max + 1) * sizeof(int));
    if (!counters_array) {
        perror("Memory allocation failed");
        for (int i = 0; i < n; i++) free(result[i]);
        free(result);
        free(debug);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        counters_array[int_array[i]]++;
    }
    for (int j = 1; j <= max; j++) {
        counters_array[j] += counters_array[j - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int current_value = int_array[i];
        for (int j = n - 1; j >= 0; j--) {
            if (result[j][0] == current_value && result[j][2] == INT_MIN) {
                result[j][2] = --counters_array[int_array[i]];
                break;
            }
        }
    }

    free(counters_array);
    free(debug);
    return result;
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
        if (!prepared_array) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        int* cloned_sample = (int*)malloc(n * sizeof(int));
        if (!cloned_sample) {
            perror("Memory allocation failed");
            free(prepared_array);
            exit(EXIT_FAILURE);
        }
        memcpy(cloned_sample, sample, n * sizeof(int));

        for (int j = 0; j < n; j++) {
            prepared_array[j] = get_digit_by_position(sample[j], i);
        }

        int** sortin_result = counting_sort(prepared_array, n);

        for (int j = 0; j < n; j++) {
            printf("sortin_result[%d][2] = %d, sortin_result[%d][1] = %d\n", j, sortin_result[j][2], j, sortin_result[j][1]);
            sample[sortin_result[j][2]] = cloned_sample[sortin_result[j][1]];
        }

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

            for (int k = 0; k < n; k++) {
                int current_order_val = (*order)[j][idx];
                int sort_original_index = sortin_result[k][1];
                if (sort_original_index == current_order_val) {
                    int new_order_len = 0;
                    while ((*order)[j][new_order_len] != INT_MIN) {
                        new_order_len++;
                    }
                    (*order)[j] = (int*)realloc((*order)[j], (new_order_len + 2) * sizeof(int));
                    if (!(*order)[j]) {
                        perror("Memory reallocation failed");
                        exit(EXIT_FAILURE);
                    }
                    (*order)[j][idx + 1] = sortin_result[k][2];
                    (*order)[j][new_order_len + 1] = INT_MIN; // Keep the terminator
                    break;
                }
            }
        }

        for (int j = 0; j < n; j++) {
            for (int k = 0; k < 3; k++) {
                free(sortin_result[j]);
            }
        }
        free(sortin_result);
        free(prepared_array);
        free(cloned_sample);
    }
}

char get_max_occurrence_character(char** sample, int n) {
    if (n == 0) return '\0';
    int count_array[256] = {0};
    int max_value = 0;
    char result_char = '\0';
    char* str = (char*)malloc(1);
    if (!str) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    str[0] = '\0';
    size_t total_len = 0;
    for (int i = 0; i < n; i++) {
        total_len += strlen(sample[i]);
    }
    str = (char*)realloc(str, (total_len + 1) * sizeof(char));
    if (!str) {
        perror("Memory reallocation failed");
        exit(EXIT_FAILURE);
    }
    str[0] = '\0';
    for (int i = 0; i < n; i++) {
        strcat(str, sample[i]);
    }

    for (size_t i = 0; i < strlen(str); i++) {
        count_array[(unsigned char)str[i]]++;
        if (count_array[(unsigned char)str[i]] >= max_value) {
            max_value = count_array[(unsigned char)str[i]];
            result_char = str[i];
        }
    }
    free(str);
    return result_char;
}

char* generate_password(char** sample, int n) {
    if (n == 0) {
        char* empty = (char*)malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    char max_occurrence_character = get_max_occurrence_character(sample, n);
    size_t len1 = strlen(sample[0]);
    size_t len_last = strlen(sample[n - 1]);
    size_t password_len = len1 + 1 + len_last + 1;
    char* password = (char*)malloc(password_len * sizeof(char));
    if (!password) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    sprintf(password, "%s%c%s", sample[0], max_occurrence_character, sample[n - 1]);
    return password;
}

void sort_strings(char** sample, int n) {
    if (n == 0) return;
    if (sample[0] == NULL) return;
    size_t max_len = 0;
    for (int i = 0; i < n; i++) {
        if (sample[i] && strlen(sample[i]) > max_len) {
            max_len = strlen(sample[i]);
        }
    }

    for (int i = max_len - 1; i >= 0; i--) {
        char** cloned_sample = (char**)malloc(n * sizeof(char*));
        if (!cloned_sample) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < n; j++) {
            cloned_sample[j] = strdup(sample[j]);
            if (!cloned_sample[j]) {
                perror("Memory allocation failed");
                for (int k = 0; k < j; k++) free(cloned_sample[k]);
                free(cloned_sample);
                exit(EXIT_FAILURE);
            }
        }

        int* ascii_sample = (int*)malloc(n * sizeof(int));
        if (!ascii_sample) {
            perror("Memory allocation failed");
            for (int j = 0; j < n; j++) free(cloned_sample[j]);
            free(cloned_sample);
            exit(EXIT_FAILURE);
        }
        int*** order = (int***)malloc(sizeof(int**));
        if (!order) {
            perror("Memory allocation failed");
            free(ascii_sample);
            for (int j = 0; j < n; j++) free(cloned_sample[j]);
            free(cloned_sample);
            exit(EXIT_FAILURE);
        }
        *order = (int**)malloc(n * sizeof(int*));
        if (!(*order)) {
            perror("Memory allocation failed");
            free(ascii_sample);
            for (int j = 0; j < n; j++) free(cloned_sample[j]);
            free(cloned_sample);
            free(order);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < n; j++) {
            (*order)[j] = (int*)malloc((max_len + 1) * sizeof(int));
            if (!(*order)[j]) {
                perror("Memory allocation failed");
                free(ascii_sample);
                for (int k = 0; k < j; k++) free((*order)[k]);
                free(*order);
                free(order);
                for (int k = 0; k < n; k++) free(cloned_sample[k]);
                free(cloned_sample);
                exit(EXIT_FAILURE);
            }
            for (size_t k = 0; k <= max_len; k++) {
                (*order)[j][k] = INT_MIN;
            }
            (*order)[j][0] = j;
            ascii_sample[j] = (j < strlen(sample[j])) ? (int)sample[j][i] : -1; // Use -1 for shorter strings
        }

        sort_int(ascii_sample, n, order);

        char** temp_sample = (char**)malloc(n * sizeof(char*));
        if (!temp_sample) {
            perror("Memory allocation failed");
            free(ascii_sample);
            for (int j = 0; j < n; j++) free((*order)[j]);
            free(*order);
            free(order);
            for (int j = 0; j < n; j++) free(cloned_sample[j]);
            free(cloned_sample);
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < n; j++) {
            int original_index = (*order)[j][0];
            int sorted_index_order = (*order)[j][max_len]; // Last valid index should hold the final sorted position
            if (sorted_index_order >= 0 && sorted_index_order < n) {
                temp_sample[sorted_index_order] = strdup(cloned_sample[original_index]);
                if (!temp_sample[sorted_index_order]) {
                    perror("Memory allocation failed");
                    free(ascii_sample);
                    for (int k = 0; k < n; k++) free((*order)[k]);
                    free(*order);
                    free(order);
                    for (int k = 0; k < n; k++) free(cloned_sample[k]);
                    free(cloned_sample);
                    for (int k = 0; k < j; k++) free(temp_sample[k]);
                    free(temp_sample);
                    exit(EXIT_FAILURE);
                }
            }
        }

        for (int j = 0; j < n; j++) {
            free(sample[j]);
            sample[j] = temp_sample[j];
            free(cloned_sample[j]);
            free((*order)[j]);
        }
        free(temp_sample);
        free(cloned_sample);
        free(*order);
        free(order);
        free(ascii_sample);
    }
}

