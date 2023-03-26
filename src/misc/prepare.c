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
    uint32_t line_unfinished_offset = 0;
    file_ptr->line_count = 0;
    file_ptr->path = (char*)path;
    file_ptr->is_saved = true;

    // preparing the blocks
    micro_block* last_block = file_ptr->first_block = (micro_block*)malloc(sizeof(micro_block));
    if (!file_ptr->first_block){
        printf("Could not allocate first block!\n");
        return false;
    }

    last_block->pervious = NULL;
    last_block->next = NULL;
    last_block->size = 0;
    uint8_t current_line = 0;
    uint32_t current_index = 0;
    printf("Prepared first block\n");
    micros_process_current_process_sleep(200);

    while (size_read != file_info.size){

        uint32_t size_to_read = file_info.size - size_read;
        if (size_to_read > micro_read_buffer){
            size_to_read = micro_read_buffer;
        }

        printf("Calculated %i bytes to be read at %i\n", size_to_read, size_read);
        micros_process_current_process_sleep(500);

        micros_filesystem_read_file(path, buffer, size_read, size_to_read);


        // finding the amount of newlines
        uint32_t newlines = 0;
        for (uint32_t n = 0; n < micro_read_buffer; n++){
            if (buffer[n] == '\n'){
                newlines++;
            }
        }
        bool next_line_unfinished = false;
        if (buffer[size_to_read - 1] != '\n'){
            next_line_unfinished = true;
            if (!line_unfinished){
                newlines++;
            }
        }

        printf("Reading %i new lines of text...\n", newlines);
        micros_process_current_process_sleep(500);

        // preparing space for the new data
        uint32_t lines_to_set = last_block->size + newlines;
        bool was_filled = false;
        if (lines_to_set > micro_block_size){
            lines_to_set = micro_block_size;
            was_filled = true;
        }

        if (newlines > 0){
            last_block->text = (char**)realloc(last_block->text, lines_to_set * sizeof(char*));
            if (last_block->text == NULL){
                printf("Could not reallocate the text!\n");
                return false;
            }
        }

        // actual filling
        uint32_t last_start = 0;
        uint32_t newlines_read = 0;
        uint32_t n = 0;
        for (; n < size_to_read; n++){
            if (buffer[n] == '\n'){

                
                // temp
                for (int m = 0; m < n - last_start; m++){
                    //printf("%c", buffer[last_start + m]);
                }
                //printf("\n");
                
                

                // case where some text is already here
                if (line_unfinished == true){
                    line_unfinished = false;
                    //last_block->text[n + last_block->size] = (char*)realloc(last_block->text[n + last_block->size], (n - last_start + 1 + line_unfinished_offset) * sizeof(char));
                    for (int m = 0; m < n - last_start; m++){
                        //last_block->text[n + last_block->size][m + line_unfinished_offset] = buffer[last_start + m];
                    }
                    //last_block->text[n + last_block->size][n - last_start + line_unfinished_offset] = '\0';

                }
                else{
                    //last_block->text[n + last_block->size] = (char*)malloc((n - last_start + 1) * sizeof(char));
                    for (int m = 0; m < n - last_start; m++){
                        //last_block->text[n + last_block->size][m] = buffer[last_start + m];
                    }
                    //last_block->text[n + last_block->size][n - last_start] = '\0';
                }

                last_start = n + 1;
            }
        }

        
        

        // last line and no newlines case

        if (next_line_unfinished){
            for (int m = last_start; m < size_to_read; m++){
                    //printf("%c", buffer[m]);
                    micros_process_current_process_sleep(200);
                }
        }

        printf("hefsg\n");
        micros_process_current_process_sleep(500);


        last_block->size = lines_to_set;
        newlines -= newlines_read;
        size_read += size_to_read;




        /*

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
        */
    }


    return true;
}