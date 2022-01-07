#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>
cJSON* parse_json_file(char* data){
    // read data from file
    cJSON* root_json = cJSON_Parse(data); 
    if (root_json == NULL){
        printf("An error occured while processing the query results.\n");
        exit(1);
    }
    return root_json;
}