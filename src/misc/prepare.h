#ifndef PREPARE_H
#define PREPARE_H

#include <micros.h>
#include "../file_struct.h"

// prepares a file for edition, true when succesful, false when not
bool micro_prepare_file(micro_file* file_ptr, const char* path);

#endif