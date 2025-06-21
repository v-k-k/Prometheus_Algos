#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

#define BST_BASIC_SIZE 21

extern "C"
{
#include "../helpers/zip_helper/zip_helper.h"
#include "../modules/data_structures/heap.h"
#include "../modules/data_structures/hash_table.h"
#include "../modules/data_structures/bst.h"
#include "../modules/sorting/sorting.h"
}

class TrainingHeapSortTest : public ::testing::TestWithParam<std::tuple<const char*, const char*, size_t>> {};

TEST_P(TrainingHeapSortTest, Training)
{
    // Retrieve test parameters
    const char* ZIPPED_FOLDER = std::get<0>(GetParam());
    const char* F_SUFFIX = std::get<1>(GetParam());
    size_t ARRAY_SIZE = std::get<2>(GetParam());

    int input_array[ARRAY_SIZE];
    
    struct f_list* file_list = NULL;
    int input_size = 0;
    sds *input_tokens = NULL;
    int output_size = 0;
    sds *output_tokens = NULL;
    
    int count = 0;
    
    get_zipped_files(&file_list, ZIPPED_FOLDER);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;

    sds input_file = sdscatfmt(sdsempty(), "input%s", F_SUFFIX);
    sds output_file = sdscatfmt(sdsempty(), "output%s", F_SUFFIX);

    for(int64_t idx = 0; idx < entries_count; ++idx){    
        if (sdscmp(input_file, targetX->name) == 0) {
            input_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &count);            
            if (sdsToInt(input_tokens[0], &input_size) != 0) return;

            EXPECT_EQ(ARRAY_SIZE, static_cast<size_t>(input_size))
                << "ARRAY_SIZE = " << ARRAY_SIZE << "\n"
                << "input_size = " << input_size;

            for (int j = 0; j < input_size; j++) 
                if (sdsToInt(input_tokens[j + 1], &input_array[j]) != 0) return; 

            sdsfreesplitres(input_tokens, count);           
        }
        else if (sdscmp(output_file, targetX->name) == 0) {
            output_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &output_size);
            EXPECT_EQ(ARRAY_SIZE, static_cast<size_t>(output_size))
                << "ARRAY_SIZE = " << ARRAY_SIZE << "\n"
                << "output_size = " << output_size;            
        }
        targetX = targetX->next;
    }
    Heap my_median_finder_heap;
    Heap_init(&my_median_finder_heap);
    
    IntArray current_medians; 
    IntArray_init(&current_medians);

    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        sds *expected_values = sdssplitlen(output_tokens[i], sdslen(output_tokens[i]), " ", 1, &count);
        int expected_median = 0;

        AddElementAndFindMedian(&my_median_finder_heap, input_array[i], &current_medians);        

        for (size_t j = 0; j < current_medians.size; j++) {
            sdsToInt(expected_values[j], &expected_median); 
            EXPECT_EQ(current_medians.array[j], expected_median)
                << targetX->name << "Expected median: " << expected_median 
                << ", but got: " << current_medians.array[j];
        }
        sdsfreesplitres(expected_values, count);
    }
    sdsfreesplitres(output_tokens, output_size);
    IntArray_destroy(&current_medians);
    Heap_destroy(&my_median_finder_heap);
    list_destroy(file_list);
    sdsfree(input_file);
    sdsfree(output_file);
}

