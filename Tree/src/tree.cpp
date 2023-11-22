#include <string.h>

#include "tree.h"
#include "my_assert.h"
#include "file_processing.h"
#include "strings.h"

const char * TREE_DUMP_FILE_NAME = "./graphviz/tree_dump";

const size_t TRASH_VALUE = 0xAB1BA5;
const Elem_t TRASH_ELEM_VALUE = "nillsukablyat";
const size_t BUFFER_SIZE = 256;

static void op_elem_assigment(Elem_t * dst, Elem_t const src);
static int op_elem_comparison(const Elem_t * elem1, const Elem_t * elem2);
static size_t tree_free(TreeNode * main_node);
static size_t tree_free_iternal(TreeNode * node, size_t * count);
static void print_tree_nodes(const TreeNode * node, FILE * fp);
static void print_tree_edges(const TreeNode * node, FILE * fp);
static Error_t tree_create_node(Tree * tree, TreeNode * * node_ptr);


static void op_elem_assigment(Elem_t * dst, Elem_t const src)
{
    MY_ASSERT(dst);

    strcpy(*dst, src);
}


static int op_elem_comparison(const Elem_t * elem1, const Elem_t * elem2)
{
    MY_ASSERT(elem1);
    MY_ASSERT(elem2);

    return strcmp(*elem1, *elem2);
}


Error_t op_new_tree(Tree * tree)
{
    MY_ASSERT(tree);

    Error_t errors = 0;

    if (!tree_vtor(tree))
    {
        errors |= TREE_ERRORS_ALREADY_CONSTRUCTED;
        return errors;
    }

    if ((tree->root = (TreeNode *) calloc(1, sizeof(Elem_t))) == NULL)
    {
        errors |= TREE_ERRORS_CANT_ALLOCATE_MEMORY;
        return errors;
    }
    tree->size = 1;

    tree->root->left = NULL;
    tree->root->right = NULL;
    op_elem_assigment(&tree->root->value, TRASH_ELEM_VALUE);

    return errors;
}


static size_t tree_free(TreeNode * main_node)
{
    MY_ASSERT(main_node);

    size_t count = 0;
    count = tree_free_iternal(main_node, &count);

    return count;
}


static size_t tree_free_iternal(TreeNode * node, size_t * count)
{
    MY_ASSERT(node);
    MY_ASSERT(count);

    if (node->left)
    {
        tree_free_iternal(node->left, count);
    }

    if (node->right)
    {
        tree_free_iternal(node->right, count);
    }

    free(node);
    count++;

    return *count;
}


Error_t op_delete_tree(Tree * tree)
{
    MY_ASSERT(tree);

    Error_t errors = 0;

    if (tree->root == NULL)
    {
        errors |= TREE_ERRORS_ALREADY_DESTRUCTED;
        return errors;
    }

    if (tree_free(tree->root) != tree->size)
        errors |= TREE_ERRORS_INVALID_SIZE;
    tree->size = TRASH_VALUE;

    return errors;
}


Error_t tree_vtor(Tree * tree)
{
    MY_ASSERT(tree);

    Error_t errors = 0;

    if (tree->size == 0)
        errors |= TREE_ERRORS_INVALID_SIZE;

    if (!tree->root)
        errors |= TREE_ERRORS_INVALID_ROOT;

    return errors;
}


static Error_t tree_create_node(Tree * tree, TreeNode * * node_ptr)
{
    MY_ASSERT(tree);
    MY_ASSERT(!*node_ptr);

    Error_t errors = 0;

    if (!(*node_ptr = (TreeNode *) calloc(1, sizeof(TreeNode))))
    {
        errors |= TREE_ERRORS_CANT_ALLOCATE_MEMORY;
        return errors;
    }

    (*node_ptr)->left = NULL;
    (*node_ptr)->right = NULL;
    op_elem_assigment(&(*node_ptr)->value, TRASH_ELEM_VALUE);

    tree->size++;

    return errors;
}


