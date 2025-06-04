#include "heap.h"

void Heap_init(Heap* heap) {
    IntArray_init(&heap->Hlow);
    IntArray_init(&heap->Hhigh);
}

void Heap_destroy(Heap* heap) {
    IntArray_destroy(&heap->Hlow);
    IntArray_destroy(&heap->Hhigh);
}
 
int Delete(int A[], int n){
    int x = A[0];  // Max element
    A[0] = A[n-1];
 
    int i = 0;
    int j = 2 * i + 1;
 
    while (j < n-1){
        // Compare left and right children
        if (A[j] < A[j+1]){
            j = j+1;
        }
 
        // Compare parent and largest child
        if (A[i] < A[j]){
            swap(&A[i], &A[j]);
            i = j;
            j = 2 * i + 1;
        } else {
            break;
        }
    }
    return x;
}

void BuildMaxHeap(int* array, size_t array_len) {
    if (array == NULL || array_len == 0) return;
    for (int i = (int)array_len / 2 - 1; i >= 0; i--)
        MaxHeapify(array, array_len, i);
}

void BuildMinHeap(int* array, size_t array_len) {
    if (array == NULL || array_len == 0) return;
    for (int i = (int)array_len / 2 - 1; i >= 0; i--)
        MinHeapify(array, array_len, i);
}

int ExtractMax(IntArray* heap_array) {
    if (heap_array->size == 0) {
        fprintf(stderr, "Error: ExtractMax called on empty heap.\n");
        return -1; 
    }

    int max_val = heap_array->array[0];
    swap(&heap_array->array[0], &heap_array->array[heap_array->size-1]);
    
    IntArray_resize(heap_array, heap_array->size - 1); 
    if (heap_array->size > 0) 
        MaxHeapify(heap_array->array, heap_array->size, 0);
 
    return max_val;
/*(int *maxA, int *n_ptr){
    if (*n_ptr == 0) {
        fprintf(stderr, "Error: ExtractMax called on empty heap.\n");
        return -1; // Or some other error indicator, like INT_MIN from limits.h
    }
    int max = maxA[0];
    swap(&maxA[0], &maxA[*n_ptr-1]);
    (*n_ptr)--; // Decrement the size in the caller's scope

    // Only heapify if there are elements left after extraction
    if (*n_ptr > 0) {
        MaxHeapify(maxA, *n_ptr, 0);
    }
    return max;*/
}

int ExtractMin(IntArray* heap_array) {
    if (heap_array->size == 0) {
        fprintf(stderr, "Error: ExtractMin called on empty heap.\n");
        return -1; 
    }

    int min_val = heap_array->array[0];
    swap(&heap_array->array[0], &heap_array->array[heap_array->size-1]);

    IntArray_resize(heap_array, heap_array->size - 1); 

    if (heap_array->size > 0)
        MinHeapify(heap_array->array, heap_array->size, 0);

    return min_val;
/*(int *minA, int *n_ptr){
    if (*n_ptr == 0) {
        fprintf(stderr, "Error: ExtractMax called on empty heap.\n");
        return -1; // Or some other error indicator, like INT_MIN from limits.h
    }
    int min = minA[0];
    swap(&minA[0], &minA[*n_ptr-1]);
    (*n_ptr)--; // Decrement the size in the caller's scope

    // Only heapify if there are elements left after extraction
    if (*n_ptr > 0) {
        MinHeapify(minA, *n_ptr, 0);
    }
    return min;*/
}

void MaxHeapify(int* array, size_t array_len, int start) {
    int left = 2 * start + 1;
    int right = 2 * start + 2;
    int largest = start;

    if (left < array_len && array[left] > array[largest]) 
        largest = left;
    if (right < array_len && array[right] > array[largest])
        largest = right;

    if (largest != start) {
        swap(&array[start], &array[largest]);
        MaxHeapify(array, array_len, largest);
    }
}

void MinHeapify(int* array, size_t array_len, int start) {
    int left = 2 * start + 1;
    int right = 2 * start + 2;
    int lowest = start;

    if (left < array_len && array[left] < array[lowest]) 
        lowest = left;
    if (right < array_len && array[right] < array[lowest]) 
        lowest = right;

    if (lowest != start) {
        swap(&array[start], &array[lowest]);
        MinHeapify(array, array_len, lowest);
    }
}
 
