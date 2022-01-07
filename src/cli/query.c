#include "query.h"
#include "../utils.h"
#include "../utils.c"
#include "../config.def.h"
#include <string.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include <stdio.h>
// include header files for curl
#include <unistd.h>
#include <curl/curl.h>

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

void query_extension(char name[]){
    printf("Querying mozilla extension store for: '%s'\n", name);
    // get the extension query url
    char* query_url = get_query_url(name);
    // make a request to the extension store
    CURL *curl_handle;
    static const char *queryfilename = "query.json";
    FILE *bodyfile;
    curl_global_init(CURL_GLOBAL_ALL);
 
    curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_URL,query_url);
    // set the curl write function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    
    bodyfile = fopen(queryfilename, "wb");
    if(!bodyfile) {
      curl_easy_cleanup(curl_handle);
      exit(1);
    }
    
    // write all of the body data to a file
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);
    // perform the request
    curl_easy_perform(curl_handle);
    // cleanup everything
    fclose(bodyfile);
    curl_easy_cleanup(curl_handle);    
    printf("Downloaded query info, parsing...\n");
    // parse the contents of query.json
    char *buffer = NULL;
    size_t size = 0;
    FILE *fp = fopen("query.json", "r");
    fseek(fp, 0, SEEK_END); /* Go to end of file */
    size = ftell(fp); /* How many bytes did we pass ? */
    rewind(fp);
    buffer = malloc((size + 1) * sizeof(*buffer)); /* size + 1 byte for the \0 */
    fread(buffer, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */

    /* NULL-terminate the buffer */
    buffer[size] = '\0';
    cJSON* query_result = parse_json_file(buffer);
    printf("Loading results...\n");
    const cJSON* results = NULL;
    results = cJSON_GetObjectItemCaseSensitive(query_result, "results");
    const cJSON *ext = NULL;
    // print all of the results
    cJSON_ArrayForEach(ext, results)
    {
        // char *string = cJSON_Print(ext);
        const cJSON* current_version_json = cJSON_GetObjectItemCaseSensitive(ext, "current_version");
        const cJSON* current_file_version_json = cJSON_GetObjectItemCaseSensitive(current_version_json, "file");
        const cJSON* current_file_id_json = cJSON_GetObjectItemCaseSensitive(current_file_version_json, "id");
        const cJSON* current_store_url_json = cJSON_GetObjectItemCaseSensitive(ext, "url");
        const cJSON* current_promoted_status_json = cJSON_GetObjectItemCaseSensitive(ext, "promoted");
        printf("----------------------------------\n");
        printf("Extension ID: %s\n",  cJSON_Print(current_file_id_json));
        if (strncmp(cJSON_Print(current_promoted_status_json), "null", 51) == 0){
            printf("Extension is promoted: no\n");
        } else {
            printf("Extension is promoted: yes\n");
        }
        printf("Extension Store URL: %s\n", cJSON_Print(current_store_url_json));
    }
    printf("----------------------------------\n");
    printf("To install an extension use: 'firefoxext install [extensionid]'\n");
    // cleanup files
    if (remove("query.json") != 0){
        printf("Failed to remove query.json!\n");
        printf("Please remove this file before running again\n");
        exit(1);
    } else {
        exit(0);
    }
} 
char* replace(const char* s, const char* oldW, char* newW){
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
  
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
    return result;
}
char* get_query_url(char name[]){
    char replace_str[] = "QUERY";
    char* result = NULL;
    result = replace(extension_query_url, replace_str, name);
    result = replace(result, " " , "\%");
    return result;
}
char* get_download_url(int id){
    char id_str[50];
    sprintf(id_str, "%d", id);
    char replace_str[] = "ID";
    char* result = NULL;
    result = replace(extension_download_url, replace_str, id_str);
    return result;
}