Error_t tree_insert(Tree * tree, TreeNode * node, TreeNodeBranches mode, const Elem_t value)
{
    MY_ASSERT(node);

    Error_t errors = 0;

    if (errors = tree_vtor(tree))
    {
        return errors;
    }

    switch (mode)
    {
        case TREE_NODE_BRANCH_LEFT:
            if (node->left)
            {
                errors |= TREE_ERRORS_INVALID_NODE;
                return errors;
            }

            if (errors = tree_create_node(tree, &node->left))
                return errors;
            op_elem_assigment(&node->left->value, value);

            break;

        case TREE_NODE_BRANCH_RIGHT:
            if (node->right)
            {
                errors |= TREE_ERRORS_INVALID_NODE;
                return errors;
            }

            if (errors = tree_create_node(tree, &node->right))
                return errors;
            op_elem_assigment(&node->right->value, value);

            break;

        default:
            MY_ASSERT(0 && "UNREACHABLE");
            break;
    }

    return errors;
}


Error_t tree_delete_branch(Tree * tree, TreeNode * node)
{
    MY_ASSERT(tree);
    MY_ASSERT(node);

    Error_t errors = 0;

    if (errors = tree_vtor(tree))
    {
        return errors;
    }

    size_t deleting_nodes_count = tree_free(node);

    if (tree->size < deleting_nodes_count)
    {
        errors |= TREE_ERRORS_INVALID_SIZE;
        return errors;
    }

    tree->size -= deleting_nodes_count;

    return errors;
}


void tree_dump_iternal(const Tree * tree,
                       const char * tree_name, const char * func,
                       const int line, const char * file)
{
    MY_ASSERT(tree);
    MY_ASSERT(tree_name);
    MY_ASSERT(func);
    MY_ASSERT(file);

    FILE * fp = NULL;

    char dot_file_name[64] = "";
    make_file_extension(dot_file_name, TREE_DUMP_FILE_NAME, ".dot");

    if (!(fp = file_open(dot_file_name, "wb")))
    {
        return;
    }

    fprintf(fp, "digraph G\n"
                "{\n"
                "    graph [dpi = 150]\n"
                "    ranksep = 0.6;\n"
                "    bgcolor = \"#f0faf0\"\n"
                "    splines = curved;\n"
                "    edge[minlen = 3];\n"
                "    node[shape = record, style = \"rounded\", color = \"#f58eb4\",\n"
                "        fixedsize = true, height = 1, width = 4, fontsize = 15];\n"
                "    {rank = min;\n"
                "        inv_min [style = invis];\n"
                "    }\n"
                "    {rank = max;\n"
                "        info_node [label = \" %s[%p] \\n from %s \\n %s:%d \", color = green];\n"
                "    }\n", tree_name, tree, file, func, line);

    if (tree->root)
    {
        print_tree_nodes(tree->root, fp);
        print_tree_edges(tree->root, fp);

        fprintf(fp, "info_node -> node%p [style = invis];\n", tree->root);
    }

    fprintf(fp, "}");

    fclose(fp);

    static size_t dumps_count = 0;
    char png_dump_file_name[BUFFER_SIZE] = "";
    char command_string[BUFFER_SIZE] = "";
    char extension_string[BUFFER_SIZE] = "";

    sprintf(extension_string, "%zd.png", dumps_count);
    make_file_extension(png_dump_file_name, TREE_DUMP_FILE_NAME, extension_string);
    sprintf(command_string, "dot %s -T png -o %s", TREE_DUMP_FILE_NAME, png_dump_file_name);
    system(command_string);

    dumps_count++;
}


static void print_tree_nodes(const TreeNode * node, FILE * fp)
{
    MY_ASSERT(node);

    // printf("printing: %p\n"
    //        "          %p\n"
    //        "          %p\n", node->value, node->left, node->right);
    fprintf(fp, "    node%p [ label = \"[%p] " ELEM_SPEC " | { <l> left = [%p] | right = [%p] }  }\" ]\n",
            node, node, node->value, node->left, node->right);
            // printf("printed\n");

    if (node->right)
    {
        // printf("%p\n", node->right);
        print_tree_nodes(node->right, fp);
    }

    if (node->left)
    {
        // printf("%p\n", node->left);
        print_tree_nodes(node->left, fp);
    }
}


static void print_tree_edges(const TreeNode * node, FILE * fp)
{
    MY_ASSERT(node);

    if (node->left)
    {
        fprintf(fp, "    node%p:<l> -> node%p;\n", node, node->left);
        print_tree_edges(node->left, fp);
    }

    if (node->right)
    {
        fprintf(fp, "    node%p:<r> -> node%p;\n", node, node->right);
        print_tree_edges(node->right, fp);
    }
}