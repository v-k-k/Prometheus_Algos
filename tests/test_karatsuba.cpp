#include <gtest/gtest.h>

extern "C"
{
#include "../modules/optimized_math/karatsuba.h"
}

TEST(test_karatsuba, optimized_math)
{
    char* x = "1685287499328328297814655639278583667919355849391453456921116729";
    char* y = "7114192848577754587969744626558571536728983167954552999895348492";
    char* expected_result = "11989460275519080564894036768322865785999566885539505969749975204962718118914971586072960191064507745920086993438529097266122668";
    
    EXPECT_STREQ(expected_result, multiply(x, y));
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