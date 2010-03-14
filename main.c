/*
 * Branden Rolston <brolston@gmail.com>
 */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"redblacktree.h"

int main()
{
    char cmd = '\0';
    int value;
    char buffer[1000] = "";
    RedBlackTree tree;

    rb_init(&tree);
    while(toupper(cmd) != 'Q')
    {
        scanf("%c %d", &cmd, &value);
        
        switch(toupper(cmd))
        {
            case 'I':
                printf("inserting %d\n", value);
                tree.insert(&tree, value);
                break;
            case 'R':
                printf("removing %d\n", value);
                tree.remove(&tree, value);
                break;
            case 'S':
                printf("searching for %d\n", value);
                if (tree.search(&tree, value))
                {
                    printf("found\n");
                }
                else
                {
                    printf("not found\n");
                }
                break;
            case 'P':
                printf("%s\n", tree.print(&tree, buffer));
                break;
        }
    }
    rb_free(&tree);
    return 0;
}
