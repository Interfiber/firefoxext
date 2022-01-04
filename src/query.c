#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.def.h"

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
void query_name(char* name){
    printf("Querying firefox extension database for '%s'...\n", name);
    // Make a request via libcurl to the database
    
    // Replace the QUERY in the extension_query_url with the extension name
    char replace_str[] = "QUERY";
    char* result = NULL;
    result = replace(extension_query_url, replace_str, name);
    printf("Query endpoint: %s\n", result);
}

