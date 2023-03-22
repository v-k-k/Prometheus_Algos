#include <stdio.h>
#include "modules/optimized_math/karatsuba.h"
#include "helpers/http_helper/http_helper.h"
#include "helpers/simple_dynamic_string/sds.h"

int main(int argc, char *argv[])
{
    char* x = "1685287499328328297814655639278583667919355849391453456921116729";
    char* y = "7114192848577754587969744626558571536728983167954552999895348492";
    char* r = multiply(x, y);
    //printf("%s", r);

    char* url = "http://courses.prometheus.org.ua/assets/courseware/v1/b562f0c2309e30de66682f4fa0b447ef/c4x/KPI/Algorithms101/asset/data_examples_02.zip";

    struct f_list* file_list = { sdsempty(), sdsempty(), NULL, 0 };
    
    get_zipped_files(&file_list, url);// void main3(struct f_list** file_list, char* source)

    //struct f_list* last_dummy_file = list_last(file_list);
    int64_t entries = file_list->entries;
    for(int64_t idx = 0; idx < entries; ++idx){
        printf("%s\n%s\n", file_list->name, file_list->content);
        sdsfree(file_list->name);
        sdsfree(file_list->content);
        file_list = file_list->next;
    }

    return 0;
}