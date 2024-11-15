#ifndef STRUCTS_H
#define STRUCTS_H

#include "simple_dynamic_string/sds.h"

struct f_list {
    sds name;
    sds content;
    struct f_list* next;

    void (*show_content)(struct f_list* head);
};
 
struct MemoryStruct {
  char *memory;
  size_t size;
};

// Structure to store elements of the first array along with their original indices
typedef struct {
    int value;
    int index;
} Element;

typedef struct {
    int user;
    int *inversions;
} ExpextedUserInversions;

// Parameter structure for `TestFiveFilmsSequences`.
struct TestParams {
    sds testKey;
    int userIndex;
    int comparUserIndex;
    int expectedUserInversions;
};

#endif