INSTANTIATE_TEST_SUITE_P(
    HeapSortParamTests, 
    TrainingHeapSortTest,
    ::testing::Values(
        std::make_tuple("../data/data_examples_05.zip", "_01_10.txt", 10),
        std::make_tuple("../data/data_examples_05.zip", "_02_10.txt", 10),
        std::make_tuple("../data/data_examples_05.zip", "_03_10.txt", 10),
        std::make_tuple("../data/data_examples_05.zip", "_04_10.txt", 10),
        std::make_tuple("../data/data_examples_05.zip", "_05_100.txt", 100),
        std::make_tuple("../data/data_examples_05.zip", "_06_100.txt", 100),
        std::make_tuple("../data/data_examples_05.zip", "_07_100.txt", 100),
        std::make_tuple("../data/data_examples_05.zip", "_08_100.txt", 100),
        std::make_tuple("../data/data_examples_05.zip", "_09_1000.txt", 1000),
        std::make_tuple("../data/data_examples_05.zip", "_10_1000.txt", 1000),
        std::make_tuple("../data/data_examples_05.zip", "_11_1000.txt", 1000),
        std::make_tuple("../data/data_examples_05.zip", "_12_1000.txt", 1000)
    )
);

TEST(test_main_heap_sort, main)
{
    std::string DATA_SOURCE = "https://courses.prometheus.org.ua/assets/courseware/v1/7bfd7538f12aba38e8570c5f51b78648/c4x/KPI/Algorithms101/asset/input_16_10000.txt";
    size_t EXP_ARRAY_SIZE = 10000;
    size_t CH_1 = 2015;
    int EM_1 = 4905;
    size_t CH_2 = 9876;
    int EM_2[] = {4994, 4995};
    size_t HEAP_VALS_TO_CHECK = 5;

    // Doublecheck the `AddElementAndFindMedian` because `EXP_HIGH` values are { 4905, 4959, 4918, 4978, 4969 }
    int EXP_HIGH[] = {4905, 4959, 4918, 4989, 4969};
    int EXP_LOW[] = {4900, 4833, 4893, 4817, 4824};

    int input_array[EXP_ARRAY_SIZE];
    int count = 0;
    int input_size = 0;
    sds content = retrievePlainText(DATA_SOURCE.c_str());
    sds *input_tokens = sdssplitlen(content, sdslen(content), "\n", 1, &count);

    if (sdsToInt(input_tokens[0], &input_size) != 0) return;
    EXPECT_EQ(EXP_ARRAY_SIZE, static_cast<size_t>(input_size))
        << "ARRAY_SIZE = " << EXP_ARRAY_SIZE << "\n"
        << "input_size = " << input_size;

    Heap my_median_finder_heap;
    Heap_init(&my_median_finder_heap);
    
    IntArray current_medians; 
    IntArray_init(&current_medians);

    for (size_t i = 0; i < EXP_ARRAY_SIZE; i++) {
        if (i == CH_1 - 1) {
            for (size_t j = 0; j < HEAP_VALS_TO_CHECK; j++) {
                EXPECT_EQ(my_median_finder_heap.Hhigh.array[j], EXP_HIGH[j])
                    << "Checkpoint: " << CH_1 
                    << "\nExpected EXP_HIGH[" << j << "]: " << EXP_HIGH[j] <<", but got: " << my_median_finder_heap.Hhigh.array[j];
                EXPECT_EQ(my_median_finder_heap.Hlow.array[j], EXP_LOW[j])
                    << "Checkpoint: " << CH_1   
                    << "\nExpected EXP_LOW[" << j << "]: " << EXP_LOW[j] <<", but got: " << my_median_finder_heap.Hlow.array[j];
            }
        }

        if (sdsToInt(input_tokens[i + 1], &input_array[i]) != 0) return;

        AddElementAndFindMedian(&my_median_finder_heap, input_array[i], &current_medians);      

        if (i == CH_1 - 1) {
            EXPECT_EQ(current_medians.size, 1)
                << "Checkpoint: " << CH_1 
                << "\nExpected size: 1, but got: " << current_medians.size;
            EXPECT_EQ(current_medians.array[0], EM_1)
                << "Checkpoint: " << CH_1 
                << "\nExpected median: " << EM_1 
                << ", but got: " << current_medians.array[0];
        }
        else if (i == CH_2 - 1) {
            EXPECT_EQ(current_medians.size, 2)
                << "Checkpoint: " << CH_2
                << "\nExpected size: 2, but got: " << current_medians.size;
            EXPECT_EQ(current_medians.array[0], EM_2[0])
                << "Checkpoint: " << CH_1 
                << "\nExpected median: " << EM_2[0] 
                << ", but got: " << current_medians.array[0];
            EXPECT_EQ(current_medians.array[1], EM_2[1])
                << "Checkpoint: " << CH_1 
                << "\nExpected median: " << EM_2[1] 
                << ", but got: " << current_medians.array[1];
        }        
    }
    sdsfree(content);
    sdsfreesplitres(input_tokens, count);
    IntArray_destroy(&current_medians);
    Heap_destroy(&my_median_finder_heap);
}


