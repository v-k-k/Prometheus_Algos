//#include <curl/curl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include "zip_helper.h"
#include "../simple_dynamic_string/sds.h"

 
struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

struct f_list* node_create(sds name, sds content) {
    struct f_list* a = (struct f_list*)malloc(sizeof(struct f_list));
    a->name = name;
    a->content = content;
    a->next = NULL;
    return a;
}

void list_add_front(struct f_list** old, sds name, sds content) {
    struct f_list* node = node_create(name, content);
    node->next = *old;
    (*old) = node;
}

size_t list_length(const struct f_list* list) {
    size_t len = 0;
    struct f_list* currentPtr = (struct f_list*)list;
    if (currentPtr != NULL) {
        len++;
        while (currentPtr->next != NULL) {
            len++;
            currentPtr = currentPtr->next;
        }
    }
    return len;
}

void list_destroy(struct f_list* list) {
    struct f_list* currentPtr = (struct f_list*)list;
    struct f_list* next;
    while (currentPtr != NULL) {
        next = currentPtr->next;
        free(currentPtr);
        currentPtr = next;
    }
    list = NULL;
}

struct f_list* list_last(struct f_list* list) {
    struct f_list* currentPtr = (struct f_list*)list;
    if (currentPtr != NULL) {
        while (currentPtr->next != NULL) {
            currentPtr = currentPtr->next;
            //currentPtr->show_content(currentPtr);
        }
    }
    return currentPtr;
}

void list_add_back(struct f_list** old, char* name, char* content) {
    if (*old == NULL)
        list_add_front(old, name, content);
    else {
        struct f_list* node = node_create(name, content);
        struct f_list* lst = list_last(*old);
        lst->next = node;
    }
}

void show(struct f_list* head) {
    struct f_list* current = head;

    while (current != NULL) {
        fprintf(stderr, "Name: %s\n", current->name);
        fprintf(stderr, "Content: %s\n", current->content);
        fprintf(stderr, "\n\t");

        current = current->next;
    }
}
 
void get_zipped_files(struct f_list** file_list, char* source) {
    zip_t *zip = zip_open(source, ZIP_RDONLY, NULL);
    if (!zip) {
        fprintf(stderr, "Failed to open ZIP file: %s\n", source);
        return;
    }

    int num_entries = zip_get_num_entries(zip, 0);
    for (int i = 0; i < num_entries; i++) {
        struct zip_stat stat;
        if (zip_stat_index(zip, i, 0, &stat) == 0) {
            const char* file_name = stat.name;
            struct zip_file* zf = zip_fopen(zip, file_name, ZIP_FL_UNCHANGED);
            if (!zf) {
                fprintf(stderr, "Failed to open ZIP file entry: %s\n", file_name);
                continue;
            }

            // Read the file contents
            char buffer[1024];
            size_t bytes_read;
            sds content = sdsempty();
            while ((bytes_read = zip_fread(zf, buffer, sizeof(buffer))) > 0) {
                content = sdscatlen(content, buffer, bytes_read);
            }

            // Add the file to the linked list
            list_add_back(file_list, strdup(file_name), content);

            zip_fclose(zf);
        }
    }
    zip_close(zip);
}