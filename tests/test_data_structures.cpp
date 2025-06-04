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
    int count = 0;
    sds content = retrievePlainText(DATA_SOURCE.c_str());
    sds *input_tokens = sdssplitlen(content, sdslen(content), "\n", 1, &count);

    std::cout << "Input tokens count: " << count << std::endl;
    /*std::vector<sds> EXPECTED_PASSWORDS = {sdsnew("aaolzzr"), sdsnew("aaojzzr")};

    sort_strings(input_tokens, count);
    sds password = generate_password(input_tokens, count);
    
    //EXPECT_THAT(EXPECTED_PASSWORDS, ::testing::Contains(password))
    EXPECT_TRUE(std::any_of(EXPECTED_PASSWORDS.begin(), EXPECTED_PASSWORDS.end(),
                        [&](const sds& stored_password) { return sdscmp(stored_password, password) == 0; }))
    << "Password " << password << " not found in EXPECTED_PASSWORDS!";


    sdsfree(content);
    sdsfree(password);
    for (int i = 0; i < count; ++i) {
        sdsfree(input_tokens[i]);
    }
    free(input_tokens);
    for (const auto& expected_password : EXPECTED_PASSWORDS) {
        sdsfree(expected_password);
    }
    EXPECTED_PASSWORDS.clear(); // Optionally clear the vector*/
}
