#include <curl/curl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include "http_helper.h"
#include "../simple_dynamic_string/sds.h"

 
struct MemoryStruct {
  char *memory;
  size_t size;
};

// struct f_list {
//     sds name;
//     sds content;
//     struct f_list* next;
//     zip_int64_t entries;
// };

struct f_list* node_create(sds name, sds content, zip_int64_t entries) {
    struct f_list* a = (struct f_list*)malloc(sizeof(struct f_list));
    a->name = name;
    a->content = content;
    a->entries = entries;
    return a;
}

void list_add_front(struct f_list** old, sds name, sds content, zip_int64_t entries) {
    struct f_list* node = node_create(name, content, entries);
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
        }
    }
    return currentPtr;
}

void list_add_back(struct f_list** old, char* name, char* content, zip_int64_t entries) {
    if ((*old) == NULL)
        list_add_front(old, name, content, entries);
    else {
        struct f_list* node = node_create(name, content, entries);
        struct f_list* lst = list_last(*old);
        lst->next = node;
    }
}

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


/*int main2(void)
{
  CURLcode rc;
  CURL *curl;

  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "https://courses.prometheus.org.ua/assets/courseware/v1/b562f0c2309e30de66682f4fa0b447ef/c4x/KPI/Algorithms101/asset/data_examples_02.zip");
  curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
  curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  rc = curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  return (int) rc;
}*/
 

void get_zipped_files(struct f_list** file_list, char* source){
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, source);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip, deflate");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);

        /* check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        else {
            zip_error_t ze; // for errors

            // create a zip_source from the data you've stored in memory
            zip_source_t* zs = zip_source_buffer_create(chunk.memory, chunk.size, 0, &ze);

            // open the archive from the zip_source
            zip_t* zip = zip_open_from_source(zs, ZIP_CHECKCONS | ZIP_RDONLY, &ze);

            // read how many files you've got in there
            zip_int64_t entries = zip_get_num_entries(zip, 0);

            //struct f_list* pfile_list = { NULL };     // &&&&&&&&&&
            //struct f_list** file_list = &pfile_list;  // &&&&&&&&&&
            (*file_list)->entries = entries;

            // loop over the entries in the archive
            for(zip_int64_t idx = 0; idx < entries; ++idx) {
                char* fname = zip_get_name(zip, idx, ZIP_FL_ENC_STRICT);

                // open the file at this index
                zip_file_t* fp = zip_fopen_index(zip, idx, 0);

                // process the file
                int64_t SIZE = 1024; // !!!!

                zip_int64_t len;
                char buf[SIZE];

                sds content = sdsempty();
                
                while((len = zip_fread(fp, buf, sizeof buf)) > 0) {
                    buf[len] = '\0';
                    content = sdscat(content, buf);
                }
                
                list_add_front(file_list, sdsnew(fname), content, entries);
                //sdsclear(content);
                zip_fclose(fp); // close this file
            }
            //zip_close(zip); // close the whole archive

            //struct f_list* file_list = { NULL };
        
            /*printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            printf("\n\n%" PRId64 "\n", entries);//chunk.memory);
            printf("\n\nFUCK");*/
        }
    }

    /* we are done with libcurl, so clean it up */
    curl_easy_cleanup(curl);
    free(chunk.memory);
    curl_global_cleanup();
}
