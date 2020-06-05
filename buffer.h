#ifndef INCLUDE_BUFFER_H
#define INCLUDE_BUFFER_H

#include <stdio.h>
#include <stdlib.h>

#define META_SUCCESS 0
#define META_FAILURE -1


typedef struct InputBuffer {
    char* buffer;
    size_t buffer_size;
    ssize_t input_size;
} InputBuffer;

InputBuffer* new_input_buffer();

void read_input_buffer(InputBuffer* input_buffer);

int clear_input_buffer(InputBuffer* input_buffer);

#endif