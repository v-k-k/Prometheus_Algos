#include <stdio.h>
#include "modules/optimized_math/karatsuba.h"

int main(int argc, char *argv[])
{
    char* x = "1685287499328328297814655639278583667919355849391453456921116729";
    char* y = "7114192848577754587969744626558571536728983167954552999895348492";
    char* r = multiply(x, y);
    printf("%s", r);

    return 0;
}