#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

extern "C"
{
#include "../helpers/zip_helper/zip_helper.h"
#include "../modules/data_structures/heap.h"
#include "../modules/data_structures/hash_table.h"
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
);
