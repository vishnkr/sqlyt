#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "memory.h"




const uint32_t ROW_ID_SIZE = get_member_size(Row,row_id);
const uint32_t COL_1_SIZE = get_member_size(Row,col1);
const uint32_t COL_2_SIZE = get_member_size(Row,col2);
const uint32_t COL_1_OFFSET= ROW_ID_SIZE;
const uint32_t COL_2_OFFSET = COL_1_SIZE+COL_1_OFFSET;
const uint32_t ROW_SIZE = ROW_ID_SIZE+COL_1_SIZE+COL_2_SIZE;
const uint32_t PAGE_SIZE = 4096;
const int TABLE_MAX_PAGES = 100;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t MAX_ROWS = ROWS_PER_PAGE*MAX_PAGES_PER_TABLE; 

const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = IS_ROOT_SIZE;
const uint32_t POINTER_PARENT_SIZE = sizeof(uint32_t);
const uint32_t POINTER_PARENT_OFFSET = IS_ROOT_OFFSET+IS_ROOT_SIZE;
const uint32_t HEADER_NODE_SIZE = NODE_TYPE_SIZE+IS_ROOT_SIZE+POINTER_PARENT_SIZE;

//leaf node header
const uint32_t LEAF_NODE_CELL_COUNT_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_CELL_COUNT_OFFSET = HEADER_NODE_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = LEAF_NODE_CELL_COUNT_OFFSET + LEAF_NODE_CELL_COUNT_SIZE;

const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;


uint32_t* leaf_node_number_cells(void* node){
    return node+ LEAF_NODE_CELL_COUNT_OFFSET;
}

void* get_leaf_node_cell(void* node,uint32_t cell_number ){
    return node + LEAF_NODE_HEADER_SIZE + cell_number * LEAF_NODE_CELL_SIZE;
}

void init_leaf(void* node){
    *leaf_node_number_cells(node) = 0;
}

uint32_t* get_leaf_key(void* node,uint32_t cell_number){
    return get_leaf_node_cell(node, cell_number);
}

void* get_leaf_node_value(void* node, uint32_t cell_number){
    return get_leaf_node_cell(node, cell_number) + LEAF_NODE_KEY_SIZE;
}

Cursor* init_start_cursor(Table* table){
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->cell_number = 0;
    cursor->page_number = table->root_page;
    cursor->table= table;
    void* root_node = get_page_data(table->file_pager,table->root_page);
    int cell_count = *leaf_node_number_cells(root_node);
    cursor->is_end = cell_count==0 ? true: false;
    return cursor;
}


Table* init_sqlyt_db(const char* file_name){
    Table* new_table = (Table*)malloc(sizeof(Table));
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    const char* file_path = "db_files/";
    char *result = malloc(strlen(file_path) + strlen(file_name) + 1);
    strcpy(result, file_path);
    strcat(result, file_name);
    int new_fd = open(result,O_RDWR|O_CREAT,mode);//user write,read and open with read/write
    Pager* new_pager = (Pager*)malloc(sizeof(Pager));
    new_pager->fd = new_fd;
    new_pager->flength= lseek(new_fd, 0, SEEK_END);
    new_pager->num_pages = new_pager->flength/PAGE_SIZE;

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        new_pager->pages[i] = NULL;
    }

    if(new_pager->flength%PAGE_SIZE!=0){
        printf("db does not contain complete pages");
        exit(EXIT_FAILURE);
    }
    new_table->file_pager = new_pager;
    new_table->root_page=0;
    if(new_pager->num_pages==0){
        void* root_node = get_page_data(new_pager,0);
        init_leaf(root_node);
    }
    free(result);
    return new_table;
}

Cursor* init_end_cursor(Table* table){
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->is_end = true;
    cursor->page_number = table->root_page;
    void* root_node = get_page_data(table->file_pager,table->root_page);
    cursor->cell_number = *leaf_node_number_cells(root_node);
    cursor->table = table;
    return cursor;
}

void increment_cursor(Cursor* cursor){
    void* node = get_page_data(cursor->table->file_pager,cursor->page_number);
    cursor->cell_number+=1;
    if(cursor->cell_number>=(*leaf_node_number_cells(node))){
        cursor->is_end=true;
    }
}

void* get_page_data(Pager* pager,int page_number){
    int used_page_count;
    if(pager->pages[page_number]==NULL){
        pager->pages[page_number] = malloc(PAGE_SIZE);
        used_page_count = pager->flength/PAGE_SIZE;
        if (pager->flength % PAGE_SIZE) {
            used_page_count += 1;
        }
        if (page_number<=used_page_count){
            lseek(pager->fd, page_number * PAGE_SIZE, SEEK_SET);
            int bytes_read = read(pager->fd,pager->pages[page_number],PAGE_SIZE);
            printf("read:%d\n",bytes_read);
        }
        if(page_number >= pager->num_pages){
            pager->num_pages = page_number+1;
        }
    }
    return pager->pages[page_number];
}

int flush_page_data(Pager* pager, int page_number){
    int offset = lseek(pager->fd, page_number * PAGE_SIZE, SEEK_SET);
    int written_bytes = write(pager->fd,pager->pages[page_number],PAGE_SIZE);
    printf("offset:%d, written:%d",offset,written_bytes);
    return 0;
}

void close_sqlyt_db(Table* table){
    //uint32_t full_pages_count = table->number_of_rows/ROWS_PER_PAGE;
    for (int i = 0; i <= table->file_pager->num_pages; i++) {
        if (table->file_pager->pages[i] == NULL) {
            continue;
        }
        printf("%d ",i);
        flush_page_data(table->file_pager, i);
        free(table->file_pager->pages[i]);
        table->file_pager->pages[i] = NULL;
    }
    int close_result = close(table->file_pager->fd);
    printf("close:%d\n",close_result);
}


void* get_cursor_value(Cursor* cursor){
    int page_number;
    Table* table;
    table = cursor->table;
    page_number = cursor->page_number;
    void* page = get_page_data(table->file_pager, page_number);
    return get_leaf_node_value(page,cursor->cell_number);
}

void free_pager_table(Table* table){
    for(int i=0;i<MAX_PAGES_PER_TABLE;i++){
        free(table->file_pager->pages[i]);
    }
    free(table->file_pager);
    free(table);
}
