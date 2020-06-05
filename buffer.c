
#include "buffer.h"

InputBuffer* new_input_buffer(){
    InputBuffer* inp_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    inp_buffer->buffer=NULL;
    inp_buffer->buffer_size = 0;
    inp_buffer->input_size = 0;

    return inp_buffer;
}

void read_input_buffer(InputBuffer* input_buffer){
    ssize_t bytes_read = getline(&(input_buffer->buffer),&(input_buffer->buffer_size),stdin);
    if (bytes_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }
  input_buffer->input_size = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

int clear_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
    exit(EXIT_FAILURE);
    return 0;
}