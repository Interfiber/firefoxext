#include <cjson/cJSON.h>
#include <stdbool.h>

cJSON* parse_json_file(char* data);
char* get_config_dir();
bool extension_dir_exists(char* profile_dir);
