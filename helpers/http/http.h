#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include "../tools/utils.h"

//sds retrievePlainText(char* url);
static sds retrievePlainText(const char* url){
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
      struct curl_slist *headers = NULL;
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    sds result = sdsnew(chunk.memory);
    free(chunk.memory); // Free the memory allocated for chunk

    return result;
}

#endif