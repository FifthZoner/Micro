#include "prepare.h"
#include "../config.h"
#include <stdint.h>

// prepares a file for edition
bool micro_prepare_file(micro_file* file_ptr, const char* path){

    // gets file size, then in a loop it reads it and interprets the contents into a text mode 

    micros_filesystem_file_info file_info;
    micros_filesystem_get_file_info(file_ptr, &file_info);
    uint32_t size_read = 0;
    // i'm not sure if this is treated as a string or normal array
    uint8_t buffer[micro_read_buffer];

    bool line_unfinished = false;
    file_ptr->line_count = 0;
    file_ptr->path = path;
    file_ptr->text = NULL;
    file_ptr->is_saved = true;

    while (size_read < file_info.size){

        uint32_t size_to_read = file_info.size - size_read;
        if (size_to_read > micro_read_buffer){
            size_to_read = micro_read_buffer;
        }

        micros_filesystem_read_file(path, buffer, size_read, size_to_read);

        // finding the amount of newlines
        uint32_t newlines = 0;
        for (uint32_t n = 0; n < micro_read_buffer; n++){
            if (buffer[n] == '\n'){
                newlines++;
            }
        }

        


        size_read += micro_read_buffer;
    }


    return true;
}