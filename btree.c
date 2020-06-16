#include "btree.h"

//This file is supposed to have functions associated with B+ tree used for storing data and B-tree for indexing
/*
Node type: children type : min children : max children : ex(b=7)
root : records or internal or leaf: 1(if only node)/else 2: b-1/else b: 1-6/2-7
internal node: internal nodes or leaf: floor(b/2): b: 4-7
leaf node: records : floor(b/2): b: 4-7


For an order-m tree…	Internal Node	Leaf Node
Stores	keys and pointers to children	keys and values
Number of keys	up to m-1	as many as will fit
Number of pointers	number of keys + 1	none
Number of values	none	number of keys
Key purpose	used for routing paired with value
Stores values?	No	Yes

ex: when order = 3
up to 3 children per internal node
up to 2 keys per internal node
at least 2 children per internal node
at least 1 key per internal node
*/


BtreeNode* setup_btree(bool isleaf){
    BtreeNode* btree = (BtreeNode)malloc(sizeof(BtreeNode));
    btree->isleaf = isleaf;
    btree->keys = malloc()
    return btree;
}
