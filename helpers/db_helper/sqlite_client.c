#include "sqlite_client.h"

// Helper function to initialize an IntStringListDictionary
IntStringListDictionary* create_int_string_list_dictionary() {
    IntStringListDictionary *dict = (IntStringListDictionary*)malloc(sizeof(IntStringListDictionary));
    if (!dict) {
        perror("Failed to allocate IntStringListDictionary");
        return NULL;
    }
    dict->pairs = NULL;
    dict->capacity = 0;
    dict->count = 0;
    return dict;
}

// Helper function to add a key-value pair to an IntStringListDictionary
void add_int_string_list_pair(IntStringListDictionary *dict, int key, char **values, int value_count) {
    // Simple resizing (reallocate more space if needed)
    if (dict->count >= dict->capacity) {
        dict->capacity = (dict->capacity == 0) ? 4 : dict->capacity * 2;
        IntStringListPair *temp = (IntStringListPair*)realloc(dict->pairs, dict->capacity * sizeof(IntStringListPair));
        if (!temp) {
            perror("Failed to reallocate IntStringListPair array");
            // Handle error appropriately (e.g., free existing memory)
            return;
        }
        dict->pairs = temp;
    }

    dict->pairs[dict->count].key = key;
    dict->pairs[dict->count].values = (char**)malloc(value_count * sizeof(char*));
    if (!dict->pairs[dict->count].values) {
        perror("Failed to allocate values array");
        // Handle error
        return;
    }
    dict->pairs[dict->count].value_count = value_count;
    for (int i = 0; i < value_count; ++i) {
        dict->pairs[dict->count].values[i] = strdup(values[i]);
        if (!dict->pairs[dict->count].values[i]) {
            perror("Failed to duplicate string");
            // Handle error (free previously allocated strings)
            return;
        }
    }
    dict->count++;
}

// Helper function to free the memory of an IntStringListDictionary
void free_int_string_list_dictionary(IntStringListDictionary *dict) {
    if (dict) {
        for (int i = 0; i < dict->count; ++i) {
            for (int j = 0; j < dict->pairs[i].value_count; ++j) {
                free(dict->pairs[i].values[j]);
            }
            free(dict->pairs[i].values);
        }
        free(dict->pairs);
        free(dict);
    }
}

// Helper function to initialize a StringIntDictionary
StringIntDictionary* create_string_int_dictionary() {
    StringIntDictionary *dict = (StringIntDictionary*)malloc(sizeof(StringIntDictionary));
    if (!dict) {
        perror("Failed to allocate StringIntDictionary");
        return NULL;
    }
    dict->pairs = NULL;
    dict->capacity = 0;
    dict->count = 0;
    return dict;
}

// Helper function to add a key-value pair to a StringIntDictionary
void add_string_int_pair(StringIntDictionary *dict, char *key, int value) {
    if (dict->count >= dict->capacity) {
        dict->capacity = (dict->capacity == 0) ? 4 : dict->capacity * 2;
        StringIntPair *temp = (StringIntPair*)realloc(dict->pairs, dict->capacity * sizeof(StringIntPair));
        if (!temp) {
            perror("Failed to reallocate StringIntPair array");
            return;
        }
        dict->pairs = temp;
    }
    dict->pairs[dict->count].key = strdup(key);
    if (!dict->pairs[dict->count].key) {
        perror("Failed to duplicate string");
        return;
    }
    dict->pairs[dict->count].value = value;
    dict->count++;
}

// Helper function to free the memory of a StringIntDictionary
void free_string_int_dictionary(StringIntDictionary *dict) {
    if (dict) {
        for (int i = 0; i < dict->count; ++i) {
            free(dict->pairs[i].key);
        }
        free(dict->pairs);
        free(dict);
    }
}

// Helper function to free the memory of a StringArrayTuple
void free_string_array_tuple(StringArrayTuple *tuple) {
    if (tuple) {
        if (tuple->array1) {
            for (int i = 0; i < tuple->array1_count; ++i) {
                free(tuple->array1[i]);
            }
            free(tuple->array1);
        }
        if (tuple->array2) {
            for (int i = 0; i < tuple->array2_count; ++i) {
                free(tuple->array2[i]);
            }
            free(tuple->array2);
        }
        free(tuple);
    }
}

