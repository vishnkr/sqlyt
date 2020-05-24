#include <stdio.h>

#define SAMPLE_COLUMN_SIZE_1 50
#define SAMPLE_COLUMN_SIZE_2 255
#define MAX_PAGES_PER_TABLE 100
#define get_member_size(type, member) sizeof(((type *)0)->member)

typedef struct Row{
    int row_id;
    char col1[SAMPLE_COLUMN_SIZE_1]; //currently test table with only 3 columns, future plans to expand to more columns
    char col2[SAMPLE_COLUMN_SIZE_2];
} Row;

typedef struct Table{
    int number_of_rows;
    void* pages[MAX_PAGES_PER_TABLE];
} Table;

const uint32_t ROW_ID_SIZE = get_member_size(Row,row_id);
const uint32_t COL_1_SIZE = get_member_size(Row,col1);
const uint32_t COL_2_SIZE = get_member_size(Row,col2);
const uint32_t ROW_SIZE = ROW_ID_SIZE+COL_1_SIZE+COL_2_SIZE;
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t MAX_ROWS = ROWS_PER_PAGE*MAX_PAGES_PER_TABLE; 
const uint32_t COL_1_OFFSET= ROW_ID_SIZE;
const uint32_t COL_2_OFFSET = COL_1_SIZE+COL_1_OFFSET;

Table* init_table(){
    Table* new_table = (Table*)malloc(sizeof(Table));
    new_table->number_of_rows=0;
    for(int i =0;i<MAX_PAGES_PER_TABLE;i++){
        new_table->pages[i]=NULL;
    }
    return new_table;
}

void* get_row_insert_location(Table* table, int row_number){
    int page_num,row_offset,byte_offset;
    page_num = row_number/ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if (page==NULL){
        page = malloc(PAGE_SIZE);
    }
    row_offset = row_number/ROWS_PER_PAGE;
    byte_offset = row_offset*ROW_SIZE;
    return page+byte_offset;
}

void free_table(Table* table){
    for(int i=0;i<MAX_PAGES_PER_TABLE;i++){
        free(table->pages[i]);
    }
    free(table);
}



