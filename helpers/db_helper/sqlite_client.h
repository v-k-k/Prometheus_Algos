#ifndef SQLITE_CLIENT_H
#define SQLITE_CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../structs.h"
#include "../tools/utils.h"

#define DB_CONNECTION_STRING "TestDataDb.db"
#define SQL_FILE "../helpers/db_helper/TestDataDb.sql"

// Helper function to initialize an IntStringListDictionary
IntStringListDictionary* create_int_string_list_dictionary();

// Helper function to add a key-value pair to an IntStringListDictionary
void add_int_string_list_pair(IntStringListDictionary*, int, char**, int);

// Helper function to free the memory of an IntStringListDictionary
void free_int_string_list_dictionary(IntStringListDictionary*);

// Helper function to initialize a StringIntDictionary
StringIntDictionary* create_string_int_dictionary();

// Helper function to add a key-value pair to a StringIntDictionary
void add_string_int_pair(StringIntDictionary*, char*, int);

// Helper function to free the memory of a StringIntDictionary
void free_string_int_dictionary(StringIntDictionary*);

// Helper function to free the memory of a StringArrayTuple
void free_string_array_tuple(StringArrayTuple*);

// Mimics the DoQuery function
IntStringListDictionary* do_query(const char*, int);

// Mimics GenerateTestDataDb function
void generate_test_data_db();

// Mimics GetLettersRate function
StringIntDictionary* get_letters_rate();

// Mimics GetStringsCollections function
StringArrayTuple* get_strings_collections();

// Mimics GetResultPassword function
char* get_result_password();

#endif