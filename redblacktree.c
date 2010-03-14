/*
 * Branden Rolston <brolston@gmail.com>
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"redblacktree.h"
#include"node.h"

/* Properties:
 * 1. A node is either red or black.
 * 2. The root is black.
 * 3. All leaves are black. (null children are counted as black nodes)
 * 4. Both children of every red node are black.
 * 5. Every simple path from a given node to any of its descendant leaves 
 *     contains the same number of black nodes.
 */

static char * BLACK_CLR = "[1;37;48m";
static char * RED_CLR = "[1;31;48m";
static char * RESET_CLR = "[0;0;0m";

Node * traverse(Node * root, int value)
{
    Node * curr = root;
    Node * prev = NULL;
    while (curr && curr->value != value)
    {
        prev = curr;
        if (value < curr->value)
        {
            curr = curr->left;
        }
        else if (value > curr->value)
        {
            curr = curr->right;
        }
    }
    if (curr == NULL)
    {
        return prev;
    }
    return curr;
}

void chop(Node * n)
{
    if (n)
    {
        chop(n->left);
        chop(n->right);
        free(n);
    }
}

int printtree(Node * n, int level, char * buffer)
{
    int offset = 0;
    char * color;
    int i;
    if (n->color == NODE_BLACK)
    {
        color = BLACK_CLR;
    }
    else
    {
        color = RED_CLR;
    }

    /* print right subtree */
    if (n->right)
    {
        offset += printtree(n->right, level + 1, buffer);
    }

    /* print current node */
    for (i = 0; i < level; ++i)
    {
        offset += sprintf(buffer + offset, "\t");
    }
    offset += sprintf(buffer + offset, "%c%s%d%c%s\n", 27, color, n->value, 
            27, RESET_CLR);

    /* print left subtree */
    if (n->left)
    {
        offset += printtree(n->left, level + 1, buffer + offset);
    }

    return offset;
}

void rb_init(RedBlackTree * tree)
{
    tree->root = NULL;
    tree->insert = &rb_insert;
    tree->remove = &rb_remove;
    tree->search = &rb_search;
    tree->print = &rb_print;
}

void rb_free(RedBlackTree * tree)
{
    chop(tree->root);

}

Node * rotate_left(Node * root)
{
    Node * pivot = root->right;
    Node * rparent = root->parent;

    /* set root to be parent of pivot's child */
    root->right = pivot->left;
    if (pivot->left)
    {
        pivot->left->parent = root;
    }

    /* set pivot to be parent of root */
    pivot->left = root;
    root->parent = pivot;

    /* set rparent to be parent of pivot */
    pivot->parent = rparent;
    if (rparent && rparent->left == root)
    {
        rparent->left = pivot;
    }
    if (rparent && rparent->right == root)
    {
        rparent->right = pivot;
    }

    return pivot;
}

Node * rotate_right(Node * root)
{
    Node * pivot = root->left;
    Node * rparent = root->parent;

    /* set root to be parent of pivot's child */
    root->left = pivot->right;
    if (pivot->right)
    {
        pivot->right->parent = root;
    }

    /* set pivot to be parent of root */
    pivot->right = root;
    root->parent = pivot;

    /* set rparent to be parent of pivot */
    pivot->parent = rparent;
    if (rparent && rparent->left == root)
    {
        rparent->left = pivot;
    }
    if (rparent && rparent->right == root)
    {
        rparent->right = pivot;
    }

    return pivot;
}

void rebalance_on_insert(Node ** root, Node * n)
{
    /* node n is assumed to be red */
    Node * p = n->parent;
    Node * g = node_grandparent(n);
    Node * u = node_uncle(n);
    if (!p) /* n is root, so must be black (property 2) */
    {
        n->color = NODE_BLACK;
    }
    else if (p->color == NODE_RED)
    { /* n's color violates property 4 */
        if (u && u->color == NODE_RED)
        {
            p->color = NODE_BLACK;
            u->color = NODE_BLACK;
            g->color = NODE_RED;
            rebalance_on_insert(root, g); /* g may now violate property 4 */
        }
        else 
        {
            /* if n is "inside" the tree, rotate it toward the outside.
             * this will swap the roles of n and p.
             */
            if (n == p->right && p == g->left)
            {
                rotate_left(p);
                /* n is now parent of p; swap for clarity */
                n = p;
                p = n->parent;
            }
            else if (n == p->left && (p == g->right))
            {
                rotate_right(p);
                /* n is now parent of p; swap for clarity */
                n = p;
                p = n->parent;
            }

            /* now that both red nodes are on the outer branches, continue */
            /* n and p are both red, violating property 4 */
            p->color = NODE_BLACK;
            g->color = NODE_RED;
            if (n == p->left && p == g->left)
            {
                Node * pivot = rotate_right(g);
                if (*root == g)
                {
                    *root = pivot;
                }
            }
            else /* n == p->right && p == g->right */
            {
                Node * pivot = rotate_left(g);
                if (*root == g)
                {
                    *root = pivot;
                }
            }
        }
    }
}

