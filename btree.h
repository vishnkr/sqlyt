#include <stdio.h>
#include <stdint.h>


typedef enum {
    INTERNAL_NODE,
    LEAF_NODE
} NodeType;


uint32_t* leaf_node_number(void* node);

void init_leaf(void* node);

void* get_leaf_node_cell(void* node,uint32_t cell_number );

uint32_t* get_leaf_key(void* node,uint32_t cell_number);