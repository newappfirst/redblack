/*
 * Branden Rolston <brolston@gmail.com>
 */

#include<stdio.h>
#include<stdlib.h>
#include"node.h"

typedef struct _redblacktree
{
    Node * root;
    void (*insert) (struct _redblacktree *, int);
    void (*remove) (struct _redblacktree *, int);
    int (*search) (struct _redblacktree *, int);
    char * (*print) (struct _redblacktree *, char *);
} RedBlackTree;

void rb_init(RedBlackTree *);
void rb_free(RedBlackTree *);
void rb_insert(RedBlackTree *, int);
void rb_remove(RedBlackTree *, int);
int rb_search(RedBlackTree *, int);
char * rb_print(RedBlackTree *, char *);
