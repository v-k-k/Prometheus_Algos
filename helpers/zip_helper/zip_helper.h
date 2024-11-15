#ifndef ZIP_HELPER_H
#define ZIP_HELPER_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include "../payload_extractors/inversions.h"

void show(struct f_list* head);

void get_zipped_files(struct f_list**, const char*);

size_t list_length(const struct f_list*);

#endif