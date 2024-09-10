#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "modules/optimized_math/karatsuba.h"
#include "helpers/zip_helper/zip_helper.h"
#include "helpers/simple_dynamic_string/sds.h"
#include "helpers/payload_extractors/payload_extractors.h"


int main(int argc, char *argv[])
{
    char* x = "1685287499328328297814655639278583667919355849391453456921116729";
    char* y = "7114192848577754587969744626558571536728983167954552999895348492";
    char* r = multiply(x, y);
    printf("%s", r);

    char* path = "../data/data_examples_02.zip";

    struct f_list* file_list = NULL;
    
    get_zipped_files(&file_list, path);
    
    size_t entries_count = list_length(file_list);

    for(int64_t idx = 0; idx < entries_count; ++idx){
      
      printf("%s\n\n", file_list->name);

      prepare_inversions_samples(file_list->content);

      // TODO: create user data struct and save particular inversion data

        // printf("%s\n%s\n", file_list->name, file_list->content);
        // sdsfree(file_list->name);
        // sdsfree(file_list->content);
        // file_list = file_list->next;
    }

    //test_me(&file_list, url);

    return 0;
}
