// #include <curl/curl.h>
// #include <inttypes.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <zip.h>
// #include "http_helper.h"
// #include "../simple_dynamic_string/sds.h"


// struct MemoryStruct {
//   char *memory;
//   size_t size;
// };

// struct f_list* node_create(sds name, sds content, zip_int64_t entries) {
//     struct f_list* a = (struct f_list*)malloc(sizeof(struct f_list));
//     a->name = name;
//     a->content = content;
//     a->entries = entries;
//     return a;
// }

// void list_add_front(struct f_list** old, sds name, sds content, zip_int64_t entries) {
//     struct f_list* node = node_create(name, content, entries);
//     node->next = *old;
//     (*old) = node;
// }

// size_t list_length(const struct f_list* list) {
//     size_t len = 0;
//     struct f_list* currentPtr = (struct f_list*)list;
//     if (currentPtr != NULL) {
//         len++;
//         while (currentPtr->next != NULL) {
//             len++;
//             currentPtr = currentPtr->next;
//         }
//     }
//     return len;
// }

// void list_destroy(struct f_list* list) {
//     struct f_list* currentPtr = (struct f_list*)list;
//     struct f_list* next;
//     while (currentPtr != NULL) {
//         next = currentPtr->next;
//         free(currentPtr);
//         currentPtr = next;
//     }
//     list = NULL;
// }

// struct f_list* list_last(struct f_list* list) {
//     struct f_list* currentPtr = (struct f_list*)list;
//     if (currentPtr != NULL) {
//         while (currentPtr->next != NULL) {
//             currentPtr = currentPtr->next;
//         }
//     }
//     return currentPtr;
// }

// void list_add_back(struct f_list** old, char* name, char* content, zip_int64_t entries) {
//     if ((*old) == NULL)
//         list_add_front(old, name, content, entries);
//     else {
//         struct f_list* node = node_create(name, content, entries);
//         struct f_list* lst = list_last(*old);
//         lst->next = node;
//     }
// }

// static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
//   size_t realsize = size * nmemb;
//   struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
//   char *ptr = realloc(mem->memory, mem->size + realsize + 1);
//   if(!ptr) {
//     /* out of memory! */
//     printf("not enough memory (realloc returned NULL)\n");
//     return 0;
//   }
 
//   mem->memory = ptr;
//   memcpy(&(mem->memory[mem->size]), contents, realsize);
//   mem->size += realsize;
//   mem->memory[mem->size] = 0;
 
//   return realsize;
// }

// void test_me(struct f_list** file_list, char* source)
// {
//     CURL *curl;
//     CURLcode res;

//     struct MemoryStruct chunk;
 
//     chunk.memory = malloc(1);  
//     chunk.size = 0;    
    
//     curl_global_init(CURL_GLOBAL_ALL);
//     curl = curl_easy_init();

//     curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
// 	curl_easy_setopt(curl, CURLOPT_URL, source);
// 	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
// 	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
// 	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip, deflate");
// 	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
// 	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

// 	struct curl_slist *headers = NULL;
// 	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
// 	res = curl_easy_perform(curl);
		
//     if(res != CURLE_OK) {
//             fprintf(stderr, "curl_easy_perform() failed: %s\n",
//                     curl_easy_strerror(res));
//     }
//     else {
//         zip_error_t ze; 
//         zip_source_t* zs = zip_source_buffer_create(chunk.memory, chunk.size, 0, &ze);

//         zip_t* zip = zip_open_from_source(zs, ZIP_CHECKCONS | ZIP_RDONLY, &ze);

//         zip_int64_t entries = zip_get_num_entries(zip, 0);
//         (*file_list)->entries = entries;

//         zip_error_t* nnnn  = zip_get_error(zip);

//         for(zip_int64_t idx = 0; idx < entries; ++idx) {

//             printf("File number %d, filename is %s \n", idx, zip_get_name(zip, idx, ZIP_FL_ENC_STRICT));
            
//             zip_file_t* fp = zip_fopen_index(zip, idx, 0);
            
//             zip_int64_t len;
//             char buf[1024];
//             while((len = zip_fread(fp, buf, sizeof buf)) > 0) {
//                 printf("read %d bytes\n", len);
//             }
//             zip_fclose(fp);
//         }
//         zip_close(zip);
//     }

//     curl_easy_cleanup(curl);
//     free(chunk.memory);
//     curl_global_cleanup();

//     printf("\nTEST IS OVER - DELETE ME!!!\n");
// }