class HashTableTest : public ::testing::TestWithParam<std::tuple<const char*, int, int>> {};

TEST_P(HashTableTest, Full)
{
    // Retrieve test parameters
    const char* path = std::get<0>(GetParam());
    int EXP_ARRAY_SIZE = std::get<1>(GetParam());
    int EXPECTED_COUNT = std::get<2>(GetParam());
    
    long long int input_array[EXP_ARRAY_SIZE];
    int MIN_SUM = -1000;
    int MAX_SUM = 1000;
    int RANGE_SIZE = (MAX_SUM - MIN_SUM + 1);
    bool found_sums[RANGE_SIZE];

    long long int *HT;
    struct f_list* file_list = NULL;
    sds *tokens, *real_tokens;
    int count, j, real_t_count = 0, COUNTER = 0;
    
    get_zipped_files(&file_list, path);
    
    size_t entries_count = list_length(file_list);
    EXPECT_EQ(entries_count, 1)
        << "entries_count = " << entries_count << "\n"
        << "expected = " << 1;

    struct f_list* targetX = file_list;

    tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &count);
    real_tokens = (sds *)malloc(count * sizeof(sds));
    if (real_tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sdsfreesplitres(tokens, count);  // Free tokens array before exiting
        list_destroy(file_list);
        return;
    }

    for (j = 0; j < count; j++) {
        if (sdslen(tokens[j]) > 0) 
            real_tokens[real_t_count++] = tokens[j]; // Assign, don't copy!
    }
    
    //Reallocate real_tokens to the exact size
    real_tokens = (sds*)realloc(real_tokens, real_t_count * sizeof(sds));
    if (real_tokens == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        sdsfreesplitres(tokens, count);
        list_destroy(file_list);
        return;
    }

    EXPECT_EQ(EXP_ARRAY_SIZE, real_t_count)
        << "EXP_ARRAY_SIZE = " << EXP_ARRAY_SIZE << "\n"
        << "count = " << real_t_count;

    HT = (long long int *)calloc(TABLE_SIZE, sizeof(long long int)); // calloc initializes all elements to 0
    if (!HT) {
        std::cerr << "Memory allocation for hash table failed!" << std::endl;
        return;
    }

    for (int i = 0; i < EXP_ARRAY_SIZE; i++) {
        if (sdsToLongLong(real_tokens[i], &input_array[i]) != 0) {
            std::cerr << "sdsToInt conversion failed at index: " << i << " = " << real_tokens[i] << std::endl;
            return;
        }
        Insert(HT, input_array[i]);
    }
    
    for (int i = 0; i < RANGE_SIZE; i++) found_sums[i] = false; 

    for (int i = 0; i < EXP_ARRAY_SIZE; i++) {
        long long x = input_array[i];
        
        // Check all S = x + y ∈ [-1000, 1000]
        for (int S = MIN_SUM; S <= MAX_SUM; S++) {
            long long y = S - x;

            // Check: y ≠ x and y є in hash table
            if (y != x && HashSearch(HT, y) != -1) {
                found_sums[S - MIN_SUM] = true;
            }
        }
    }
    for (int i = 0; i < RANGE_SIZE; i++) {
        if (found_sums[i]) COUNTER++;
    }
    
    EXPECT_EQ(EXPECTED_COUNT, COUNTER)
        << "EXPECTED_COUNT = " << EXPECTED_COUNT << "\n"
        << "COUNTER = " << COUNTER;

    sdsfreesplitres(tokens, count);
    free(HT);
    free(real_tokens);
    list_destroy(file_list);
}

