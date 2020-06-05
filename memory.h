#ifndef INCLUDE_MEMORY_H
#define INCLUDE_MEMORY_H

#define SAMPLE_COLUMN_SIZE_1 50
#define SAMPLE_COLUMN_SIZE_2 255
#define MAX_PAGES_PER_TABLE 100
#define get_member_size(type, member) sizeof(((type *)0)->member)

typedef struct Row{
    int row_id;
    char col1[SAMPLE_COLUMN_SIZE_1]; //currently test table with only 3 columns, future plans to expand to more columns
    char col2[SAMPLE_COLUMN_SIZE_2];
} Row;


typedef struct Pager{
    void* pages[MAX_PAGES_PER_TABLE];
    int fd;
    int flength;
} Pager;

typedef struct Table{
    int number_of_rows;
    Pager* file_pager;
} Table;


typedef struct Cursor{
    int row_number;
    Table* table;
    bool is_end;
} Cursor;


Cursor* init_start_cursor(Table* table);

Table* init_sqlyt_db(const char* file_name);

Cursor* init_end_cursor(Table* table);

void increment_cursor(Cursor* cursor);

void* get_page_data(Pager* pager,int page_number);

int flush_page_data(Pager* pager, int page_number,int size);

void close_sqlyt_db(Table* table);

void* get_cursor_page_data(Cursor* cursor);

void free_pager_table(Table* table);
#endif