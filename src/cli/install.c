#include <cjson/cJSON.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils.h"
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
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
        // location if the varible is set
        config_file = concat(xdg_config_dir, "/firefoxext.json");
    }
    // check if we can read from the config file
    if(access(config_file,F_OK) == -1 ) {
        printf("Failed to find config file, make sure you have read and write access to it!\n");
        printf("Expected config location: %s\n", config_file);
        exit(1);
        return;
    } 
    char *buffer = NULL;
    size_t size = 0;
    FILE *fp = fopen(config_file, "r");
    fseek(fp, 0, SEEK_END); /* Go to end of file */
    size = ftell(fp); /* How many bytes did we pass ? */
    rewind(fp);
    buffer = malloc((size + 1) * sizeof(*buffer)); /* size + 1 byte for the \0 */
    fread(buffer, size, 1, fp); /* Read 1 chunk of size bytes from fp into buffer */

    /* NULL-terminate the buffer */
    buffer[size] = '\0';
    cJSON* config = parse_json_file(buffer);
    //  get profile dir
    const cJSON* profile_dir_json = cJSON_GetObjectItemCaseSensitive(config, "profileDir");
    char* profile_dir = profile_dir_json->valuestring;
    printf("Install: Using profile dir %s\n", profile_dir);
    // check if the profile folder exists
    DIR* dir = opendir(profile_dir);
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        printf("Failed to find profile dir! Make sure the path given in the config file, is aboslute.\n");
        printf("Profile Location: %s\n", profile_dir);
        exit(1);
        return;
    } else {
        printf("Failed to call opendir(), something went wrong when testing if the profile exists!\n");
        printf("Please report this bug to: https://github.com/Interfiber/firefoxext/issues\n");
        exit(1);
        return;
    }
    // update the extension.xpi file name
    printf("Install: Update and Move extension files...\n");
    char* extension_file_name = concat(extension_id, ".xpi");
    char* extensions_dir = concat(profile_dir, "/extensions/");
    rename("extension.xpi", concat(extensions_dir, extension_file_name));
    printf("Install: Cleanup...\n");
    remove(".manifest");
    printf("Install: Restart firefox...\n");
    // TODO: Allow option to not restart firefox
    system("kill firefox");
    printf("Install: Completed\n");
    printf("Note: You will need allow firefox to load the installed extension!\n");
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
    // we need to look for the extension id in:
    // applications.gecko.id
    // browser_specific_settings.gecko.id

    // get the two objects we need
    const cJSON* applications = cJSON_GetObjectItemCaseSensitive(manifest, "applications");
    const cJSON* browser_specific_settings = cJSON_GetObjectItemCaseSensitive(manifest, "browser_specific_settings");

    // load the data from the objects
    cJSON* gecko_applications = cJSON_GetObjectItemCaseSensitive(applications, "gecko");
    cJSON* gecko_browser = cJSON_GetObjectItemCaseSensitive(browser_specific_settings, "gecko");
    cJSON* applications_id = cJSON_GetObjectItemCaseSensitive(gecko_applications, "id");
    cJSON* browser_id = cJSON_GetObjectItemCaseSensitive(gecko_browser, "id");
    // detect which one has a value
    if (applications_id == NULL){
        printf("type of browser id\n");
        install_extension_id(browser_id);
    } else if (browser_id == NULL){
        printf("type of application id\n");
    } else {
        printf("an error occured during installation:\n");
        printf("The extension manifest does not contain a extension id\n");
        remove(".manifest");
        remove("extension.xpi");
        exit(1);
    }
}