void Heapify(int A[], int n){
    // # of leaf elements: (n+1)/2, index of last leaf element's parent = (n/2)-1
    for (int i=(n/2)-1; i>=0; i--){
 
        int j = 2 * i + 1;  // Left child for current i
 
        while(j < n-1){
            // Compare left and right children of current i
            if (A[j] < A[j+1]) j = j+1;
 
            // Compare parent and largest child
            if (A[i] < A[j]){
                swap(&A[i], &A[j]);
                i = j;
                j = 2 * i + 1;
            } 
            else break;
        }
    }
}

int AddElementAndFindMedian(Heap* heap_instance, int new_element, IntArray* medians_out) {
    // Clear medians_out before filling with new median(s)
    IntArray_destroy(medians_out);
    IntArray_init(medians_out);

    // Step 1: Add the new element.
    // A common and robust strategy: always add to Hhigh (min-heap) first.
    // This simplifies the initial element logic.
    if (IntArray_append(&heap_instance->Hhigh, new_element) != 0) {
        return -1; // Memory allocation error
    }
    // After appending, ensure the Hhigh remains a min-heap
    // BuildMinHeap is heavy, typically you'd bubble up, but for consistency we'll call BuildMinHeap after append.
    if (heap_instance->Hhigh.size > 0) {
        BuildMinHeap(heap_instance->Hhigh.array, heap_instance->Hhigh.size);
    }

    // Step 2: Balance the heaps (ensure Hlow has smaller elements).
    // Move the smallest from Hhigh to Hlow (max-heap)
    // This ensures Hlow elements are <= Hhigh elements
    if (heap_instance->Hhigh.size > 1) {
        int val_from_hhigh = ExtractMin(&heap_instance->Hhigh);
        if (val_from_hhigh == -1 && heap_instance->Hhigh.size == 0 && heap_instance->Hlow.size + heap_instance->Hhigh.size > 0) {
             // Handle error if ExtractMin failed but heap was not empty
             return -1;
        }
        if (IntArray_append(&heap_instance->Hlow, val_from_hhigh) != 0) {
            return -1; // Memory error
        }
        // After appending, ensure Hlow remains a max-heap
        if (heap_instance->Hlow.size > 0) {
            BuildMaxHeap(heap_instance->Hlow.array, heap_instance->Hlow.size);
        }
    }


    // Step 3: Restore the size invariant: |Hlow.size - Hhigh.size| <= 1
    // If Hlow has too many elements (more than Hhigh)
    if (heap_instance->Hlow.size > heap_instance->Hhigh.size + 1) {
        int val = ExtractMax(&heap_instance->Hlow);
        if (val == -1 && heap_instance->Hlow.size == 0) return -1; // Error check
        if (IntArray_append(&heap_instance->Hhigh, val) != 0) return -1; // Memory error
        // After move, ensure Hhigh remains a min-heap
        if (heap_instance->Hhigh.size > 0) {
            BuildMinHeap(heap_instance->Hhigh.array, heap_instance->Hhigh.size);
        }
    }

    // Re-evaluating the balancing logic after the 'always add to Hhigh, then move to Hlow'
    // Now, Hlow contains all elements that are <= Hhigh.array[0].
    // We just need to ensure their sizes are balanced.
    // If Hlow ends up with more elements than Hhigh by more than 1
    if (heap_instance->Hlow.size > heap_instance->Hhigh.size + 1) {
        int val = ExtractMax(&heap_instance->Hlow);
        if (val == -1 && heap_instance->Hlow.size == 0) return -1; // Error check
        if (IntArray_append(&heap_instance->Hhigh, val) != 0) return -1; // Memory error
        if (heap_instance->Hhigh.size > 0) {
            BuildMinHeap(heap_instance->Hhigh.array, heap_instance->Hhigh.size);
        }
    }
    // If Hhigh ends up with more elements than Hlow
    // This case only happens if the total number of elements is odd, and the median should be in Hhigh.
    // Or if Hhigh is indeed too large.
    else if (heap_instance->Hhigh.size > heap_instance->Hlow.size) {
        // If total_elements is odd, and Hhigh has the extra element, it's balanced.
        // Only move from Hhigh to Hlow if Hhigh has _too many_ elements.
        // After the initial add-to-Hhigh and move-to-Hlow, the total count in Hlow is (N+1)/2, Hhigh is (N-1)/2.
        // The condition should be more specific for Hhigh being too large.
        // Original check: Hhigh.size > Hlow.size + 1 (i.e., difference is 2 or more)
        if (heap_instance->Hhigh.size > heap_instance->Hlow.size + 1) {
            int val = ExtractMin(&heap_instance->Hhigh);
            if (val == -1 && heap_instance->Hhigh.size == 0) return -1; // Error check
            if (IntArray_append(&heap_instance->Hlow, val) != 0) return -1; // Memory error
            if (heap_instance->Hlow.size > 0) {
                BuildMaxHeap(heap_instance->Hlow.array, heap_instance->Hlow.size);
            }
        }
    }

    // Reconfirming the Build*Heap calls.
    // After each append and extract, the heap property might be broken.
    // A single BuildHeap is a bit overkill, but it's simpler than
    // implementing an 'insert' (bubble up) or 'heapify down from root' after each append.
    // If your ExtractMax/Min already call MaxHeapify/MinHeapify on the root, that's fine.
    // The main place heap property is broken is when *appending* to the array.
    if (heap_instance->Hlow.size > 0) {
        BuildMaxHeap(heap_instance->Hlow.array, heap_instance->Hlow.size);
    }
    if (heap_instance->Hhigh.size > 0) {
        BuildMinHeap(heap_instance->Hhigh.array, heap_instance->Hhigh.size);
    }

    // Determine the median(s)
    size_t total_elements = heap_instance->Hlow.size + heap_instance->Hhigh.size;

    if (total_elements == 0) {
        // If no elements, median is undefined or -1 as an indicator
        if (IntArray_append(medians_out, -1) != 0) return -1;
    } else if (total_elements % 2 == 0) { // Even number of elements
        // Medians are the largest from Hlow and the smallest from Hhigh
        if (heap_instance->Hlow.size > 0) {
            if (IntArray_append(medians_out, heap_instance->Hlow.array[0]) != 0) return -1;
        }
        if (heap_instance->Hhigh.size > 0) {
            if (IntArray_append(medians_out, heap_instance->Hhigh.array[0]) != 0) return -1;
        }
    } else { // Odd number of elements
        // Median is the root of the larger heap
        if (heap_instance->Hlow.size > heap_instance->Hhigh.size) {
            if (IntArray_append(medians_out, heap_instance->Hlow.array[0]) != 0) return -1;
        } else { // Hhigh.size > Hlow.size (or equal if total is 1, but then Hlow would be 0, Hhigh 1)
            if (IntArray_append(medians_out, heap_instance->Hhigh.array[0]) != 0) return -1;
        }
    }

    return 0;
}

