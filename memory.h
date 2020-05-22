#include <stdio.h>

#define SAMPLE_COLUMN_SIZE_1 50
#define SAMPLE_COLUMN_SIZE_2 255
#define MAX_PAGES_PER_TABLE 100
#define get_member_size(type, member) sizeof(((type *)0)->member)

typedef struct Row{
    int row_id;
    char* col1[SAMPLE_COLUMN_SIZE_1];
    char* col2[SAMPLE_COLUMN_SIZE_2];
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


