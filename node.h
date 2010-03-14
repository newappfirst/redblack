/*
 * Branden Rolston <brolston@gmail.com>
 */

#include<stdio.h>
#include<stdlib.h>

#define NODE_RED 0
#define NODE_BLACK 1

#ifndef NODE_H
#define NODE_H

typedef struct _node
{
    struct _node * parent;
    struct _node * left;
    struct _node * right;
    int color;
    int value;
} Node;

Node * node_grandparent(Node *);
Node * node_uncle(Node *);
Node * node_sibling(Node *);

#endif
