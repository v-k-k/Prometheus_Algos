#include <stdio.h>
#include "modules/optimized_math/karatsuba.h"

int main(int argc, char *argv[])
{
    int arr[] = {1,5,4,6,7,9,8,10,2};
    size_t n = sizeof(arr)/sizeof(arr[0]);
    float mean = calculate_mean(arr, n);
    printf("Mean=%.2f\n", mean);

    char* x = "1685287499328328297814655639278583667919355849391453456921116729";
    char* y = "7114192848577754587969744626558571536728983167954552999895348492";
    multiply(x, y);//char* r = 

    return 0;
}