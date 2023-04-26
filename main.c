#include <stdio.h>
#include "modules/optimized_math/karatsuba.h"
#include "helpers/http_helper/http_helper.h"
#include "helpers/simple_dynamic_string/sds.h"

int main(int argc, char *argv[])
{
    char* x = "1685287499328328297814655639278583667919355849391453456921116729";
    char* y = "7114192848577754587969744626558571536728983167954552999895348492";
    char* r = multiply(x, y);
    //printf("%s", r);

    char* url = "http://courses.prometheus.org.ua/assets/courseware/v1/b562f0c2309e30de66682f4fa0b447ef/c4x/KPI/Algorithms101/asset/data_examples_02.zip";

    struct f_list* file_list = { sdsempty(), sdsempty(), NULL, 0 };
    
    get_zipped_files(&file_list, url);
    
    size_t entries_count = list_length(file_list);

    struct f_list* last_dummy_file = list_last(file_list);
    //int64_t entries = file_list->entries;
    for(int64_t idx = 0; idx < entries_count; ++idx){
        printf("%s\n%s\n", file_list->name, file_list->content);
        sdsfree(file_list->name);
        sdsfree(file_list->content);
        file_list = file_list->next;
    }

    //test_me(&file_list, url);

    return 0;
}
/*
#include <curl/curl.h>
#include <stdio.h>
#include <zip.h>


struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

int main(int argc, char *argv[])
{
    char* url = "http://courses.prometheus.org.ua/assets/courseware/v1/b562f0c2309e30de66682f4fa0b447ef/c4x/KPI/Algorithms101/asset/data_examples_02.zip";

    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);  
    chunk.size = 0;    
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "br, gzip, deflate");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	struct curl_slist *headers = NULL;
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	res = curl_easy_perform(curl);
		
    zip_error_t ze; // for errors

    // create a zip_source from the data you've stored in memory
    zip_source_t* zs = zip_source_buffer_create(chunk.memory, chunk.size, 0, &ze);

    // open the archive from the zip_source
    zip_t* zip = zip_open_from_source(zs, ZIP_CHECKCONS | ZIP_RDONLY, &ze);

    // read how many files you've got in there
    zip_int64_t entries = zip_get_num_entries(zip, 0);

    //struct f_list* pfile_list = { NULL };     // &&&&&&&&&&
    //struct f_list** file_list = &pfile_list;  // &&&&&&&&&&
    //(*file_list)->entries = entries;

    zip_error_t* nnnn  = zip_get_error(zip);

    // loop over the entries in the archive
    for(zip_int64_t idx = 0; idx < entries; ++idx) {
        printf("File number %d, filename is %s \n", idx, zip_get_name(zip, idx, ZIP_FL_ENC_STRICT));

        // open the file at this index
        zip_file_t* fp = zip_fopen_index(zip, idx, 0);

        // process the file
        zip_int64_t len;
        char buf[1024];
        while((len = zip_fread(fp, buf, sizeof buf)) > 0) {
            printf("read %d bytes\n", len);
            // do something with the `len` bytes you have in `buf`
        }
        zip_fclose(fp); // close this file
    }
    zip_close(zip); // close the whole archive

    curl_easy_cleanup(curl);
    free(chunk.memory);
    curl_global_cleanup();
}*/