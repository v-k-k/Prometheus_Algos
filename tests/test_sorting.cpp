#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>

extern "C"
{
#include "../helpers/zip_helper/zip_helper.h"
#include "../modules/sorting/sorting.h"
#include "../helpers/db_helper/sqlite_client.h"
}

class TrainingQuickSortTest : public ::testing::TestWithParam<std::tuple<const char*, int>> {};

TEST_P(TrainingQuickSortTest, Training) {
    // Retrieve test parameters
    int ARRAY_SIZE = std::get<1>(GetParam());
    const char* ZIPPED_FOLDER = std::get<0>(GetParam());
    
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
);

class MainQuickSortTest : public ::testing::TestWithParam<std::tuple<int, int, int>> {};

TEST_P(MainQuickSortTest, Main) {
    // Retrieve test parameters
    int ARRAY_SIZE = std::get<0>(GetParam());
    int FUNC_TO_CALL = std::get<1>(GetParam());
    int EXPECTED_COMPARISONS = std::get<2>(GetParam());
    
    int MODIFICATIONS = 3;
    int input_array[ARRAY_SIZE];
    std::string DATA_SOURCE = "https://courses.prometheus.org.ua/assets/courseware/v1/31d63b32652e3d457f7b0c084b0019a8/asset-v1:KPI+Algorithms101+2015_Spring+type@asset+block/input__10000.txt";

    sds content = retrievePlainText(DATA_SOURCE.c_str());
    int input_size, count = 0;
    
    // Array of function pointers
    int (*sortFunctions[])(int[], int, int, int*) = {sortByLast, sortByFirst, sortByMedian};

    sds *input_tokens = sdssplitlen(content, sdslen(content), "\n", 1, &count);
    
    if (sdsToInt(input_tokens[0], &input_size) != 0) return;

    EXPECT_EQ(ARRAY_SIZE, input_size)
        << "ARRAY_SIZE = " << ARRAY_SIZE << "\n"
        << "input_size = " << input_size;

    for (int j = 0; j < input_size; j++) 
    if (sdsToInt(input_tokens[j + 1], &input_array[j]) != 0) return; 

    sdsfreesplitres(input_tokens, count);

    // Reset counter
    int* counterValue = (int*)malloc(sizeof(int));
    *counterValue = 0; 

    // Call each function in a loop
    sortFunctions[FUNC_TO_CALL](input_array, 0, ARRAY_SIZE - 1, counterValue);
    EXPECT_EQ(EXPECTED_COMPARISONS, *counterValue)
        << "Main input\n"
        << ARRAY_SIZE << " samples\n"
        << "Expected comparisons " << EXPECTED_COMPARISONS << "\n"
        << "Actual counter value " << *counterValue;

    free(counterValue); // Free the allocated memory for the counter
    sdsfree(content); // Free the content string
}

INSTANTIATE_TEST_SUITE_P(
    QuickSortParamTests, 
    MainQuickSortTest,
    ::testing::Values(
        std::make_tuple(10000, 0, 150262),
        std::make_tuple(10000, 1, 159864),
        std::make_tuple(10000, 2, 130957)
    )
);

TEST(test_radix_sort, training)
{
    // Generating Test Data DB from SQL_FILE
    generate_test_data_db();
    
    // Getting Letters Rate
    StringIntDictionary *letters_rate = get_letters_rate();
    EXPECT_NE(letters_rate, nullptr) << "letters_rate should not be null after calling get_letters_rate()";

    // Getting Strings Collections
    StringArrayTuple *strings_collections = get_strings_collections();
    EXPECT_NE(strings_collections, nullptr) << "strings_collections should not be null after calling get_strings_collections()";

    sds *sdsArray = (sds *)malloc(strings_collections->array1_count * sizeof(sds));
    if (!sdsArray) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    convertToSDS(strings_collections->array1, strings_collections->array1_count, sdsArray);

    // Getting expected Result Password
    char *raw_expected_password = get_result_password();
    EXPECT_NE(raw_expected_password, nullptr) << "raw_expected_password should not be null after calling get_result_password()";

    sds expected_password = sdsnew(raw_expected_password);
    sort_strings(sdsArray, strings_collections->array1_count);
    sds password = generate_password(sdsArray, strings_collections->array1_count);
    
    EXPECT_EQ(sdscmp(expected_password, password), 0) << "expected_password should be equal to password";

    // Free SDS strings
    for (size_t i = 0; i < strings_collections->array1_count; i++) {
        sdsfree(sdsArray[i]);
    }
    free(sdsArray);
    free_string_int_dictionary(letters_rate);
    free_string_array_tuple(strings_collections);
    free(raw_expected_password);
    sdsfree(expected_password);
    sdsfree(password);
}


TEST(test_radix_sort, main)
{
    std::string DATA_SOURCE = "https://courses.prometheus.org.ua/assets/courseware/v1/c527b289b77bd5c7d2851ca728471685/c4x/KPI/Algorithms101/asset/anagrams.txt";
    int count = 0;
    sds content = retrievePlainText(DATA_SOURCE.c_str());
    sds *input_tokens = sdssplitlen(content, sdslen(content), "\n", 1, &count);
    std::vector<sds> EXPECTED_PASSWORDS = {sdsnew("aaolzzr"), sdsnew("aaojzzr")};

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
    EXPECTED_PASSWORDS.clear(); // Optionally clear the vector
}
