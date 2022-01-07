#include "query.h"
#include "config.def.h"
#include <string.h>
#include <stdlib.h>
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
    return result;
}