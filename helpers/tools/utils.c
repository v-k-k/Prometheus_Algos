#include "utils.h"


void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void appendToPositiveIntArray(int* a, int n, int value) {
    for (int i = 0; i < n; i++) {
        if (a[i] == -1) { // Assuming -1 is used as a placeholder for empty slots
            a[i] = value;
            return;
        }
    }
}

char* fileNameFromUrl(const char* url) {
    // Find the last occurrence of '/'
    const char* lastSlash = strrchr(url, '/');
    return strdup(lastSlash + 1);
}

// Comparison function for qsort, which sorts based on values in ascending order
int compare(const void *a, const void *b) {
    Element *elem1 = (Element *)a;
    Element *elem2 = (Element *)b;
    return elem1->value - elem2->value;
}

void sort_with_corresponding(int **arrays, int rows, int cols, int N) {
    // Check if N is a valid row index
    if (N < 0 || N >= rows) {
        printf("Invalid index for sorting array.\n");
        return;
    }

    // Create an array of Element structs to keep track of values and original indices of the N-th array
    Element *elements = (Element *)malloc(cols * sizeof(Element));
    for (int i = 0; i < cols; i++) {
        elements[i].value = arrays[N][i];
        elements[i].index = i;
    }

    // Sort elements based on values in the N-th array
    qsort(elements, cols, sizeof(Element), compare);

    // Temporary 2D array to store sorted values for each row
    int **sorted_arrays = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        sorted_arrays[i] = (int *)malloc(cols * sizeof(int));

    // Rearrange each row in arrays based on the sorted order of the N-th array
    for (int i = 0; i < cols; i++) 
        for (int j = 0; j < rows; j++) 
            sorted_arrays[j][i] = arrays[j][elements[i].index];

    // Copy the sorted values back to the original arrays
    for (int i = 0; i < rows; i++) 
        for (int j = 0; j < cols; j++) 
            arrays[i][j] = sorted_arrays[i][j];

    // Free temporary arrays
    for (int i = 0; i < rows; i++) 
        free(sorted_arrays[i]);

    free(sorted_arrays);
    free(elements);
}

int* copy_without_first(int* array, int size) {
    if (size <= 1) {
        return NULL; // Return NULL if the new array would be empty
    }

    // Allocate memory for the new array (size - 1)
    int* new_array = (int*)malloc((size - 1) * sizeof(int));
    if (new_array == NULL) {
        printf("Memory allocation failed!\n");
        exit(1); // Exit if memory allocation fails
    }

    // Copy elements from the second element onwards
    for (int i = 1; i < size; i++) {
        new_array[i - 1] = array[i];
    }

    return new_array;
}

int** copy_arrays(int *arrays[], int rows, int cols) {
    // Allocate memory for the array of pointers
    int **copy = (int **)malloc(rows * sizeof(int *));
    if (copy == NULL) {
        printf("Memory allocation failed for copy array!\n");
        exit(1);
    }

    // Copy each individual array
    for (int i = 0; i < rows; i++) {
        // Allocate memory for each row in the new array
        copy[i] = (int *)malloc(cols * sizeof(int));
        if (copy[i] == NULL) {
            printf("Memory allocation failed for row %d!\n", i);
            exit(1);
        }

        // Copy the elements from the original array to the new array
        for (int j = 0; j < cols; j++) {
            copy[i][j] = arrays[i][j];
        }
    }

    return copy;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

// Finds the single digit in an integer value by its digit-position
int get_digit_by_position(int source_num, int position) {
    int a = (int)(source_num % (int)pow(10, position));
    int b = (int)(source_num % (int)pow(10, position - 1));
    return (int)((a - b) / pow(10, position - 1));
}

char get_max_occurrence_character(sds *sample, int n) {
    if (n == 0) return '\0';

    int count_array[256] = {0}; // ASCII frequency array
    int max_value = 0;
    char result_char = '\0';

    // Concatenate all SDS strings into one
    sds str = sdsnew("");
    for (int i = 0; i < n; i++) {
        if (sample[i]) {
            str = sdscat(str, sample[i]);
        }
    }

    // Find the most occurring character
    for (size_t i = 0; i < sdslen(str); i++) {
        count_array[(unsigned char)str[i]]++;

        if (count_array[(unsigned char)str[i]] > max_value) {
            max_value = count_array[(unsigned char)str[i]];
            result_char = str[i];
        }
    }

    sdsfree(str);
    return result_char;
}

sds generate_password(sds *sample, int n) {
    if (n == 0) return sdsnew(""); // Return empty SDS string

    char max_char = get_max_occurrence_character(sample, n);

    // Create password using SDS functions
    sds password = sdsnew(sample[0]); // Start with first string
    password = sdscatlen(password, &max_char, 1); // Append most occurring character
    password = sdscat(password, sample[n - 1]); // Append last string

    return password;
}

void convertToSDS(char **baseArray, size_t numElements, sds *sdsArray) {
    for (size_t i = 0; i < numElements; i++) {
        if (baseArray[i]) {
            sdsArray[i] = sdsnew(baseArray[i]); // Convert C string to SDS
        } else {
            sdsArray[i] = NULL;
        }
    }
}

void IntArray_init(IntArray* arr) {
    arr->array = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

int IntArray_append(IntArray* arr, int value) {
    if (arr->size >= arr->capacity) {
        size_t new_capacity = (arr->capacity == 0) ? 4 : arr->capacity * 2;
        int* new_array = (int*)realloc(arr->array, new_capacity * sizeof(int));
        if (new_array == NULL) {
            perror("Failed to reallocate memory for IntArray_append");
            return -1; 
        }
        arr->array = new_array;
        arr->capacity = new_capacity;
    }
    arr->array[arr->size] = value;
    arr->size++;
    return 0;
}

int IntArray_resize(IntArray* arr, size_t new_size) {
    if (new_size > arr->capacity) {
        fprintf(stderr, "Error: IntArray_resize attempting to increase size beyond capacity.\n");
        return -1;
    }
    arr->size = new_size;
    return 0;
}

void IntArray_destroy(IntArray* arr) {
    if (arr->array != NULL) {
        free(arr->array);
        arr->array = NULL;
    }
    arr->size = 0;
    arr->capacity = 0;
}

void printInorder(struct BTreeNode *node) {
    if (node == NULL) { // Explicitly check for NULL, same as printPreorder
        printf("NULL ");
        return;
    }
    // 1. Visit Left Child
    printInorder(node->lchild);
    // 2. Visit Root (Print Data)
    printf("%d ", node->data);
    // 3. Visit Right Child
    printInorder(node->rchild);
}

void printPreorder(struct BTreeNode* node) {
    if (node == NULL) {
        printf("NULL ");
        return;
    }
    printf("%d ", node->data);
    printPreorder(node->lchild);
    printPreorder(node->rchild);
}
