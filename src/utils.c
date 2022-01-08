#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>
cJSON* parse_json_file(char* data){
    // read data from file
    cJSON* root_json = cJSON_Parse(data); 
    if (root_json == NULL){
        printf("An error occured while processing the query results.\n");
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            fprintf(stderr, "Error log: %s\n", error_ptr);
        } else {
            printf("Failed to get cJSON error log!\n");
        }
        exit(1);
    }
    return root_json;
}
char* get_config_dir(){
    char* xdg_config_dir = getenv("XDG_CONFIG_HOME");
    return xdg_config_dir;
}