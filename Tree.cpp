#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "TXLib.h"

//#include "Tree.h"
#include "Akinator.h"
#include "html.h"


int cell = 0;
FILE* dump_file = NULL;
const char* dump_file_name = "debug\\dump.dot";

void TreeCtor(Tree* tree, char* root_node_value)
{
    printf("I'm in TreeCtor\n");
    tree->root = NodeCtor(root_node_value);
    tree->size = 1;
    Tree_Dump(tree);
    printf("I've finished TreeCtor\n");
}

Node* NodeCtor(char* value, Node* parent, Node* left, Node* right)
{
    Node* new_node = (Node*) calloc(1, sizeof(Node));
    assert(new_node != nullptr);


    new_node->left = left;
    new_node->parent = parent;
    new_node->right = right;

    new_node->value = (char*) calloc(strlen(value)+1, sizeof(char));
    strcpy(new_node->value, value);

    new_node->cell_num = cell;
    cell++;

    return new_node;
}

void Insert_Node(Tree* tree, Node* node, Child_Num child, char* value)
{
    if (child == LEFT_CHILD)
    {
        if (node->left != NULL)
        {
            printf("You debil. Add children to free nodes\n");
            return;
        }

        node->left = NodeCtor(value, node);
    }

    if (child == RIGHT_CHILD)
    {
        if (node->right != NULL)
        {
            printf("You debil. Add children to free nodes\n");
            return;
        }

        node->right = NodeCtor(value, node);
    }

    tree->size++;
    Tree_Dump(tree);
}

void Delete_Node(Tree* tree, Node* node)
{
    if (node == tree->root || node->left != NULL || node->right != NULL)
    {
        printf("You debil. Impossible to delete root or not free nodes\n");
        return;
    }

    if (node->parent->left == node) node->parent->left = NULL;
    if (node->parent->right == node) node->parent->right = NULL;

    free(node);
    Tree_Dump(tree);
    
}

void Recursive_Node_Dump(Node* node)
{
    char* color = NULL;
    char* edge_color = NULL;
    if (node->left == NULL && node->right == NULL) color = "darkolivegreen1";
    else  color = "cyan1";

    if (node->parent->left == node) edge_color = "firebrick4";
    else edge_color = "darkgreen";

    DUMP("cell_%d [label = \"%s\"\n style = filled\n fillcolor = %s \n]\n", node->cell_num, node->value, color);
    DUMP("cell_%d->cell_%d[color = %s]\n", node->parent->cell_num, node->cell_num, edge_color);

    if (node->left != NULL) Recursive_Node_Dump(node->left);
    if (node->right != NULL) Recursive_Node_Dump(node->right);
}

void Tree_Dump(Tree* tree)
{
    dump_file = fopen(dump_file_name, "w"); 
    printf("fgfdgdfgdfgdfgdfgdfgdfgdfgdfgdfgdgfdgdfg\n");                            
    DUMP("digraph{\n");                                                 
    DUMP("nodesep = 2;\n");                                             
    DUMP("splines = \"ortho\";\n");                                     
    DUMP("node [ shape=record, style = rounded, fontsize = 30];\n");  

    DUMP("cell_0 [label = \"%s\"\n style = filled\n fillcolor = violet ]\n", tree->root->value);

    
    if (tree->root->left != NULL) Recursive_Node_Dump(tree->root->left);
    if (tree->root->right != NULL) Recursive_Node_Dump(tree->root->right);

    DUMP("}");

    fclose(dump_file);

    char command[100] = "dot -Tpng debug\\dump.dot -Gcharset=latin1 -o ";
    char name[20] = "debug\\dump";
    char num[10] = "";

    itoa(image_num, num, 10);
    strcat(num, png);
    strcat(name, num);
    strcat(command, name);

    image_num++;

    system(command);

    Add_To_HTML(name);

    printf("I'm dfghggiojdgrfdg\n");
}

void Add_To_HTML(char* image)
{
    fprintf(HTML, "<img src = \"%s\"><hr>", image);
    fflush(HTML);
}