void rb_insert(RedBlackTree * tree, int value)
{
    Node * curr = traverse(tree->root, value);
    if (curr && value == curr->value)
    {
        return; /* no duplicates */
    }
    else
    {
        /* create new node */
        Node * new = malloc(sizeof(Node));
        if (new == NULL)
        {
            exit(1);
        }
        new->parent = curr;
        new->left = NULL;
        new->right = NULL;
        new->color = NODE_RED;
        new->value = value;

        /* place new node on tree */
        if (!curr)
        {
            tree->root = new;
        }
        else if (new->value < curr->value)
        {
            curr->left = new;
        }
        else if (new->value > curr->value)
        {
            curr->right = new;
        }

        rebalance_on_insert(&(tree->root), new);
    }
}

void replace_node(Node * n, Node * new)
{
    if (new)
    {
        new->parent = n->parent;
    }
    if (n->parent && n->parent->left == n)
    {
        n->parent->left = new;
    }
    else if (n->parent)
    {
        n->parent->right = new;
    }
}

void rebalance_on_remove(Node ** root, Node * n)
{
    /* n may be a leaf node aka NULL */
    Node * s = node_sibling(n);
    if (!(n->parent))
    {
        return;
    }

    if (s->color == NODE_RED)
    {
        n->parent->color = NODE_RED;
        s->color = NODE_BLACK;
        if (n == n->parent->left)
        {
            rotate_left(n->parent);
        }
        else
        {
            rotate_right(n->parent);
        }
        s = node_sibling(n); /* need to update because of rotation */
    }

    if (n->parent->color == NODE_BLACK && 
        s->color == NODE_BLACK &&
        (!(s->left) || s->left->color == NODE_BLACK) &&
        (!(s->right) || s->right->color == NODE_BLACK))
    {
        s->color = NODE_RED;
        rebalance_on_remove(root, n->parent);
    }

    else if (n->parent->color == NODE_RED &&
             s->color == NODE_BLACK &&
             (!(s->left) || s->left->color == NODE_BLACK) &&
             (!(s->right) || s->right->color == NODE_BLACK))
    {
        s->color = NODE_RED;
        n->parent->color = NODE_BLACK;
    }

    else if (s->color == NODE_BLACK)
    {
        if (n == n->parent->left &&
            (!(s->right) || s->right->color == NODE_BLACK) &&
            (s->left && s->left->color == NODE_RED))
        {
            s->color = NODE_RED;
            s->left->color = NODE_BLACK;
            rotate_right(s);
        }
        else if (n == n->parent->right &&
            (!(s->left) || s->left->color == NODE_BLACK) &&
            (s->right && s->right->color == NODE_RED))
        {
            s->color = NODE_RED;
            s->right->color = NODE_BLACK;
            rotate_left(s);
        }
        s = node_sibling(n); /* need to update because of rotation */

        /* s is black and s's right child is red */
        /* exchange s's and p's colors (s is known to be black) */
        s->color = n->parent->color;
        n->parent->color = NODE_BLACK;

        if (n == n->parent->left)
        {
            if (s->right)
            {
                s->right->color = NODE_BLACK;
            }
            rotate_left(n->parent);
        }
        else
        {
            if (s->left)
            {
                s->left->color = NODE_BLACK;
            }
            rotate_right(n->parent);
        }
        if (*root == n->parent) /* root might've changed from last rotation */
        {
            *root = n->parent->parent;
        }
    }

}

/* should only be called on a node with at most one nonleaf child */
Node * remove_node(Node ** root, Node * n)
{
    Node * child;
    if (n->left)
    {
        child = n->left;
    }
    else
    {
        child = n->right;
    }
    if (n->color == NODE_RED || (child && child->color == NODE_RED) || 
            !(n->parent)) 
    {
        /* delete and replace with child, repaint it black */
        replace_node(n, child);
        if (child)
        {
            /* if n is red then this is already true
             * if child is red then this satisfies property 5
             * if n is root then this satisfies property 2
             */
            child->color = NODE_BLACK;
        }
        free(n);
    }
    else /* node black, child black */
    {
        rebalance_on_remove(root, n);
        if (n->parent->left == n)
        {
            n->parent->left = NULL;
        }
        else
        {
            n->parent->right = NULL;
        }
        free(n);
    }

    return child;
}

void rb_remove(RedBlackTree * tree, int value)
{
    Node * curr = traverse(tree->root, value);
    Node * child;

    if (!curr || value != curr->value)
    {
        return; /* value is not in the tree */
    }
    if (curr->left)
    {
        /* replace with largest nonleaf child from left subtree */
        child = traverse(curr->left, value);
        curr->value = child->value;
        Node * replacement = remove_node(&(tree->root), child);
    }
    else if (curr->right)
    {
        /* replace with smallest nonleaf child from right subtree */
        child = traverse(curr->right, value);
        curr->value = child->value;
        Node * replacement = remove_node(&(tree->root), child);
    }
    else /* node has no nonleaf children */
    {
        if (tree->root = curr)
        {
            tree->root = NULL;
        }
        remove_node(&(tree->root), curr);
    }
}


int rb_search(RedBlackTree * tree, int value)
{
    Node * n = traverse(tree->root, value);
    if (n && n->value == value)
    {
        return 1;
    }
    return 0;
}

char * rb_print(RedBlackTree * tree, char * buffer)
{
    if (tree->root)
    {
        printtree(tree->root, 0, buffer);
    }
    else
    {
        strcpy(buffer, "");
    }
    return buffer;
}
