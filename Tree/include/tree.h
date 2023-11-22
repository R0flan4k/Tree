#ifndef TREE_H
    #define TREE_H

    #include <stdio.h>

    const size_t MAX_STR_SIZE = 64;

    typedef char Elem_t [MAX_STR_SIZE];
    typedef int Error_t;

    #define ELEM_SPEC "%s"
    #define tree_dump(tree) tree_dump_iternal((tree), #tree, __func__, __LINE__, __FILE__)

    enum TreeErrorsMasks {
        TREE_ERRORS_CANT_ALLOCATE_MEMORY = 1 << 0,
        TREE_ERRORS_ALREADY_DESTRUCTED   = 1 << 1,
        TREE_ERRORS_INVALID_ROOT         = 1 << 2,
        TREE_ERRORS_INVALID_SIZE         = 1 << 3,
        TREE_ERRORS_INVALID_NODE         = 1 << 4,
        TREE_ERRORS_ALREADY_CONSTRUCTED  = 1 << 5,
    };

    enum TreeNodeBranches {
        TREE_NODE_BRANCH_LEFT,
        TREE_NODE_BRANCH_RIGHT,
    };

    struct TreeNode {
        Elem_t value;
        TreeNode * right;
        TreeNode * left;
    };

    struct Tree {
        TreeNode * root;
        size_t size;
    };

    extern const char * TREE_DUMP_FILE_NAME;

    Error_t op_new_tree(Tree * tree);
    Error_t op_delete_tree(Tree * tree);
    Error_t tree_vtor(Tree * tree);
    Error_t tree_insert(Tree * tree, TreeNode * node, TreeNodeBranches mode, const Elem_t value);
    Error_t tree_delete_branch(Tree * tree, TreeNode * node);
    void tree_dump_iternal(const Tree * tree,
                              const char * tree_name, const char * func,
                              const int line, const char * file);

#endif // TREE_H