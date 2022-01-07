#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils.h"
#include "query.h"

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void install_extension(int id){
    printf("Installing extension with id: %d\n", id);
    // download extension file from the api
    printf("Downloading extension file...\n");
    // get the download url
    char* download_url = get_download_url(id);
    printf("Downloading from: %s\n", download_url);
    // download it via curl
    system(concat("curl -#L -o extension.xpi ", download_url));
    printf("Extracting metadata from extension...\n");
    system("unzip -p extension.xpi manifest.json >.manifest");
    printf("Loading metadata from extension...\n");
}
