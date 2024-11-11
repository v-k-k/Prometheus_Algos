#ifndef ZIP_HELPER_H
#define ZIP_HELPER_H

#include <zip.h>
#include <stdio.h>
#include "../simple_dynamic_string/sds.h"
#include "../structs.h"

void show(struct f_list* head);

void get_zipped_files(struct f_list**, char*);

size_t list_length(const struct f_list*);

/// @brief ///
//void test_me(struct f_list**, char*);

#endif