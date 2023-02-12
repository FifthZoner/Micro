// Welcome to micro!
// The whole thing was "absolutely not" inspired by "gnu nano", trust me

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <micros.h>
#include "misc/prepare.h"

int main(int argc, char* argv[]){

    printf("%i %i\n", sizeof(micro_block), sizeof(micro_file));

    // argv[0] - .elf path
    
    // argv[1] - current shell path
    // argv[2] - argument string

    // creates a single path string and checks if the file exists, if not, it creates one
    char* file_path = (char*)malloc(sizeof(char) * (strlen(argv[1]) + strlen(argv[2]) + 1));

    if (file_path == NULL){
        printf("Memory allocation refused to cooperate!\n");
        return -1;
    }

    strcpy(file_path, argv[1]);
    strcat(file_path, argv[2]);

    // might add a question about creating in the future
    if (micros_filesystem_is_file(file_path) == false){
        if (micros_filesystem_create_file(file_path) == false){
            printf("Error while creating the file!\n");
            return -1;
        }
        printf("Created file!\n");
    }

    printf("Reading file...\n");

    micro_file file;
    micro_prepare_file(&file, file_path);

    free(file_path);

    return 0;
}