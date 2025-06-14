#include "hash_table.h"


// Hash function that handles negative keys correctly
int hash(long long int key) {
    return (int)(((key % TABLE_SIZE) + TABLE_SIZE) % TABLE_SIZE);
}

// Linear probing to find the next available slot
int probe(long long int H[], long long int key) {
    int index = hash(key);
    int i = 0;

    while (H[(index + i) % TABLE_SIZE] != 0) {
        i++;
        if (i >= TABLE_SIZE) {
            // Table is full
            fprintf(stderr, "Error: Hash table full or cannot find empty slot for key %lld\n", key);
            return -1;
        }
    }
    return (index + i) % TABLE_SIZE;
}

// Insert a key into the hash table
void Insert(long long int H[], long long int key) {
    int index = hash(key);

    if (H[index] != 0) {
        index = probe(H, key);
        if (index == -1)
            return; // Failed to insert
    }
    H[index] = key;
}

// Search for a key in the hash table
int HashSearch(long long int H[], long long int key) {
    int index = hash(key);
    int i = 0;

    while (i < TABLE_SIZE) {
        int probe_index = (index + i) % TABLE_SIZE;

        if (H[probe_index] == 0)
            return -1;  // Empty slot found — key not in table

        if (H[probe_index] == key)
            return probe_index;  // Key found

        i++;
    }
    return -1;  // Full loop, key not found
}
