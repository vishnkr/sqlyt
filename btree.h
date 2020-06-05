#include <stdio.h>
#include <stdint.h>


typedef enum {
    INTERNAL_NODE,
    LEAF_NODE
} NodeType;


//regular NODE HEADER
const uint32_t NODETYPE_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = IS_ROOT_SIZE;
const uint32_t POINTER_PARENT_SIZE = sizeof(uint32_t);
const uint32_t POINTER_PARENT_OFFSET = IS_ROOT_OFFSET+IS_ROOT_SIZE;
const uint32_t HEADER_NODE_SIZE = NODETYPE_SIZE+IS_ROOT_SIZE+POINTER_PARENT_SIZE;

//leaf node header
const uint32_t 