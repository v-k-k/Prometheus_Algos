#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "../../helpers/tools/utils.h"

// Define the desired capacity.
// For 1,000,000 keys, a significantly larger table is needed.
// A common heuristic is to make the hash table size a prime number
// larger than the expected number of elements to minimize collisions.
// Let's aim for a load factor of around 0.7 to 0.8 for open addressing,
// so a size of 1,300,000 to 1,500,000 would be a good starting point.
// For simplicity and demonstration, let's use a slightly larger power of 2
// or a prime number close to 1,300,000.
// Let's choose a prime number for better distribution.
#define TABLE_SIZE 1300003 // A prime number larger than 1,000,000

// Insert function
void Insert(long long int H[], long long int key);

// Search function
int HashSearch(long long int H[], long long int key);

#endif