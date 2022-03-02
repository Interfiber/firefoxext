#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <string.h>
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

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

bool extension_dir_exists(char *profile_dir){
  char* ext_dir = concat(profile_dir, "/extensions");
  DIR* extension_directory = opendir(ext_dir);
  if (extension_directory){
    closedir(extension_directory);
    return true;
  } else if (ENOENT == errno){
    return false;
  } else {
    printf("Failed to execute opendir() on profile dir\n");
    exit(1);
  }
}
