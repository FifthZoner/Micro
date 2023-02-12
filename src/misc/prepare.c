#include "prepare.h"
#include "../config.h"
#include <stdint.h>
#include <stdio.h>

// prepares a file for edition, true when succesful, false when not
bool micro_prepare_file(micro_file* file_ptr, const char* path){

    // gets file size, then in a loop it reads it and interprets the contents into a text mode 

    micros_filesystem_file_info file_info;
    micros_filesystem_get_file_info(path, &file_info);
    uint32_t size_read = 0;
    // i'm not sure if this is treated as a string or normal array
    uint8_t buffer[micro_read_buffer];

    bool line_unfinished = false;
    file_ptr->line_count = 0;
    file_ptr->path = (char*)path;
    file_ptr->is_saved = true;

    // preparing the blocks
    micro_block* last_block = file_ptr->first_block = (micro_block*)malloc(sizeof(micro_block));
    if (!file_ptr->first_block){
        printf("Allocation problem!\n");
        return false;
    }

    last_block->pervious = NULL;
    last_block->next = NULL;
    last_block->size = 0;
    uint8_t current_line = 0;
    uint32_t current_index = 0;
    printf("1\n");

    while (size_read != file_info.size){

        printf("2\n");
        micros_process_current_process_sleep(200);

        uint32_t size_to_read = file_info.size - size_read;
        if (size_to_read > micro_read_buffer){
            size_to_read = micro_read_buffer;
        }

        printf("3\n");
        micros_process_current_process_sleep(200);

        micros_filesystem_read_file(path, buffer, size_read, size_to_read);

        // finding the amount of newlines
        uint32_t newlines = 0;
        for (uint32_t n = 0; n < micro_read_buffer; n++){
            if (buffer[n] == '\n'){
                newlines++;
            }
        }
        uint32_t* line_lengths = (uint32_t*)malloc(sizeof(uint32_t) * newlines);
        uint32_t total = 0;
         for (uint32_t amount = 0, last = 0; amount < newlines; total++){
            if (buffer[total] == '\n'){
                line_lengths[amount] = total - last;
                last = total;
                amount++;
            }
        }

        printf("4\n");
        micros_process_current_process_sleep(200);

        while (newlines > 0){
            uint32_t new_size = (uint32_t)last_block->size + newlines;
            if (new_size > micro_block_size){
                new_size = micro_block_size;
            }
            newlines -= new_size - (uint32_t)last_block->size;

            last_block->text = (char**)realloc(last_block->text, new_size * sizeof(char*));
            // add check here

            // actual text loading
            

            printf("5 %i %i %i\n", newlines, new_size, last_block->size);
            micros_process_current_process_sleep(200);


            for (uint32_t n = 0; n < micro_read_buffer;){
                while (buffer[n] != '\n'){
                    // end of buffer
                    if (n == micro_read_buffer - 1){
                        // an unfinished line at the end of buffer
                    }
                    else{
                        // normal filling
                    }
                }
                // end of line here
            }



            printf("Loaded this one\n");

            last_block->size = new_size;
            if (newlines > 0){
                last_block->next = (micro_block*)malloc(sizeof(micro_block));
                last_block->next->pervious = last_block;
                last_block = last_block->next;
                last_block->next = NULL;
                last_block->size = 0;
                last_block->text = NULL;
            }

        } 

        printf("6\n");
        micros_process_current_process_sleep(500);
         

        file_ptr->line_count+= newlines;

        size_read += size_to_read;
    }


    return true;
}