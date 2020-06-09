
#include <stdio.h>
#include <stdint.h>
#include "btree.h"



//regular NODE HEADER
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = IS_ROOT_SIZE;
const uint32_t POINTER_PARENT_SIZE = sizeof(uint32_t);
const uint32_t POINTER_PARENT_OFFSET = IS_ROOT_OFFSET+IS_ROOT_SIZE;
const uint32_t HEADER_NODE_SIZE = NODETYPE_SIZE+IS_ROOT_SIZE+POINTER_PARENT_SIZE;

//leaf node header
const uint32_t LEAF_NODE_CELL_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_CELL_OFFSET = HEADER_NODE_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = LEAF_NODE_CELL_OFFSET + LEAF_NODE_CELL_SIZE;

uint32_t* leaf_node_number_cells(void* node){
    return node+ LEAF_NODE_CELL_OFFSET;
}

void* get_leaf_node_cell(void* node,uint32_t cell_number ){
    return node + LEAF_NODE_HEADER_SIZE + cell_number * LEAF_NODE_CELL_SIZE;
}

void init_leaf(void* node){
    *leaf_node_number_cells(node) = 0;
}

uint32_t* get_leaf_key(void* node,uint32_t cell_number){
    return get_leaf_node_cell(node, cell_num);
}

void* get_leaf_node_value(void* node, uint32_t cell_number){
    return get_leaf_node_cell(node, cell_num);
}