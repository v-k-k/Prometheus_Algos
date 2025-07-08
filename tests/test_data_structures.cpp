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
#include "../modules/data_structures/graph.h"
#include "../modules/sorting/sorting.h"
}
/*
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
);

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

TEST(test_bst_main, main)
{
    std::string DATA_SOURCE = "https://courses.prometheus.org.ua/assets/courseware/v1/2b7ac6054236d173fc556de9f817c494/c4x/KPI/Algorithms101/asset/input_1000a.txt";
    int EXP_ROOT = 490;
    int EXP_FIRST_LEAFS[] = {2, 4, 7};
    int EXP_LAST_LEAFS[] = {992, 996, 999};
    int count = 0, real_t_count = 0, input_size = 0;
    int SUMS[] = {1059, 1546, 1940}; // Expected sums for the unique paths
    int PATH1[] = {354, 353, 352}; // Expected path for sum 1059
    int PATH2[] = {388, 385, 387, 386}; // Expected path for sum 1546
    int PATH3[] = {490, 258, 366, 424, 402}; // Expected path for sum 1940

    int *input_array;
    sds *real_input_tokens;

    sds content = retrievePlainText(DATA_SOURCE.c_str());
    sds *input_tokens = sdssplitlen(content, sdslen(content), " ", 1, &count);
    real_input_tokens = (sds *)malloc(count * sizeof(sds));
    if (real_input_tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sdsfreesplitres(input_tokens, count);  // Free tokens array before exiting
        return;
    }

    for (int j = 0; j < count; j++) {
        if (sdslen(input_tokens[j]) > 0) 
            real_input_tokens[real_t_count++] = input_tokens[j]; // Assign, don't copy!
    }

    //Reallocate real_tokens to the exact size
    real_input_tokens = (sds*)realloc(real_input_tokens, real_t_count * sizeof(sds));
    if (real_input_tokens == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        sdsfreesplitres(input_tokens, count);
        return;
    }

    // Allocate memory for input_array
    input_array = (int *)malloc(real_t_count * sizeof(int));
    if (input_array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for input_array.\n");
        // Handle allocation failure: maybe return an error code or exit
        return; // Or exit(EXIT_FAILURE);
    }

    for (int j = 0; j < real_t_count; j++) 
        if (sdsToInt(real_input_tokens[j], &input_array[j]) != 0) return;    
    
    sdsfreesplitres(input_tokens, count);
    free(real_input_tokens);
    sdsfree(content);
    
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

    EXPECT_EQ(root->data, EXP_ROOT)
        << "ROOT = " << root->data << "\n"
        << "EXP_ROOT = " << EXP_ROOT; 

    // Pass 1: Count qualifying leaf nodes
    int leaf_count = 0;
    countQualifyingLeafNodes(root, &leaf_count);

    // Pass 2: Allocate array and collect values
    int *leafs_array = (int*)malloc(leaf_count * sizeof(int));
    if (leafs_array == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for leafs_array.\n");
        return;
    }

    int current_idx = 0;
    collectQualifyingLeafValues(root, leafs_array, &current_idx);

    for (int i = 0; i < leaf_count; i++) {
        if (i < 3) {
            EXPECT_EQ(leafs_array[i], EXP_FIRST_LEAFS[i])
                << "First leafs[" << i << "] = " << leafs_array[i] 
                << ", expected: " << EXP_FIRST_LEAFS[i];
        }
        else if (i >= leaf_count - 3) {
            EXPECT_EQ(leafs_array[i], EXP_LAST_LEAFS[-(leaf_count - i - 3)])
                << "Last leafs[" << i << "] = " << leafs_array[i] 
                << ", expected: " << EXP_LAST_LEAFS[-(leaf_count - i - 3)];
        }
    }   
    
    for (int i = 0; i < 3; i++) {
        int path_len = 0;
        int *path_to_sum = findUniqueMonotonicPath(root, SUMS[i], &path_len);

        switch (i)
        {
        case 0:
            for (int j = 0; j < path_len; j++) {
                EXPECT_EQ(path_to_sum[j], PATH1[j])
                    << "Path for sum " << SUMS[i] << " does not match expected value at index " << j;
            }
            break;

        case 1:
            for (int j = 0; j < path_len; j++) {
                EXPECT_EQ(path_to_sum[j], PATH2[j])
                    << "Path for sum " << SUMS[i] << " does not match expected value at index " << j;
            }
            break;
        
        default:
            for (int j = 0; j < path_len; j++) {
                EXPECT_EQ(path_to_sum[j], PATH3[j])
                    << "Path for sum " << SUMS[i] << " does not match expected value at index " << j;
            }
            break;
        }

        free(path_to_sum);
    }
    free(leafs_array);
    freeTree(root);
    free(input_array);
}

class StrongConnectedGraphTest : public ::testing::TestWithParam<std::tuple<const char*, int>> {};

TEST_P(StrongConnectedGraphTest, Base)
{
    // Retrieve test parameters
    const char* path = std::get<0>(GetParam());
    int SAMPLE_IDX = std::get<1>(GetParam());

    const int ELEMENTS_PER_EDGE = 2; // Each edge has a source and a destination

    sds inSuffix = sdscatfmt(sdsempty(), "%i.txt", SAMPLE_IDX);
    sds outSuffix = sdscatfmt(sdsempty(), "%i.output.txt", SAMPLE_IDX);

    sds *input_tokens, *real_input_tokens, *output_tokens, *real_output_tokens;
    struct f_list* file_list = NULL;
    int* graphEdges;
    int* expectedOutput;
    int count, j, real_t_count = 0, outputSize = 0;

    get_zipped_files(&file_list, path);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;

    for(int64_t idx = 0; idx < entries_count; ++idx){        
        if (sdsendswith(targetX->name, inSuffix)){
            input_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &count);
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

            // Allocate memory for all (real_t_count * ELEMENTS_PER_EDGE) integers
            graphEdges = (int*)malloc(real_t_count * ELEMENTS_PER_EDGE * sizeof(int));
            if (graphEdges == NULL) perror("Failed to allocate memory for graphEdges");

            for (j = 0; j < real_t_count; j++) {
                if (sdslen(real_input_tokens[j]) > 0) {
                    int eCount = 0;
                    sds* edge = sdssplitlen(real_input_tokens[j], sdslen(real_input_tokens[j]), " ", 1, &eCount);
                    edge[1] = sdstrim(edge[1],"\r");

                    // Source
                    if (sdsToInt(edge[0], &graphEdges[j * ELEMENTS_PER_EDGE + 0]) != 0) printf("IN: sdsToInt conversion failed for edge[0]: %s\n", edge[0]);
                    
                    // Destination
                    if (sdsToInt(edge[1], &graphEdges[j * ELEMENTS_PER_EDGE + 1]) != 0) printf("IN: sdsToInt conversion failed for edge[1]: %s\n", edge[1]);
                    
                    sdsfreesplitres(edge, eCount); // Free the edge tokens
                }
            }   
            sdsfreesplitres(input_tokens, count);
            free(real_input_tokens);         
        }
        else if (sdsendswith(targetX->name, outSuffix)) {
            output_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), " ", 1, &count);
            real_output_tokens = (sds *)malloc(count * sizeof(sds));
            if (real_output_tokens == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                sdsfreesplitres(output_tokens, count);  // Free tokens array before exiting
                list_destroy(file_list);
                return;
            }

            for (j = 0; j < count; j++) {
                if (sdslen(output_tokens[j]) > 0) 
                    real_output_tokens[outputSize++] = output_tokens[j]; // Assign, don't copy!
            }
            
            //Reallocate real_tokens to the exact size
            real_output_tokens = (sds*)realloc(real_output_tokens, outputSize * sizeof(sds));
            if (real_output_tokens == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                sdsfreesplitres(output_tokens, count);
                list_destroy(file_list);
                return;
            }

            expectedOutput = (int*)malloc(outputSize * sizeof(int));
            if (expectedOutput == NULL) {
                fprintf(stderr, "Memory allocation failed for expectedOutput\n");
                sdsfreesplitres(output_tokens, count);
                free(real_output_tokens);
                list_destroy(file_list);
                return;
            }
            for (j = 0; j < outputSize; j++) {
                if (sdsToInt(real_output_tokens[j], &expectedOutput[j]) != 0) {
                    fprintf(stderr, "OUT: sdsToInt conversion failed for output token: %s\n", real_output_tokens[j]);
                    free(real_output_tokens);
                    list_destroy(file_list);
                    return;
                }
            }
            sdsfreesplitres(output_tokens, count);
            free(real_output_tokens); // Free the array holding expected output
        }
        targetX = targetX->next;
    }
    // --- Graph Creation using the populated graphEdges array ---

    // 1. Find the maximum vertex number from the dynamically allocated edges
    int maxVertex = findMaxVertex(graphEdges, real_t_count, ELEMENTS_PER_EDGE);

    // 2. Create the graph structure
    struct Graph* graph = createGraph(maxVertex);
    if (graph == NULL) {
        free(graphEdges); // Clean up already allocated edge data
        return; // Indicate error
    }

    // 3. Add edges to the graph using the data from graphEdges
    for (int i = 0; i < real_t_count; i++) {
        int src = graphEdges[i * ELEMENTS_PER_EDGE + 0];
        int dest = graphEdges[i * ELEMENTS_PER_EDGE + 1];
        addEdge(graph, src, dest);
    }

    // 4. Calculate Strong Connected Components sizes using Kosaraju's algorithm
    int numSCCs = 0;
    int* sccSizes = countStrongConnectedComponentsSize(graph, &numSCCs);

    ASSERT_NE(sccSizes, nullptr) << "countStrongConnectedComponentsSize returned NULL for a valid graph.";
    EXPECT_EQ(numSCCs, outputSize) 
        << "Number of SCCs found: " << numSCCs 
        << ", expected: " << outputSize;

    for (int i = 0; i < numSCCs; i++) {
        EXPECT_EQ(expectedOutput[outputSize - (i + 1)], sccSizes[i]) 
            << "Expected size of SCC " << expectedOutput[outputSize - (i + 1)] 
            << " does not match the actual size " << sccSizes[i];
    }
    free(sccSizes); // Free the array returned by countStrongConnectedComponentsSize

    // 5. Free all allocated memory
    free(graphEdges); // Free the array holding edge data
    freeGraph(graph); // Free the adjacency list graph structure
    free(expectedOutput); // Free the expected output array
    sdsfree(inSuffix); // Free the input file suffix
    sdsfree(outSuffix); // Free the output file suffix
    list_destroy(file_list);
}

INSTANTIATE_TEST_SUITE_P(
    StrongConnectedGraphParamTests, 
    StrongConnectedGraphTest,
    ::testing::Values(
        std::make_tuple("../data/test_08.zip", 1),
        std::make_tuple("../data/test_08.zip", 2),
        std::make_tuple("../data/test_08.zip", 3),
        std::make_tuple("../data/test_08.zip", 4)
    )
);

TEST(test_strong_connected_graph_main, main)
{
    const char* PATH = "../data/input_08.txt";
    int EXPECTED_RESULT[] = { 434821, 968, 459, 313, 211 };
    const int ELEMENTS_PER_EDGE = 2; // Each edge has a source and a destination

    sds *input_tokens, *real_input_tokens;
    int count, j, real_t_count = 0, outputSize = 0;
    int* graphEdges;

    char *input_content = read_text_file(PATH);
    input_tokens = sdssplitlen(input_content, strlen(input_content), "\n", 1, &count);
            
    real_input_tokens = (sds *)malloc(count * sizeof(sds));
    if (real_input_tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sdsfreesplitres(input_tokens, count);  // Free tokens array before exiting
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
        return;
    }

    // Allocate memory for all (real_t_count * ELEMENTS_PER_EDGE) integers
    graphEdges = (int*)malloc(real_t_count * ELEMENTS_PER_EDGE * sizeof(int));
    if (graphEdges == NULL) perror("Failed to allocate memory for graphEdges");

    for (j = 0; j < real_t_count; j++) {
        if (sdslen(real_input_tokens[j]) > 0) {
            int eCount = 0;
            sds* edge = sdssplitlen(real_input_tokens[j], sdslen(real_input_tokens[j]), " ", 1, &eCount);
            edge[1] = sdstrim(edge[1],"\r");

            // Source
            if (sdsToInt(edge[0], &graphEdges[j * ELEMENTS_PER_EDGE + 0]) != 0) printf("IN: sdsToInt conversion failed for edge[0]: %s\n", edge[0]);
            
            // Destination
            if (sdsToInt(edge[1], &graphEdges[j * ELEMENTS_PER_EDGE + 1]) != 0) printf("IN: sdsToInt conversion failed for edge[1]: %s\n", edge[1]);
            
            sdsfreesplitres(edge, eCount); // Free the edge tokens
        }
    }   
    // --- Graph Creation using the populated graphEdges array ---

    // 1. Find the maximum vertex number from the dynamically allocated edges
    int maxVertex = findMaxVertex(graphEdges, real_t_count, ELEMENTS_PER_EDGE);

    // 2. Create the graph structure
    struct Graph* graph = createGraph(maxVertex);
    if (graph == NULL) {
        free(graphEdges); // Clean up already allocated edge data
        return; // Indicate error
    }

    // 3. Add edges to the graph using the data from graphEdges
    for (int i = 0; i < real_t_count; i++) {
        int src = graphEdges[i * ELEMENTS_PER_EDGE + 0];
        int dest = graphEdges[i * ELEMENTS_PER_EDGE + 1];
        addEdge(graph, src, dest);
    }

    // 4. Calculate Strong Connected Components sizes using Kosaraju's algorithm
    int numSCCs = 0;
    int* sccSizes = countStrongConnectedComponentsSize(graph, &numSCCs);

    ASSERT_NE(sccSizes, nullptr) << "countStrongConnectedComponentsSize returned NULL for a valid graph.";

    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(EXPECTED_RESULT[i], sccSizes[numSCCs - (i + 1)]) 
            << "Expected size of SCC " << EXPECTED_RESULT[i] 
            << " does not match the actual size " << sccSizes[numSCCs - (i + 1)];
    }
    free(sccSizes); // Free the array returned by countStrongConnectedComponentsSize

    // 5. Free all allocated memory
    free(graphEdges); // Free the array holding edge data
    freeGraph(graph); // Free the adjacency list graph structure
    free(real_input_tokens); // Free the array holding input tokens
    sdsfreesplitres(input_tokens, count); // Free the input tokens array
    free(input_content); // Free the content read from the file 
}

class TrainingHSortestPathTest : public ::testing::TestWithParam<std::tuple<int, int>> {};

TEST_P(TrainingHSortestPathTest, TrainingDijkstra)
{
    const char* ZIPPED_FOLDER = "../data/test_09.zip";
    int ELEMENTS_PER_EDGE = 3; // Each edge has a source, destination and a weight
    sds NO_LINK = sdsnew("--");

    // Retrieve test parameters
    int F_SUFFIX = std::get<0>(GetParam());
    int SIZE = std::get<1>(GetParam());

    sds input_file = sdscatfmt(sdsempty(), "input_%i_%i.txt", F_SUFFIX, SIZE);
    sds output_file = sdscatfmt(sdsempty(), "output_%i.txt", F_SUFFIX);

    sds *input_tokens, *real_input_tokens, *output_tokens, *real_output_tokens;
    struct f_list* file_list = NULL;
    int* graphEdges;
    int** expectedOutputMatrix;
    int count, j, real_t_count = 0, outputSize = 0, input_size = 0, output_size = 0;
    
    get_zipped_files(&file_list, ZIPPED_FOLDER);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;

    for(int64_t idx = 0; idx < entries_count; ++idx) {    
        if (sdscmp(input_file, targetX->name) == 0) {
            input_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &count);
            real_input_tokens = (sds *)malloc(count * sizeof(sds));
            if (real_input_tokens == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                sdsfreesplitres(input_tokens, count);  // Free tokens array before exiting
                list_destroy(file_list);
                return;
            }

            for (j = 1; j < count; j++) {
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
            
            // Allocate memory for all (real_t_count * ELEMENTS_PER_EDGE) integers
            graphEdges = (int*)malloc(real_t_count * ELEMENTS_PER_EDGE * sizeof(int));
            if (graphEdges == NULL) perror("Failed to allocate memory for graphEdges");

            for (j = 0; j < real_t_count; j++) {
                if (sdslen(real_input_tokens[j]) > 0) {
                    int eCount = 0;
                    sds* edge = sdssplitlen(real_input_tokens[j], sdslen(real_input_tokens[j]), " ", 1, &eCount);
                    edge[1] = sdstrim(edge[1],"\r");
                    edge[2] = sdstrim(edge[2],"\r");

                    // Source
                    if (sdsToInt(edge[0], &graphEdges[j * ELEMENTS_PER_EDGE + 0]) != 0) printf("IN: sdsToInt conversion failed for edge[0]: %s\n", edge[0]);
                    
                    // Destination
                    if (sdsToInt(edge[1], &graphEdges[j * ELEMENTS_PER_EDGE + 1]) != 0) printf("IN: sdsToInt conversion failed for edge[1]: %s\n", edge[1]);
                    
                    // Weight
                    if (sdsToInt(edge[2], &graphEdges[j * ELEMENTS_PER_EDGE + 2]) != 0) printf("IN: sdsToInt conversion failed for edge[2]: %s\n", edge[2]);

                    sdsfreesplitres(edge, eCount); // Free the edge tokens
                }
            }   
            sdsfreesplitres(input_tokens, count);
            free(real_input_tokens);  
        }
        else if (sdscmp(output_file, targetX->name) == 0) {
            output_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), "\n", 1, &count);
            real_output_tokens = (sds *)malloc(count * sizeof(sds));
            if (real_output_tokens == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                sdsfreesplitres(output_tokens, count);  // Free tokens array before exiting
                list_destroy(file_list);
                return;
            }
            for (j = 0; j < count; j++) {
                if (sdslen(output_tokens[j]) > 0) 
                    real_output_tokens[outputSize++] = output_tokens[j]; // Assign, don't copy!
            }
            //Reallocate real_tokens to the exact size
            real_output_tokens = (sds*)realloc(real_output_tokens, outputSize * sizeof(sds));
            if (real_output_tokens == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                sdsfreesplitres(output_tokens, output_size);
                list_destroy(file_list);   
                return;
            }
            // Allocate memory for expected output
            expectedOutputMatrix = (int**)malloc(SIZE * sizeof(int*));
            if (!expectedOutputMatrix) {
                perror("Failed to allocate memory for matrix rows");
                return;
            }
            for (int i = 0; i < SIZE; ++i) {
                expectedOutputMatrix[i] = (int*)malloc(SIZE * sizeof(int));
                if (!expectedOutputMatrix[i]) {
                    perror("Failed to allocate memory for matrix columns");
                    // Clean up already allocated rows
                    for (int j = 0; j < i; ++j) {
                        free(expectedOutputMatrix[j]);
                    }
                    free(expectedOutputMatrix);
                    return;
                }
            }

            for (j = 0; j < outputSize; j++) {
                if (sdslen(real_output_tokens[j]) > 0) {
                    int eCount = 0;
                    sds* row = sdssplitlen(real_output_tokens[j], sdslen(real_output_tokens[j]), " ", 1, &eCount);
                    for (int k = 0; k < SIZE; k++) {
                        if (sdscmp(row[k], NO_LINK) == 0) {
                            expectedOutputMatrix[j][k] = INT_MAX; // Use INT_MAX to indicate no link
                        }
                        else if (sdsToInt(row[k], &expectedOutputMatrix[j][k]) != 0) {
                            printf("OUT: sdsToInt conversion failed for output token: %s\n", row[k]);
                            // Free the row tokens
                            sdsfreesplitres(row, eCount);
                            free(real_output_tokens);
                            list_destroy(file_list);
                            return;
                        }
                    }
                    sdsfreesplitres(row, eCount);
                }
            }
            sdsfreesplitres(output_tokens, count); 
            free(real_output_tokens);  
        }
        targetX = targetX->next;
    }
    WeightedGraph* graph = createWeightedGraph(SIZE);
    for (j = 0; j < real_t_count; j++) 
        addWeightedEdge(graph, graphEdges[j * ELEMENTS_PER_EDGE + 0], graphEdges[j * ELEMENTS_PER_EDGE + 1], graphEdges[j * ELEMENTS_PER_EDGE + 2]);

    for (int source_vertex = 1; source_vertex <= SIZE; source_vertex++){
        int* shortest_distances = dijkstraSlow(graph, source_vertex);
        for (int i = 0; i < SIZE; i++) {
            EXPECT_EQ(shortest_distances[i], expectedOutputMatrix[source_vertex - 1][i])
                << "Shortest distance from vertex " << source_vertex 
                << " to vertex " << (i + 1) 
                << " is " << shortest_distances[i] 
                << ", expected: " << expectedOutputMatrix[source_vertex - 1][i];
        }
        // IMPORTANT: Free the memory returned by dijkstra
        free(shortest_distances);
        shortest_distances = NULL; // Good practice to set to NULL after freeing
    }
    sdsfree(NO_LINK); // Free the NO_LINK string
    sdsfree(input_file); // Free the input file suffix
    sdsfree(output_file); // Free the output file suffix
    free(graphEdges); // Free the array holding edge data
    for (int i = 0; i < SIZE; ++i) {
        free(expectedOutputMatrix[i]); // Free each row of the expected output matrix
    }
    free(expectedOutputMatrix); // Free the expected output matrix
    list_destroy(file_list); // Free the file list
    freeWeightedGraph(graph); // Free all graph memory
}

INSTANTIATE_TEST_SUITE_P(
    DijkstraParamTests, 
    TrainingHSortestPathTest,
    ::testing::Values(
        std::make_tuple(1, 100),
        std::make_tuple(2, 100),
        std::make_tuple(3, 100),
        std::make_tuple(4, 1000),
        std::make_tuple(5, 10),
        std::make_tuple(6, 10),
        std::make_tuple(7, 10),
        std::make_tuple(8, 10)
    )
);
 ****** */

 TEST(test_dijkstra_shortest_path_main, main)
{
    const char* PATH = "../data/USA-FLA.txt";
    const int ELEMENTS_PER_EDGE = 3; // Each edge has a source, destination and a weight
    int vertexAmount = 1070376, startPoint = 100562, endPoint = 1070345, expectedDistance = 6699685, expectedUniquePaths = 4;
    
    sds *input_tokens, *real_input_tokens;
    int count, j, real_t_count = 0, outputSize = 0;
    int* graphEdges;

    char *input_content = read_text_file(PATH);
    input_tokens = sdssplitlen(input_content, strlen(input_content), "\n", 1, &count);
            
    real_input_tokens = (sds *)malloc(count * sizeof(sds));
    if (real_input_tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sdsfreesplitres(input_tokens, count);  // Free tokens array before exiting
        return;
    }
                       
    for (j = 1; j < count; j++) {
        if (sdslen(input_tokens[j]) > 0) 
            real_input_tokens[real_t_count++] = input_tokens[j]; // Assign, don't copy!
    }

    //Reallocate real_tokens to the exact size
    real_input_tokens = (sds*)realloc(real_input_tokens, real_t_count * sizeof(sds));
    if (real_input_tokens == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        sdsfreesplitres(input_tokens, count);
        return;
    }

    // Allocate memory for all (real_t_count * ELEMENTS_PER_EDGE) integers
    graphEdges = (int*)malloc(real_t_count * ELEMENTS_PER_EDGE * sizeof(int));
    if (graphEdges == NULL) perror("Failed to allocate memory for graphEdges");

    for (j = 0; j < real_t_count; j++) {
        if (sdslen(real_input_tokens[j]) > 0) {
            int eCount = 0;
            sds* edge = sdssplitlen(real_input_tokens[j], sdslen(real_input_tokens[j]), " ", 1, &eCount);
            edge[1] = sdstrim(edge[1],"\r");

            // Source
            if (sdsToInt(edge[0], &graphEdges[j * ELEMENTS_PER_EDGE + 0]) != 0) printf("IN: sdsToInt conversion failed for edge[0]: %s\n", edge[0]);
            
            // Destination
            if (sdsToInt(edge[1], &graphEdges[j * ELEMENTS_PER_EDGE + 1]) != 0) printf("IN: sdsToInt conversion failed for edge[1]: %s\n", edge[1]);
            
            // Weight
            if (sdsToInt(edge[2], &graphEdges[j * ELEMENTS_PER_EDGE + 2]) != 0) printf("IN: sdsToInt conversion failed for edge[2]: %s\n", edge[2]);

            sdsfreesplitres(edge, eCount); // Free the edge tokens
        }
    }   
    // --- Graph Creation using the populated graphEdges array ---
    WeightedGraph* graph = createWeightedGraph(vertexAmount);
    for (j = 0; j < real_t_count; j++) 
        addWeightedEdge(graph, graphEdges[j * ELEMENTS_PER_EDGE + 0], graphEdges[j * ELEMENTS_PER_EDGE + 1], graphEdges[j * ELEMENTS_PER_EDGE + 2]);

    DijkstraResult* result = dijkstra(graph, startPoint);
    EXPECT_EQ(result->distances[endPoint - 1], expectedDistance)
        << "Shortest distance from vertex " << startPoint 
        << " to vertex " << endPoint 
        << " is " << result->distances[endPoint - 1] 
        << ", expected: " << expectedDistance;
    std::cout << result->distances[endPoint - 1] << "   " << result->path_counts[endPoint - 1] << std::endl;
    // IMPORTANT: Free the memory returned by dijkstra
    free(result->distances);
    free(result->path_counts);
    free(result);
    
}