// Mimics the DoQuery function
IntStringListDictionary* do_query(const char *query, int max_column_idx) {
    sqlite3 *db;
    IntStringListDictionary *result = create_int_string_list_dictionary();
    int rc = sqlite3_open(DB_CONNECTION_STRING, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free_int_string_list_dictionary(result);
        return NULL;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        free_int_string_list_dictionary(result);
        return NULL;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int start_idx = 0;
        int key = sqlite3_column_int(stmt, start_idx++);
        int column_count = sqlite3_column_count(stmt);
        int value_count = 0;
        char **values_temp = NULL;

        if (start_idx < column_count) {
            value_count++;
            values_temp = (char**)realloc(values_temp, value_count * sizeof(char*));
            if (!values_temp) {
                perror("Failed to allocate values_temp");
                // Handle error (free resources)
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                free_int_string_list_dictionary(result);
                return NULL;
            }
            const unsigned char *text = sqlite3_column_text(stmt, start_idx++);
            values_temp[value_count - 1] = text ? strdup((const char*)text) : NULL;
            if (text && !values_temp[value_count - 1]) {
                perror("Failed to duplicate string");
                // Handle error
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                free_int_string_list_dictionary(result);
                return NULL;
            }

            while (start_idx <= max_column_idx && start_idx < column_count) {
                value_count++;
                values_temp = (char**)realloc(values_temp, value_count * sizeof(char*));
                if (!values_temp) {
                    perror("Failed to reallocate values_temp");
                    // Handle error
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    free_int_string_list_dictionary(result);
                    return NULL;
                }
                const unsigned char *text = sqlite3_column_text(stmt, start_idx++);
                values_temp[value_count - 1] = text ? strdup((const char*)text) : NULL;
                if (text && !values_temp[value_count - 1]) {
                    perror("Failed to duplicate string");
                    // Handle error
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    free_int_string_list_dictionary(result);
                    return NULL;
                }
            }
        }
        add_int_string_list_pair(result, key, values_temp, value_count);
        if (values_temp) {
            for (int i = 0; i < value_count; ++i) {
                free(values_temp[i]);
            }
            free(values_temp);
        }
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error during step: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return result;
}

// Helper function to read the SQL file
char* read_sql_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening SQL file");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *sql_content = (char*)malloc(file_size + 1);
    if (sql_content == NULL) {
        perror("Failed to allocate memory for SQL content");
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(sql_content, 1, file_size, fp);
    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Error reading SQL file.\n");
        free(sql_content);
        fclose(fp);
        return NULL;
    }
    sql_content[bytes_read] = '\0';

    fclose(fp);
    return sql_content;
}

// Mimics GenerateTestDataDb function
void generate_test_data_db() {
    sqlite3 *db;
    int rc = sqlite3_open(DB_CONNECTION_STRING, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char *sql_content = read_sql_file(SQL_FILE);
    if (sql_content == NULL) {
        sqlite3_close(db);
        return;
    }

    char *errmsg = 0;
    rc = sqlite3_exec(db, sql_content, NULL, NULL, &errmsg);
    free(sql_content);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
    }

    sqlite3_close(db);
}

// Mimics GetLettersRate function
StringIntDictionary* get_letters_rate() {
    const char *query = "SELECT * FROM Letters;";
    IntStringListDictionary *query_result = do_query(query, 2);
    StringIntDictionary *result = create_string_int_dictionary();

    if (query_result) {
        for (int i = 0; i < query_result->count; ++i) {
            if (query_result->pairs[i].value_count >= 2 && query_result->pairs[i].values[0] && query_result->pairs[i].values[1]) {
                add_string_int_pair(result, query_result->pairs[i].values[0], atoi(query_result->pairs[i].values[1]));
            }
        }
        free_int_string_list_dictionary(query_result);
    }
    return result;
}

// Mimics GetStringsCollections function
StringArrayTuple* get_strings_collections() {
    const char *query = "SELECT * FROM Strings;";
    IntStringListDictionary *query_result = do_query(query, 2);
    StringArrayTuple *result = (StringArrayTuple*)malloc(sizeof(StringArrayTuple));
    if (!result) {
        perror("Failed to allocate StringArrayTuple");
        return NULL;
    }
    result->array1 = NULL;
    result->array1_count = 0;
    result->array2 = NULL;
    result->array2_count = 0;

    if (query_result) {
        result->array1 = (char**)malloc(query_result->count * sizeof(char*));
        result->array2 = (char**)malloc(query_result->count * sizeof(char*));
        if (!result->array1 || !result->array2) {
            perror("Failed to allocate string arrays");
            free(result->array1);
            free(result->array2);
            free(result);
            free_int_string_list_dictionary(query_result);
            return NULL;
        }

        for (int i = 0; i < query_result->count; ++i) {
            if (query_result->pairs[i].value_count >= 2 && query_result->pairs[i].values[0] && query_result->pairs[i].values[1]) {
                result->array1[result->array1_count++] = strdup(query_result->pairs[i].values[0]);
                result->array2[result->array2_count++] = strdup(query_result->pairs[i].values[1]);
            }
        }
        free_int_string_list_dictionary(query_result);
    }
    return result;
}

// Mimics GetResultPassword function
char* get_result_password() {
    const char *query = "SELECT * FROM ResultPassword;";
    IntStringListDictionary *query_result = do_query(query, 1);
    char *password = NULL;

    if (query_result && query_result->count > 0 && query_result->pairs[0].value_count >= 1 && query_result->pairs[0].values[0]) {
        // Assuming the password is in the first row and first column
        // The C# code accesses index 1, which might be an error or based on Id.
        // Let's adjust to get the first row's first string value.
        for (int i = 0; i < query_result->count; ++i) {
            if (query_result->pairs[i].key == 1 && query_result->pairs[i].value_count >= 1 && query_result->pairs[i].values[0]) {
                password = strdup(query_result->pairs[i].values[0]);
                break;
            } else if (query_result->count == 1 && query_result->pairs[0].value_count >= 1 && query_result->pairs[0].values[0]) {
                password = strdup(query_result->pairs[0].values[0]);
                break;
            }
        }
        free_int_string_list_dictionary(query_result);
    }
    return password;
}

