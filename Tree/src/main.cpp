#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


int main(void)
{
    Tree tree = {};
    TError_t errors = 0;

    op_new_tree(&tree, "abobus");

    tree_text_dump(&tree);
    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_LEFT, "228");
    errors = tree_insert(&tree, tree.root->left, TREE_NODE_BRANCH_LEFT, "228");
    errors = tree_insert(&tree, tree.root->left, TREE_NODE_BRANCH_RIGHT, "228");
    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_RIGHT, "228");
    tree_text_dump(&tree);

    if (errors)
    {
        tree_dump(&tree);
        return errors;
    }

    tree_dump(&tree);

    op_delete_tree(&tree);

    return 0;
}
