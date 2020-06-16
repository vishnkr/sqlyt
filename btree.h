#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef INCLUDE_BTREE_H
#define INCLUDE_BTREE_H


#define MAX_BTREE_KEYS 7 //m=8
#define MIN_BTREE_KEYS 4
typedef struct BtreeNode{
    bool isleaf;
    uint32_t key_count;
    uint32_t values[MAX_BTREE_KEYS];
    uint32_t keys[MAX_BTREE_KEYS];
    BtreeNode* children[MAX_BTREE_KEYS+1];
}BtreeNode;

BtreeNode* setup_btree(bool isleaf);

#endif