#include <gtest/gtest.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

extern "C"
{
#include "../helpers/zip_helper/zip_helper.h"
#include "../modules/sorting/sorting.h"
}

class TrainingQuickSortTest : public ::testing::TestWithParam<std::tuple<const char*, int>> {};

TEST_P(TrainingQuickSortTest, Training) {
    // Retrieve test parameters
    const char* ZIPPED_FOLDER = std::get<0>(GetParam());
    int ARRAY_SIZE = std::get<1>(GetParam());
    
    int MODIFICATIONS = 3;
    int input_array[ARRAY_SIZE];
    int full_input_container[MODIFICATIONS][ARRAY_SIZE];
    int expected_comparisons[MODIFICATIONS];

    struct f_list* file_list = NULL;
    
    // Array of function pointers
    int (*sortFunctions[])(int[], int, int, int*) = {sortByLast, sortByFirst, sortByMedian};

    // Corresponding counters
    int firstCounter = 0, medianCounter = 0, lastCounter = 0;
    int* counters[] = {&lastCounter, &firstCounter, &medianCounter};
    
    get_zipped_files(&file_list, ZIPPED_FOLDER);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;

    for(int64_t idx = 0; idx < entries_count; ++idx){
        sds *input_tokens, *real_input_tokens, *output_tokens, *real_output_tokens;
        sds suffix = sdscatfmt(sdsempty(), "%i.txt", ARRAY_SIZE);
        if (sdsendswith(targetX->name, suffix)) {
            int count, j, real_t_count = 0;
            if (sdsstartswith(targetX->name, "input")) {               
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

                int input_size;
                if (sdsToInt(real_input_tokens[0], &input_size) != 0) return;

                EXPECT_EQ(ARRAY_SIZE, input_size)
                    << "ARRAY_SIZE = " << ARRAY_SIZE << "\n"
                    << "input_size = " << input_size;

                for (j = 0; j < input_size; j++) 
                    if (sdsToInt(real_input_tokens[j + 1], &input_array[j]) != 0) return; 
                
                sdsfreesplitres(input_tokens, count);
                free(real_input_tokens);
            }
            else {
                output_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), " ", 1, &count);
                
                EXPECT_EQ(MODIFICATIONS, count)
                    << "MODIFICATIONS = " << MODIFICATIONS << "\n"
                    << "count = " << count;

                for (j = 0; j < MODIFICATIONS; j++)
                    if (sdsToInt(output_tokens[j], &expected_comparisons[j]) != 0) return; 

                sdsfreesplitres(output_tokens, count);
            }
        }        
        targetX = targetX->next;
        sdsfree(suffix);
    }
    list_destroy(file_list);

    // Check if the expected comparisons match the actual comparisons
    for (int i = 0; i < MODIFICATIONS; i++) {
        // Reset counter
        int counterValue = *counters[i]; 

        // Copy input_array in full_input_container
        for (int k = 0; k < ARRAY_SIZE; k++) 
            full_input_container[i][k] = input_array[k];

        // Call each function in a loop
        sortFunctions[i](full_input_container[i], 0, ARRAY_SIZE - 1, counters[i]);
        EXPECT_EQ(expected_comparisons[i], *counters[i])
            << "Training input\n"
            << ARRAY_SIZE << " samples\n"
            << "Expected comparisons " << expected_comparisons[i] << "\n"
            << "Actual counter value " << *counters[i];
    }
}

INSTANTIATE_TEST_SUITE_P(
    QuickSortParamTests, 
    TrainingQuickSortTest,
    ::testing::Values(
        std::make_tuple("../data/data_examples_03.zip", 10),
        std::make_tuple("../data/data_examples_03.zip", 100),
        std::make_tuple("../data/data_examples_03.zip", 1000)
    )
);/*

TEST(test_quick_sorting, training)
{
    int ARRAY_SIZE = 10;
    int MODIFICATIONS = 3;
    int input_array[ARRAY_SIZE];
    int full_input_container[MODIFICATIONS][ARRAY_SIZE];
    int expected_comparisons[MODIFICATIONS];
    const char* path = "../data/data_examples_03.zip";
    struct f_list* file_list = NULL;    

    // Array of function pointers
    int (*sortFunctions[])(int[], int, int, int*) = {sortByLast, sortByFirst, sortByMedian};

    // Corresponding counters
    int firstCounter = 0, medianCounter = 0, lastCounter = 0;
    int* counters[] = {&lastCounter, &firstCounter, &medianCounter};
    
    get_zipped_files(&file_list, path);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;

    for(int64_t idx = 0; idx < entries_count; ++idx){
        sds *input_tokens, *real_input_tokens, *output_tokens, *real_output_tokens;

        if (sdsendswith(targetX->name, sdscatfmt(sdsempty(), "%i.txt", ARRAY_SIZE))) {
            int count, j, real_t_count = 0;
            if (sdsstartswith(targetX->name, "input")) {               
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

                int input_size;
                if (sdsToInt(real_input_tokens[0], &input_size) != 0) return;

                EXPECT_EQ(ARRAY_SIZE, input_size)
                    << "ARRAY_SIZE = " << ARRAY_SIZE << "\n"
                    << "input_size = " << input_size;

                for (j = 0; j < input_size; j++) {
                    if (sdsToInt(real_input_tokens[j + 1], &input_array[j]) != 0) return; 
                }
            }
            else {
                output_tokens = sdssplitlen(targetX->content, sdslen(targetX->content), " ", 1, &count);
                real_output_tokens = (sds *)malloc(count * sizeof(sds));
                if (real_output_tokens == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    sdsfreesplitres(output_tokens, count);  // Free tokens array before exiting
                    list_destroy(file_list);
                    return;
                }
                
                EXPECT_EQ(MODIFICATIONS, count)
                    << "MODIFICATIONS = " << MODIFICATIONS << "\n"
                    << "count = " << count;

                for (j = 0; j < MODIFICATIONS; j++)
                    if (sdsToInt(output_tokens[j], &expected_comparisons[j]) != 0) return; 
            }
        }        
        targetX = targetX->next;
    }

    // Check if the expected comparisons match the actual comparisons
    for (int i = 0; i < MODIFICATIONS; i++) {
        // Reset counter
        int counterValue = *counters[i]; 

        // Copy input_array in full_input_container
        for (int k = 0; k < ARRAY_SIZE; k++) 
            full_input_container[i][k] = input_array[k];

        // Call each function in a loop
        sortFunctions[i](full_input_container[i], 0, ARRAY_SIZE - 1, counters[i]);
        EXPECT_EQ(expected_comparisons[i], *counters[i])
            << "Training input\n"
            << ARRAY_SIZE << " samples\n"
            << "Expected comparisons " << expected_comparisons[i] << "\n"
            << "Actual counter value " << *counters[i];
    }
}*/
