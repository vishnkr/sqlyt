#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "memory.h"

#define STATEMENT_SUCCESS 0
#define STATEMENT_FAILURE -1



typedef enum{
    INSERT_STATEMENT,
    SELECT_STATEMENT,
    DELETE_STATEMENT
}StatementType;


typedef struct Statement{
    StatementType type;
    Row insert_row;
}Statement;

int execute_insert(Statement* statement, Table* table);
int execute_select(Statement* statement, Table* table);

void print_help_info(){
    printf("Welcome to SQLyt - a simple sqlite clone\n");
    printf("Use '.info' to bring up this help section:\n    Available meta commands : 1) .exit - exit database\n\n");

}

void print_prompt(){
    printf("sqlyt db > ");
}

int process_meta_command(InputBuffer* input_buffer,Table* table){
    if(strncmp(input_buffer->buffer,".",1)==0){
    if(strncmp(input_buffer->buffer+1,"exit",4)==0){
        close_sqlyt_db(table);
        free_pager_table(table);
        clear_input_buffer(input_buffer);
    }
    else if(strncmp(input_buffer->buffer+1,"info",4)==0){
        print_help_info();
    }
    else{
            printf("'%s' - meta command not recognised\n",input_buffer->buffer);
            return META_FAILURE;
        }
    }
    return META_SUCCESS;
}



int prepare_sql_statement(Statement* statement, InputBuffer* input_buffer){
    
    if(strncmp(input_buffer->buffer,"select",6)==0){
        printf("select statement found\n");
        statement->type = SELECT_STATEMENT;
    }
    else if (strncmp(input_buffer->buffer,"insert",6)==0){
        printf("insert found\n");
        statement->type = INSERT_STATEMENT;
        sscanf(input_buffer->buffer,"insert %d %s %s",&((statement->insert_row).row_id),(statement->insert_row).col1,(statement->insert_row).col2);
    } 
    else{
        printf("invalid statement\n");
    }
    return 0;
}    

void execute_sql_statement(Statement* statement, Table* table){
    switch(statement->type){
        case SELECT_STATEMENT:
            execute_select(statement,table);
            break;
        case INSERT_STATEMENT:
            execute_insert(statement,table);
            break;
        default:
            break;
    }
}

void serialize_data(Row* src_row, void* dest_row){
    memcpy(dest_row,&(src_row->row_id),ROW_ID_SIZE);
    memcpy(dest_row+COL_1_OFFSET,&(src_row->col1),COL_1_SIZE);
    memcpy(dest_row+COL_2_OFFSET,&(src_row->col2),COL_2_SIZE);
}

void deserialize_data(void* src_row, Row* dest_row){
    memcpy(&(dest_row->row_id),src_row,ROW_ID_SIZE);
    memcpy(&(dest_row->col1),src_row+COL_1_OFFSET,COL_1_SIZE);
    memcpy(&(dest_row->col2),src_row+COL_2_OFFSET,COL_2_SIZE);
}   

int execute_insert(Statement* statement, Table* table){
    Cursor* cursor = init_end_cursor(table);
    //check if table is full
    if(table->number_of_rows==MAX_ROWS){
        return STATEMENT_FAILURE;
    }
    serialize_data(&(statement->insert_row),get_cursor_page_data(cursor));
    table->number_of_rows+=1;
    return STATEMENT_SUCCESS;

}

int execute_select(Statement* statement, Table* table){
    Cursor* cursor = init_start_cursor(table);
    Row ret_row;
    while(!cursor->is_end){
        deserialize_data(get_cursor_page_data(cursor),&ret_row);
        printf("%d| %s| %s\n",ret_row.row_id,ret_row.col1,ret_row.col2);
        increment_cursor(cursor);
    }
    free(cursor);
    return 0;
}

int main(int argc, char* argv[]){
    print_help_info();
    InputBuffer* input_buffer = new_input_buffer();
    Table* sample_table = init_sqlyt_db(argv[1]);
    printf("Opening file: db_files/%s\n",argv[1]);
    while(true){
        print_prompt();
        read_input_buffer(input_buffer);
        if(input_buffer)
        switch(process_meta_command(input_buffer,sample_table)){
            case META_SUCCESS:
                break;
            case META_FAILURE:
                continue;
                break;
            default:
                break;
        }
        Statement sql_statement;
        switch (prepare_sql_statement(&sql_statement,input_buffer))
        {
            case STATEMENT_SUCCESS:
                execute_sql_statement(&sql_statement,sample_table);
                //free_table(sample_table);
                break;
            case STATEMENT_FAILURE: //not possible currently
                break;
            default:
                break;
        }
    }
    printf("\n");
    return 0;
}