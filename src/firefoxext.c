#include "cli.h"
#include "query.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    if (argc == 1){
        print_help();
    } else if (argc == 2){
        printf("Error: Got command '%s' but no arguments.\n",argv[1]);
        exit(EXIT_FAILURE);
    } else if (argc == 3){
        query_name(argv[2]);   
    }
}