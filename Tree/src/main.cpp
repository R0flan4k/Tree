#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


int main(void)
{
    Tree tree = {};
    Error_t errors = 0;

    op_new_tree(&tree);

    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_LEFT, "abo");
    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_RIGHT, "lol");

    if (errors)
    {
        tree_dump(&tree);
        return errors;
    }

    tree_dump(&tree);

    op_delete_tree(&tree);

    return 0;
}
