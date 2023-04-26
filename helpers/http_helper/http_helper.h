#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include <zip.h>
#include <stdio.h>
#include "../simple_dynamic_string/sds.h"

struct f_list {
    sds name;
    sds content;
    struct f_list* next;
};

void get_zipped_files(struct f_list**, char*);

size_t list_length(const struct f_list*);

/// @brief ///
//void test_me(struct f_list**, char*);

#endif