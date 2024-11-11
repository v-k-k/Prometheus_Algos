#include <gtest/gtest.h>

extern "C"
{
#include "../modules/sorting/sorting.h"
#include "../helpers/zip_helper/zip_helper.h"
#include "../helpers/tools/utils.h"
#include "../helpers/simple_dynamic_string/sds.h"
#include "../helpers/payload_extractors/payload_extractors.h"
}

TEST(test_inversions_counting, sorting)
{
    char* path = "../data/data_examples_02.zip";
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
        }

        // Filter out empty tokens
        for (j = 0; j < count; j++) {
            if (sdslen(tokens[j]) > 0)       // Only consider non-empty tokens
                real_tokens[real_t_count++] = tokens[j];
             else
                sdsfree(tokens[j]);            // Free empty tokens to avoid memory leak
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
        for (PARSING_LINE++; PARSING_LINE < MOVE_TO_LINE; PARSING_LINE++){
            int_row = NULL;
            row_size = 0;
            sds_to_int_array(real_tokens[PARSING_LINE], &int_row, &row_size);
            arrays[PARSING_LINE - 1] = copy_without_first(int_row, films + 1);
        }
    
        MOVE_TO_LINE += (PARSING_LINE - 1);
        for (int k = 0; k < 2; k++){
            int **copy = copy_arrays(arrays, users, films);
            int CURRENT_USER = atoi(real_tokens[PARSING_LINE]);
            int *expected_inversions[users - 1];

            for (PARSING_LINE++; PARSING_LINE < MOVE_TO_LINE; PARSING_LINE++){
                int_row = NULL;
                row_size = 0;
                sds_to_int_array(real_tokens[PARSING_LINE], &expected_inversions[PARSING_LINE + users - 1 - MOVE_TO_LINE], &row_size);
            }
            
            int base = CURRENT_USER - 1;

            sort_with_corresponding(copy, users, films, base);

            for (int i = 0, j = 0; i < users; i++, j++){
                if (i != base)
                    EXPECT_EQ(expected_inversions[j][1], count_inversions(copy[i], films, 0));
                else
                    j--;
            }
            MOVE_TO_LINE += users;

            for (int i = 0; i < users; i++) 
                free(copy[i]);    
            free(copy);
        }
        targetX = targetX->next;
    }
}

// TEST(test_karatsuba, simple_arr)
// {
//     int arr[] = {1, 5, 4, 6, 7, 9, 8, 10, 2, 3};
//     size_t n = sizeof(arr) / sizeof(arr[0]);
//     float mean = calculate_mean(arr, n);
//     EXPECT_FLOAT_EQ(mean, 5.5);
// }

// TEST(test_karatsuba, empty_arr)
// {
//     int arr[] = {};
//     float mean = calculate_mean(arr, 0);
//     EXPECT_FLOAT_EQ(mean, 0);
// }
// TEST(test_karatsuba, all_negatives)
// {
//     int arr[] = {-1, -5, -4, -6, -7, -9, -8, -10, -2, -3};
//     size_t n = sizeof(arr) / sizeof(arr[0]);
//     float mean = calculate_mean(arr, n);
//     EXPECT_FLOAT_EQ(mean, -5.5);
// }
// TEST(test_karatsuba, mix_negative_positive)
// {
//     int arr[] = {-1, -5, -4, 6, 7, 9, -8, -10, -2, -3};
//     size_t n = sizeof(arr) / sizeof(arr[0]);
//     float mean = calculate_mean(arr, n);
//     EXPECT_FLOAT_EQ(mean, -1.1);
// }
// TEST(test_karatsuba, with_zeros)
// {
//     int arr[] = {-1, -5, -4, 0, 7, 9, 0, -10, -2, -3};
//     size_t n = sizeof(arr) / sizeof(arr[0]);
//     float mean = calculate_mean(arr, n);
//     EXPECT_FLOAT_EQ(mean, -0.89999998);
// }