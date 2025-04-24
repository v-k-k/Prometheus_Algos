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

TEST(test_inversions_counting, basic)
{
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
}


class MainInversionsTest : public ::testing::TestWithParam<std::tuple<std::string, int, int, int>> {};

TEST_P(MainInversionsTest, FiveFilms) {
    // Retrieve test parameters
    std::string DATA_SOURCE = std::get<0>(GetParam());
    int CURRENT_USER = std::get<1>(GetParam());
    int REL_USER = std::get<2>(GetParam());
    int EXPECTED_INVERSIONS = std::get<3>(GetParam());

    int users, films;
    int **users_films_ratings;

    parse_inversions_data(DATA_SOURCE.c_str(), &users_films_ratings, &users, &films);
    char* TEST_NAME = fileNameFromUrl(DATA_SOURCE.c_str());

    sort_with_corresponding(users_films_ratings, users, films, CURRENT_USER - 1);

    int calculatedUserInversions = count_inversions(users_films_ratings[REL_USER - 1], films, 0);

    EXPECT_EQ(EXPECTED_INVERSIONS, calculatedUserInversions)
        << "Sample \"" << TEST_NAME << "\"\n"
        << "User " << CURRENT_USER << "\n"
        << "Expected split inversions count " << EXPECTED_INVERSIONS << "\n"
        << "Actual split inversions count " << calculatedUserInversions;

    // Assume 'users_films_ratings' was allocated as an array of integer pointers
    for (int i = 0; i < users; i++) {
        free(users_films_ratings[i]); // Free each user's film ratings array
    }

    free(users_films_ratings); // Free the array of pointers itself
    free(TEST_NAME); // Free the filename string
}

INSTANTIATE_TEST_SUITE_P(
    InversionsParamTests, 
    MainInversionsTest,
    ::testing::Values(
        std::make_tuple("https://courses.prometheus.org.ua/assets/courseware/v1/9a95f1a8992d060eafde59c96919dede/c4x/KPI/Algorithms101/asset/input_1000_5.txt", 863, 29, 0),
        std::make_tuple("https://courses.prometheus.org.ua/assets/courseware/v1/9a95f1a8992d060eafde59c96919dede/c4x/KPI/Algorithms101/asset/input_1000_5.txt", 452, 100, 7),
        std::make_tuple("https://courses.prometheus.org.ua/assets/courseware/v1/c2ab5f7283f9767fb6bad4739237c13c/c4x/KPI/Algorithms101/asset/input_1000_100.txt", 618, 1, 2368),
        std::make_tuple("https://courses.prometheus.org.ua/assets/courseware/v1/c2ab5f7283f9767fb6bad4739237c13c/c4x/KPI/Algorithms101/asset/input_1000_100.txt", 951, 178, 2483)
    )
);
