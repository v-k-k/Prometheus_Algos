#ifndef PAYLOAD_EXT_H
#define PAYLOAD_EXT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include "../simple_dynamic_string/sds.h"

void prepare_inversions_samples(sds fname, sds content);

#endif