void FindMedians(int *A, int n, int start, int medians[], int arrayLow[], int arrayHigh[]){
    /*appendToPositiveIntArray(A, n, start);
    if (n == 1)
    {
        medians[0] = A[0];
        arrayHigh[0] = A[0];
        //__high = __high.Append(array[0]);
    }
    else
    {
        if (n == 2)
        {
            if (start > arrayHigh[0])
            {
                arrayLow[0] = start;
                swap(&arrayHigh[0], &arrayLow[0]);
            }
            else
                arrayLow[0] = start;
        }
        else if (n > 0 && start < arrayLow[0])
            appendToPositiveIntArray(arrayLow, n, start);
        else
            appendToPositiveIntArray(arrayHigh, n, start);
        for (int i = 0; i < n; i++) {
            if (arrayHigh[i] == -1 && arrayLow[i] == -1) break;

            if ((arrayHigh[i] > 0 && arrayLow[i] == -1) && (arrayHigh[i+1] > 0 && arrayLow[i+1] == -1)){
                appendToPositiveIntArray(arrayLow, n, ExtractMin(arrayHigh, &n));
                break;
            }
            else if ((arrayLow[i] > 0 && arrayHigh[i] == -1) && (arrayLow[i+1] > 0 && arrayHigh[i+1] == -1)){
                appendToPositiveIntArray(arrayHigh, n, ExtractMax(arrayLow, &n));
                break;
            }
        }
        BuildMinHeap(arrayHigh, n);
        BuildMaxHeap(arrayLow, n);
        if (n > 0)
        {
            if (n % 2 == 0)
            {
                appendToPositiveIntArray(medians, n, arrayLow[0]);                
                appendToPositiveIntArray(medians, n, arrayHigh[0]);
            }
            else{
                for (int i = 0; i < n; i++) {
                    if (arrayHigh[i] == -1 && arrayLow[i] == -1) break;
                    if (arrayHigh[i] > 0 && arrayLow[i] == -1)
                        appendToPositiveIntArray(medians, n, arrayHigh[0]);
                    else 
                        appendToPositiveIntArray(medians, n, arrayLow[0]);                        
                }
            }
        }
    }*/
}
