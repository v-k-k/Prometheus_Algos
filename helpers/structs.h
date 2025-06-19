#ifndef STRUCTS_H
#define STRUCTS_H

#include "simple_dynamic_string/sds.h"

 // Definition of your BTreeNode struct
struct BTreeNode {
    struct BTreeNode *lchild;
    int data;
    struct BTreeNode *rchild;
};

// DECLARATION of the global root pointer
// 'extern' tells the compiler that 'root' exists somewhere else,
// and its definition will be provided by another compilation unit (bst.c file).
extern struct BTreeNode *root;

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

// Structure to represent a key-value pair (mimicking Dictionary<int, List<string>>)
typedef struct {
    int key;
    char **values;
    int value_count;
} IntStringListPair;

// Structure to represent a dictionary (mimicking Dictionary<int, List<string>>)
typedef struct {
    IntStringListPair *pairs;
    int capacity;
    int count;
} IntStringListDictionary;

// Structure to represent a string-integer pair (mimicking Dictionary<string, int>)
typedef struct {
    char *key;
    int value;
} StringIntPair;

// Structure to represent a dictionary (mimicking Dictionary<string, int>)
typedef struct {
    StringIntPair *pairs;
    int capacity;
    int count;
} StringIntDictionary;

// Structure to represent a tuple of two string arrays (mimicking Tuple<string[], string[]>)
typedef struct {
    char **array1;
    int array1_count;
    char **array2;
    int array2_count;
} StringArrayTuple;

typedef struct {
    int* array;
    size_t size;
    size_t capacity;
} IntArray;

typedef struct {
    IntArray Hlow; 
    IntArray Hhigh; 
} Heap;

#endif