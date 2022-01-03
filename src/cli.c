#include "cli.h"
#include <stdio.h>
void print_help(){
    printf("firefox ext version 0.0.1\n");
    printf("Author: Interfiber<webmaster@interfiber.dev>\n");
    printf("Commands:\n");
    printf("      install [extension name] - Install the given extension\n");
    printf("      search [query]           - Query the firefox store for an extension\n");
    printf("      installfromfile [file]   - Install extensions from file\n");
}