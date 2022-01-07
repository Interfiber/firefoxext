#include <stdlib.h>
#include "cli/query.h"
#include "cli/query.c"

int main(int argc, char* argv[]){
    if (argc == 1){
        printf("firefoxext - Install firefox addons from the commandlin\n");
        printf("Author: Interfiber <webmaster@interfiber.dev>\n");
        printf("firefoxext GitHub: https://github.com/Interfiber/firefoxext.git\n");
        printf("Commands:\n");
        printf("    query [name]           - Query the extension store for addons\n");
        printf("    install [extensionid]  - Install an extension from the addons store\n");
        printf("    installtheme [id]      - Install a theme from the addons store\n");
        exit(1);
    } else {
        if (argc == 2){
            // parse commands without any arguments
            if (strncmp(argv[1], "version", 51) == 0){
                printf("Current version: 0.0.1\n");
                exit(0);
            } else {
                printf("No such command takes 0 arguments\n");
                exit(1);
            }
        } else if (argc == 3){
            // parse commands with arguments
            if (strncmp(argv[1], "install", 51) == 0){
                long extension_id = atoi(argv[2]);
                printf("%ld", extension_id);
            } else if (strncmp(argv[1], "query", 51) == 0){
                query_extension(argv[2]);
            }
        }
    }
    // query_extension("ublock");
}