#include <gtest/gtest.h>
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
    std::string DATA_SOURCE = "https://courses.prometheus.org.ua/assets/courseware/v1/c527b289b77bd5c7d2851ca728471685/c4x/KPI/Algorithms101/asset/anagrams.txt";
    int count = 0;
    sds content = retrievePlainText(DATA_SOURCE.c_str());
    sds *input_tokens = sdssplitlen(content, sdslen(content), "\n", 1, &count);

    // Generating Test Data DB from SQL_FILE
    generate_test_data_db();
    
    // Getting Letters Rate
    StringIntDictionary *letters_rate = get_letters_rate();
    EXPECT_NE(letters_rate, nullptr) << "letters_rate should not be null after calling get_letters_rate()";

    // Getting Strings Collections
    StringArrayTuple *strings_collections = get_strings_collections();
    EXPECT_NE(strings_collections, nullptr) << "strings_collections should not be null after calling get_strings_collections()";

    if (strings_collections) {
        printf("Base Strings:\n");
        for (int i = 0; i < strings_collections->array1_count; ++i) {
            printf("- %s\n", strings_collections->array1[i]);
        }
        printf("Sorted Strings:\n");
        for (int i = 0; i < strings_collections->array2_count; ++i) {
            printf("- %s\n", strings_collections->array2[i]);
        }
        
    } else {
        printf("Failed to get strings collections.\n");
    }
    printf("\n");

    // Getting Result Password
    char *expected_password = get_result_password();
    EXPECT_NE(expected_password, nullptr) << "expected_password should not be null after calling get_result_password()";

    printf("\n\n\n2222222222222222\n\n\n");
    char* original_strings[] = {"banana", "apple", "orange", "grape"};
    int n_str = sizeof(original_strings) / sizeof(original_strings[0]);

    // Dynamically allocate an array to hold modifiable strings
    char** sample_str = (char**)malloc(n_str * sizeof(char*));
    if (!sample_str) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    // Copy original string data so it's modifiable
    for (int i = 0; i < n_str; i++) {
        sample_str[i] = strdup(original_strings[i]);
        if (!sample_str[i]) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }
    printf("Original string array: ");
    for (int i = 0; i < n_str; i++) printf("%s ", sample_str[i]);
    printf("\n");
    sort_strings(sample_str, n_str);
    printf("Sorted string array: ");
    for (int i = 0; i < n_str; i++) printf("%s ", sample_str[i]);
    printf("\n");
    // Free allocated memory
    for (int i = 0; i < n_str; i++) free(sample_str[i]);
    free(sample_str);
    /*char** baseArray = (char**)malloc(sizeof(char*) * strings_collections->array1_count);
    if (baseArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    for (int i = 0; i < strings_collections->array1_count; ++i) {
        printf("%s\n", strings_collections->array1[i]);
        baseArray[i] = (char*)malloc(strlen(strings_collections->array1[i]) + 1);
        if (baseArray[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) free(baseArray[j]);
            free(baseArray);
            return;
        }
        strcpy(baseArray[i], strings_collections->array1[i]);
    }
    sort_strings(baseArray, count);

    char* sample_str[] = {"banana", "apple", "orange", "grape"};
    int n_str = sizeof(sample_str) / sizeof(sample_str[0]);
    printf("Original string array: ");
    for (int i = 0; i < n_str; i++) printf("%s ", sample_str[i]);
    printf("\n");
    sort_strings(sample_str, n_str);
    printf("Sorted string array: ");
    for (int i = 0; i < n_str; i++) printf("%s ", sample_str[i]);
    printf("\n");

    char* password = generate_password(sample_str, n_str);
    printf("Generated password: %s\n", password);
    free(password);
    for (int i = 0; i < n_str; i++) free(sample_str[i]);*/

    free_string_int_dictionary(letters_rate);
    free_string_array_tuple(strings_collections);
    free(expected_password);
}
/*{
    const char* path = "../data/data_examples_02.zip";
    struct f_list* file_list = NULL;
    
    get_zipped_files(&file_list, path);
    
    size_t entries_count = list_length(file_list);
    struct f_list* targetX = file_list;

    for(int64_t idx = 0; idx < entries_count; ++idx){
        sds *tokens, *real_tokens;
        int count, j, real_t_count = 0;

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
        
        // Split the sds string into an integer array    
        int *int_row = NULL;
        int row_size, MOVE_TO_LINE, PARSING_LINE = 0;

        sds_to_int_array(real_tokens[PARSING_LINE], &int_row, &row_size);
        int users = int_row[0];
        int films = int_row[1];

        // Dynamically allocate and initialize each row
        int *arrays[users];
        MOVE_TO_LINE = users + 1;
        free(int_row);
        for (PARSING_LINE++; PARSING_LINE < MOVE_TO_LINE; PARSING_LINE++){
            int_row = NULL;
            row_size = 0;
            sds_to_int_array(real_tokens[PARSING_LINE], &int_row, &row_size);
            arrays[PARSING_LINE - 1] = copy_without_first(int_row, films + 1);
            free(int_row);
        }
    
        MOVE_TO_LINE += (PARSING_LINE - 1);
        for (int k = 0; k < 2; k++){
            int **copy = copy_arrays(arrays, users, films);
            int CURRENT_USER = atoi(real_tokens[PARSING_LINE]);
            int *expected_inversions[users - 1];

            for (PARSING_LINE++; PARSING_LINE < MOVE_TO_LINE; PARSING_LINE++){
                row_size = 0;
                sds_to_int_array(real_tokens[PARSING_LINE], &expected_inversions[PARSING_LINE + users - 1 - MOVE_TO_LINE], &row_size);                
            }
            
            int base = CURRENT_USER - 1;

            sort_with_corresponding(copy, users, films, base);

            for (int i = 0, j = 0; i < users; i++, j++){
                if (i != base){
                    EXPECT_EQ(expected_inversions[j][1], count_inversions(copy[i], films, 0));
                    free(expected_inversions[j]);
                }
                else
                    j--;
            }
            MOVE_TO_LINE += users;

            for (int i = 0; i < users; i++)
                free(copy[i]);
            free(copy);
        }
        for (int i = 0; i < users; i++)
            free(arrays[i]);
        sdsfreesplitres(tokens, count);
        free(real_tokens);
        targetX = targetX->next;
    }
    list_destroy(file_list);
}*/
