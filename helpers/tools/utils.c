#include "utils.h"


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
