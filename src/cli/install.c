#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils.h"
#include "query.h"

char* concat(const char *s1, const char *s2){
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
// this function is called after we find the extensions id, its not called by any cli commands
void install_extension_id(const cJSON* id){
    char* extension_id;
    extension_id=id->valuestring;
    printf("Installing Extension with id '%s'\n", extension_id);
    printf("Install: Read profile config...\n");
    // read the profile config
    char* xdg_config_dir = get_config_dir();
    char *homedir = getenv("HOME");
    char* config_file;
    if (!xdg_config_dir){
        // default location
        config_file = concat(homedir, "/.config/firefoxext.json");
    } else {
        config_file = concat(xdg_config_dir, "/firefoxext.json");
    }
    printf("Config file location: %s\n", config_file);

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
    // parse .manifest
     char *buffer = NULL;
    size_t size = 0;
    FILE *fp = fopen(".manifest", "r");
    fseek(fp, 0, SEEK_END); /* Go to end of file */
    size = ftell(fp); /* How many bytes did we pass ? */
    rewind(fp);
    buffer = malloc((size + 1) * sizeof(*buffer)); /* size + 1 byte for the \0 */
    fread(buffer, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */

    /* NULL-terminate the buffer */
    buffer[size] = '\0';
    cJSON* manifest = parse_json_file(buffer);
    printf("%s\n", cJSON_Print(manifest));
    // we need to look for the extension id in:
    // applications.gecko.id
    // browser_specific_settings.gecko.id

    // get the two objects we need
    const cJSON* applications = cJSON_GetObjectItemCaseSensitive(manifest, "applications");
    const cJSON* browser_specific_settings = cJSON_GetObjectItemCaseSensitive(manifest, "browser_specific_settings");

    // load the data from the objects
    const cJSON* gecko_applications = cJSON_GetObjectItemCaseSensitive(applications, "gecko");
    const cJSON* gecko_browser = cJSON_GetObjectItemCaseSensitive(browser_specific_settings, "gecko");
    const cJSON* applications_id = cJSON_GetObjectItemCaseSensitive(gecko_applications, "id");
    const cJSON* browser_id = cJSON_GetObjectItemCaseSensitive(gecko_browser, "id");
    // detect which one has a value
    if (!cJSON_IsNull(applications_id)){
        printf("type of application id\n");
        install_extension_id(applications_id);
    } else if (!cJSON_IsNull(browser_id)){
        printf("type of browser id\n");
    } else {
        printf("an error occured during installation:\n");
        printf("The extension manifest does not contain a extension id\n");
        remove(".manifest");
        remove("extension.xpi");
        exit(1);
    }
}