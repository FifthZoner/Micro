#include <stdint.h>

// a block of text, a list of those forms a whole file
// they should be more efficient that a table and much easier to allocate on the go
// it will be loaded at specified size and when the max one is reached a new one will be created behind it
// they form a list that should be easy to extend and shorten as needed

typedef struct micro_block{

    char** text;
    // max 67 million lines whould be enough, can be changed if it's too little
    uint32_t firstIndex:26;
    uint32_t size:6;
    struct micro_block* pervious;
    struct micro_block* next;

}micro_block;

// stores a whole file
typedef struct {

    micro_block* first_block;
    char* path;
    uint32_t line_count:26;
    uint32_t is_saved:1;
    uint32_t other_info:5;

}micro_file;