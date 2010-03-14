/*
 * Branden Rolston <brolston@gmail.com>
 */

#include<stdio.h>
#include<stdlib.h>
#include"node.h"

Node * node_grandparent(Node * n)
{
    if (n && n->parent)
    {
        return n->parent->parent;
    }
    return NULL;
}

Node * node_uncle(Node * n)
{
    Node * g = node_grandparent(n);
    if (!g)
    {
        return NULL;
    }
    if (n->parent == g->left)
    {
        return g->right;
    }
    else
    {
        return g->left;
    }
}


Node * node_sibling(Node * n)
{
    if (!(n->parent))
    {
        return NULL;
    }
    if (n->parent->left == n)
    {
        return n->parent->right;
    }
    else
    {
        return n->parent->left;
    }
}
