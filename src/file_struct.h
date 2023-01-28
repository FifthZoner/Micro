#include <stdint.h>

// stores a whole file
typedef struct {

    char* path;
    char** text;
    // over 2 billion lines should be enough
    uint32_t line_count:31;
    uint32_t is_saved:1;

}micro_file;