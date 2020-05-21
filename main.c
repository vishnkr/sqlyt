#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"


void print_help_info(){
    printf("Welcome to SQLyt - a simple sqlite clone\n");
    printf("Use the -h flag to bring up this help section:\n    Available commands : 1) .exit - exit database\n\n");

}

void print_prompt(){
    printf("sqlyt db > ");
}

int main(int argc, char* argv[]){
    print_help_info();
    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input_buffer(input_buffer);
        if(strcmp(input_buffer->buffer,".exit")==0){
            free(input_buffer->buffer);
            free(input_buffer);
            break;
        }
        else{
            printf("'%s' - command not recognised\n",input_buffer->buffer);
        }
    }
    printf("\n");
}