INSTANTIATE_TEST_SUITE_P(
    HashTableParamTests, 
    HashTableTest,
    ::testing::Values(
        std::make_tuple("../data/test_06.txt.zip", 100000, 22),
        std::make_tuple("../data/input_06.txt.zip", 1000000, 41)
    )
);/*
void printPreorder(struct BTreeNode* node) {
    if (node == NULL) {
        printf("NULL ");
        return;
    }
    printf("%d ", node->data);
    printPreorder(node->lchild);
    printPreorder(node->rchild);
}
void printInorder(struct BTreeNode *p) {
    if (p == NULL) { // Explicitly check for NULL, same as printPreorder
        printf("NULL ");
        return;
    }
    // 1. Visit Left Child
    printInorder(p->lchild);
    // 2. Visit Root (Print Data)
    printf("%d ", p->data);
    // 3. Visit Right Child
    printInorder(p->rchild);
}*/

class TrainingBstTest : public ::testing::TestWithParam<std::tuple<std::array<int, BST_BASIC_SIZE>, int, int>> {};

TEST_P(TrainingBstTest, Training)
{
    // Access the parameters
    std::array<int, BST_BASIC_SIZE> TREE_DATA = std::get<0>(GetParam());
    int EXP_SUM = std::get<1>(GetParam());
    int EXP_AMOUNT = std::get<2>(GetParam());

    const int* c_tree_array_ptr = TREE_DATA.data();

    // Initialize the index for the build process
    int current_build_idx = 0;

    // Build the tree, passing the source array, its size, and the index pointer
    root = BuildTreePreorder(c_tree_array_ptr, BST_BASIC_SIZE, &current_build_idx);

    int inorder_values[BST_BASIC_SIZE];
    int current_inorder_index = 0; // Initialize index to 0

    // Call the function to save inorder values
    BTreeInorderToArray(root, inorder_values, &current_inorder_index, BST_BASIC_SIZE);

    int* temp = (int*)malloc(current_inorder_index * sizeof(int));
    int inversion_count = 0;
    
    merge_sort(inorder_values, temp, 0, current_inorder_index - 1, &inversion_count);
    free(temp);
    
    current_inorder_index = 0; // Reset index for the next operation
    BTreeInorderFromArray(root, inorder_values, &current_inorder_index, BST_BASIC_SIZE);

    int monotonicPathsCount = findMonotonicPathsCount(root, EXP_SUM);

    EXPECT_EQ(monotonicPathsCount, EXP_AMOUNT)
        << "Calculated = " << monotonicPathsCount << "\n"
        << "expected paths count = " << EXP_AMOUNT;     
        
    freeTree(root);
}

INSTANTIATE_TEST_SUITE_P(
    BstParamTests, 
    TrainingBstTest,
    ::testing::Values(
        std::make_tuple(std::array<int, BST_BASIC_SIZE>{ 1, 4, 6, 10, 0, 0, 0, 7, 0, 8, 0, 0, 2, 5, 0, 0, 3, 9, 0, 0, 0 }, 9, 3),
        std::make_tuple(std::array<int, BST_BASIC_SIZE>{ 1, 2, 7, 8, 0, 0, 10, 0, 0, 0, 3, 4, 6, 0, 0, 0, 5, 9, 0, 0, 0 }, 9, 3)
    )
);

class ZippedBstTest : public ::testing::TestWithParam<std::tuple<const char*, int>> {};

