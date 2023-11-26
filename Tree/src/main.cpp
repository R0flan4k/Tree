#include <stdio.h>
#include <stdlib.h>

#include "tree.h"


int main(void)
{
    Tree tree = {};
    TError_t errors = 0;

    op_new_tree(&tree, NULL);

    char string[228] = "abobus?";

    tree_text_dump(&tree);
    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_LEFT, NULL);
    errors = tree_insert(&tree, tree.root->left, TREE_NODE_BRANCH_LEFT, NULL);
    errors = tree_insert(&tree, tree.root->left, TREE_NODE_BRANCH_RIGHT, string);
    errors = tree_insert(&tree, tree.root, TREE_NODE_BRANCH_RIGHT, NULL);
    tree_text_dump(&tree);

    printf("%p\n\n", tree.root->left->right->value);

    if (errors)
    {
        tree_dump(&tree);
        return errors;
    }

    tree_dump(&tree);

    op_delete_tree(&tree);
    printf("Program successfully completed.\n");

    return 0;
}
