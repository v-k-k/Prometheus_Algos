#ifndef INVERSIONS_H
#define INVERSIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../http/http.h"


static int parse_inversions_data(const char *source_link, int ***arrays, int *users, int *films) {
    sds content = retrievePlainText(source_link);

    sds *tokens, *real_tokens;
    int count, j, real_t_count = 0;

    tokens = sdssplitlen(content, sdslen(content), "\n", 1, &count);
    real_tokens = (sds *)malloc(count * sizeof(sds));
    if (real_tokens == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        sdsfreesplitres(tokens, count);
        return 1;
    }

    // Filter out empty tokens
    for (j = 0; j < count; j++) {
        if (sdslen(tokens[j]) > 0) 
            real_tokens[real_t_count++] = sdsnewlen(tokens[j], sdslen(tokens[j])); 
    }

    // Split the sds string into an integer array
    int *int_row = NULL;
    int row_size, MOVE_TO_LINE, PARSING_LINE = 0;

    sds_to_int_array(real_tokens[PARSING_LINE], &int_row, &row_size);
    *users = int_row[0];
    *films = int_row[1];
    free(int_row); // Free the temporary int_row array
    
   // Allocate memory for the main array
    *arrays = (int**)malloc(*users * sizeof(int*));

    // Populate the main array with pointers to inner arrays
    MOVE_TO_LINE = *users + 1;
    for (PARSING_LINE++; PARSING_LINE < MOVE_TO_LINE; PARSING_LINE++) {
        int_row = NULL;
        row_size = 0;
        sds_to_int_array(real_tokens[PARSING_LINE], &int_row, &row_size);

        // Allocate memory for the inner array
        (*arrays)[PARSING_LINE - 1] = (int*)malloc(row_size * sizeof(int));

        // Copy elements to the inner array
        for (int i = 0; i < row_size; i++) {
            if (i + 1 >= row_size) break;
                
            (*arrays)[PARSING_LINE - 1][i] = int_row[i + 1];
        }

        free(int_row); // Free the temporary int_row array
    }

    sdsfreesplitres(real_tokens, real_t_count);
    sdsfreesplitres(tokens, count);    
    sdsfree(content);

    return 0; // Indicate success
}

#endif