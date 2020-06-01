#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

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

const uint32_t ROW_ID_SIZE = get_member_size(Row,row_id);
const uint32_t COL_1_SIZE = get_member_size(Row,col1);
const uint32_t COL_2_SIZE = get_member_size(Row,col2);
const uint32_t ROW_SIZE = ROW_ID_SIZE+COL_1_SIZE+COL_2_SIZE;
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE/ROW_SIZE;
const uint32_t MAX_ROWS = ROWS_PER_PAGE*MAX_PAGES_PER_TABLE; 
const uint32_t COL_1_OFFSET= ROW_ID_SIZE;
const uint32_t COL_2_OFFSET = COL_1_SIZE+COL_1_OFFSET;


Cursor* init_start_cursor(Table* table){
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->is_end = (table->number_of_rows==0)? true: false;
    cursor->row_number = 0;
    cursor->table= table;
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
    new_table->file_pager = new_pager;
    uint32_t num_rows = new_pager->flength / ROW_SIZE;
    new_table->number_of_rows=num_rows;
    for(int i =0;i<MAX_PAGES_PER_TABLE;i++){
        new_table->file_pager->pages[i]=NULL;
    }
    free(result);
    return new_table;
}

Cursor* init_end_cursor(Table* table){
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->is_end = true;
    cursor->row_number = table->number_of_rows;
    cursor->table = table;
    return cursor;
}

void increment_cursor(Cursor* cursor){
    cursor->row_number+=1;
    if(cursor->row_number>=cursor->table->number_of_rows){
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
    }
    return pager->pages[page_number];
}

int flush_page_data(Pager* pager, int page_number,int size){
    int offset = lseek(pager->fd, page_number * PAGE_SIZE, SEEK_SET);
    int written_bytes = write(pager->fd,pager->pages[page_number],size);
    printf("offset:%d, written:%d",offset,written_bytes);
    return 0;
}

void close_sqlyt_db(Table* table){
    uint32_t full_pages_count = table->number_of_rows/ROWS_PER_PAGE;
    for (int i = 0; i <= full_pages_count; i++) {
        if (table->file_pager->pages[i] == NULL) {
            continue;
        }
        printf("%d ",i);
        flush_page_data(table->file_pager, i, table->number_of_rows*ROW_SIZE);
        free(table->file_pager->pages[i]);
        table->file_pager->pages[i] = NULL;
    }
    int close_result = close(table->file_pager->fd);
    printf("close:%d\n",close_result);
}

void* get_cursor_page_data(Cursor* cursor){
    int page_num,row_offset,byte_offset,row_number;
    Table* table;
    row_number=cursor->row_number;
    table = cursor->table;
    page_num = row_number / ROWS_PER_PAGE;
    void* page = get_page_data(table->file_pager, page_num);
    row_offset = row_number % ROWS_PER_PAGE;
    byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}

void free_pager_table(Table* table){
    for(int i=0;i<MAX_PAGES_PER_TABLE;i++){
        free(table->file_pager->pages[i]);
    }
    free(table->file_pager);
    free(table);
}



