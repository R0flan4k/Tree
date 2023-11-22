#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


int main(void)
{
    Tree tree = {};
    Error_t errors = 0;

    op_new_tree(&tree, "abobus?");

    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_LEFT, "abo");

    if (errors)
    {
        tree_dump(&tree);
        return errors;
    }

    tree_dump(&tree);

    size_t j = 0;
    for (TreeNode * i = tree.root; j < 10; i = i->right, j++)
    {
        if (errors = tree_insert(&tree, i, TREE_NODE_BRANCH_RIGHT, "lol"))
        {
            return errors;
        }
        tree_dump(&tree);
    }

    op_delete_tree(&tree);

    return 0;
}
