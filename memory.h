#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#define SAMPLE_COLUMN_SIZE_1 50
#define SAMPLE_COLUMN_SIZE_2 255
#define MAX_PAGES_PER_TABLE 100
#define get_member_size(type, member) sizeof(((type *)0)->member)

typedef struct Row{
    int row_id;
    char col1[SAMPLE_COLUMN_SIZE_1]; //currently test table with only 3 columns, future plans to expand to more columns based on args from insert or adding a new create table statement?
    char col2[SAMPLE_COLUMN_SIZE_2];
} Row;


typedef struct Pager{
    void* pages[MAX_PAGES_PER_TABLE];
    int fd;
    int num_pages;
    int flength;
} Pager;

typedef struct Table{
    int number_of_rows;
    int root_page;
    Pager* file_pager;
} Table;


typedef struct Cursor{
    int page_number;
    int cell_number;
    Table* table;
    bool is_end;
} Cursor;


Cursor* init_start_cursor(Table* table);

Table* init_sqlyt_db(const char* file_name);

Cursor* init_end_cursor(Table* table);

void increment_cursor(Cursor* cursor);

void* get_page_data(Pager* pager,int page_number);

int flush_page_data(Pager* pager, int page_number);

void close_sqlyt_db(Table* table);

void* get_cursor_value(Cursor* cursor);

void free_pager_table(Table* table);


typedef enum {
    INTERNAL_NODE,
    LEAF_NODE
} NodeType;


uint32_t* leaf_node_number_cells(void* node);

void init_leaf(void* node);

void* get_leaf_node_cell(void* node,uint32_t cell_number );

uint32_t* get_leaf_key(void* node,uint32_t cell_number);

void* get_leaf_node_value(void* node, uint32_t cell_number);



#endif