TEST_P(ZippedBstTest, Zipped)
{
    const char* path = "../data/data_examples_07.zip";
    
    // Access the parameters
    const char* F_SUFF = std::get<0>(GetParam());
    int DESIRED_SUM = std::get<1>(GetParam());

    struct f_list* file_list = NULL;
    sds *tokens, *real_tokens;
    int count, j, real_t_count = 0, EXP_COUNT = 0;
    int *input_array;

    sds inPref = sdscatfmt(sdsempty(), "%s.txt", F_SUFF);
    sds outPref = sdscatfmt(sdsempty(), "%s_%i.txt", F_SUFF, DESIRED_SUM);
    
    get_zipped_files(&file_list, path);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;
    for(int64_t idx = 0; idx < entries_count; ++idx){
        sds *input_tokens, *real_input_tokens, *output_tokens, *real_output_tokens;   
        if (sdsendswith(targetX->name, inPref)) {         
            input_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), " ", 1, &count);
            real_input_tokens = (sds *)malloc(count * sizeof(sds));
            if (real_input_tokens == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                sdsfreesplitres(input_tokens, count);  // Free tokens array before exiting
                list_destroy(file_list);
                return;
            }

            for (j = 0; j < count; j++) {
                if (sdslen(input_tokens[j]) > 0) 
                    real_input_tokens[real_t_count++] = input_tokens[j]; // Assign, don't copy!
            }
        
            //Reallocate real_tokens to the exact size
            real_input_tokens = (sds*)realloc(real_input_tokens, real_t_count * sizeof(sds));
            if (real_input_tokens == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                sdsfreesplitres(input_tokens, count);
                list_destroy(file_list);
                return;
            }

            // Allocate memory for input_array
            input_array = (int *)malloc(real_t_count * sizeof(int));
            if (input_array == NULL) {
                fprintf(stderr, "Error: Memory allocation failed for input_array.\n");
                // Handle allocation failure: maybe return an error code or exit
                return; // Or exit(EXIT_FAILURE);
            }

            for (j = 0; j < real_t_count; j++) 
                if (sdsToInt(real_input_tokens[j], &input_array[j]) != 0) return; 
            
            sdsfreesplitres(input_tokens, count);
            free(real_input_tokens);
        }
        if (sdsendswith(targetX->name, outPref)) {
            output_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &EXP_COUNT);
            sdsfreesplitres(output_tokens, EXP_COUNT);
        }        
        targetX = targetX->next;
    }

    // Initialize the index for the build process
    int current_build_idx = 0;

    // Build the tree, passing the source array, its size, and the index pointer
    root = BuildTreePreorder(input_array, real_t_count, &current_build_idx);
    
    int inorder_values[real_t_count];
    int current_inorder_index = 0; // Initialize index to 0

    // Call the function to save inorder values
    BTreeInorderToArray(root, inorder_values, &current_inorder_index, real_t_count);

    int* temp = (int*)malloc(current_inorder_index * sizeof(int));
    int inversion_count = 0;
    
    merge_sort(inorder_values, temp, 0, current_inorder_index - 1, &inversion_count);
    free(temp);
    
    current_inorder_index = 0; // Reset index for the next operation
    BTreeInorderFromArray(root, inorder_values, &current_inorder_index, real_t_count);

    int monotonicPathsCount = findMonotonicPathsCount(root, DESIRED_SUM);

    EXPECT_EQ(monotonicPathsCount, EXP_COUNT)
        << "Calculated = " << monotonicPathsCount << "\n"
        << "expected paths count = " << EXP_COUNT;       
        
    sdsfree(inPref);
    sdsfree(outPref);
    free(input_array);
    list_destroy(file_list);
    freeTree(root);
}

INSTANTIATE_TEST_SUITE_P(
    ZippedBstParamTests, 
    ZippedBstTest,
    ::testing::Values(
        std::make_tuple("10a", 9),
        std::make_tuple("10b", 7),
        std::make_tuple("10c", 9),
        std::make_tuple("10d", 5),
        std::make_tuple("10e", 5),
        std::make_tuple("100a", 51),
        std::make_tuple("100b", 78),
        std::make_tuple("100c", 103),
        std::make_tuple("100d", 50),
        std::make_tuple("100e", 50)
    )
);
