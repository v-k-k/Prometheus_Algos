#include <gtest/gtest.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

extern "C"
{
#include "../helpers/zip_helper/zip_helper.h"
#include "../modules/sorting/sorting.h"
}

TEST(test_inversions_counting, sorting)
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
/*"https://courses.prometheus.org.ua/assets/courseware/v1/c2ab5f7283f9767fb6bad4739237c13c/c4x/KPI/Algorithms101/asset/input_1000_100.txt";
// Instantiate parameterized tests with different test cases.
INSTANTIATE_TEST_SUITE_P(
    Test100FilmsSequences, SplitInversionsCountingTest,
    ::testing::Values(
        TestParams{sdsnew("input_1000_100.txt"), 618, 1, 2368},
        TestParams{sdsnew("input_1000_100.txt"), 951, 178, 2483}
    )
);*/
/*
// Fixture class for parameterized tests.
class SplitInversionsCountingTest : public ::testing::TestWithParam<TestParams> {};

// Parameterized test for counting split inversions.
TEST_P(SplitInversionsCountingTest, CountInversions) {
    const auto& params = GetParam();
    char* test_data_source = "https://courses.prometheus.org.ua/assets/courseware/v1/9a95f1a8992d060eafde59c96919dede/c4x/KPI/Algorithms101/asset/input_1000_5.txt";
    
    int users, films;
    int **users_films_ratings;
    
    parse_inversions_data(test_data_source, &users_films_ratings, &users, &films);        

     ---------------------------
    // Retrieve test samples and filter by `testKey` and `userIndex`
    auto& testSamples = FiveFilmsTestSamples[params.testKey];
    
    // Extract `targetRate` for the `userIndex`
    auto targetRateIt = std::find_if(
        testSamples.begin(), testSamples.end(),
        [userIndex = params.userIndex](const RateSample& item) { return item.Number == userIndex; });
    
    ASSERT_NE(targetRateIt, testSamples.end()) << "User index not found in test samples.";
    auto targetRate = targetRateIt->Rate;

    // Extract `usersRates` for users other than `userIndex`
    std::unordered_map<int, std::vector<int>> usersRates;
    for (const auto& sample : testSamples) {
        if (sample.Number != params.userIndex) {
            usersRates[sample.Number] = sample.Rate;
        }
    }

    // Retrieve `relativeUserRate`
    auto relativeUserRate = usersRates[params.relativeUserIndex];
    
    // Prepare `preparedUserRate`
    std::vector<int> preparedUserRate;
    for (size_t i = 0; i < relativeUserRate.size(); ++i) {
        preparedUserRate.push_back(relativeUserRate[i] - targetRate[i]);
    }

    // Calculate the number of inversions
    int calculatedUserInversions = SortIntAndCountInversions(preparedUserRate);

    // Expectation
    EXPECT_EQ(calculatedUserInversions, params.expectedUserInversions)
        << "Sample \"" << params.testKey << "\"\n"
        << "User " << params.userIndex << "\n"
        << "Expected split inversions count " << params.expectedUserInversions << "\n"
        << "Actual split inversions count " << calculatedUserInversions;
        ----------------------------------------
    int base_user_idx = params.userIndex - 1;
    int cmp_user_idx = params.comparUserIndex - 1;

    for (int i = 0; i < users; i++){
      if (i == base_user_idx){
        sort_with_corresponding(users_films_ratings, users, films, base_user_idx);
        int calculatedUserInversions = count_inversions(users_films_ratings[cmp_user_idx], films, 0);
        EXPECT_EQ(params.expectedUserInversions, calculatedUserInversions)
            << "Sample \"" << params.testKey << "\"\n"
            << "User " << params.userIndex << "\n"
            << "Expected split inversions count " << params.expectedUserInversions << "\n"
            << "Actual split inversions count " << calculatedUserInversions;
      }
    }
}

// Instantiate parameterized tests with different test cases.
INSTANTIATE_TEST_SUITE_P(
    TestFiveFilmsSequences, SplitInversionsCountingTest,
    ::testing::Values(
        TestParams{sdsnew("input_1000_5.txt"), 452, 100, 7},
        TestParams{sdsnew("input_1000_5.txt"), 863, 29, 0}
    )
);*/
