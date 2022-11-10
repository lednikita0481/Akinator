#ifndef H_TREE_INCLUDED
#define H_TREE_INCLUDED

#include <stdio.h>
typedef char* Value;

int cell = 0;

struct Node
{
    Value value;

    int cell_num;

    Node* right;
    Node* left;
    Node* parent;
};

struct Tree
{
    Node* root;
    size_t size;
};

enum Child_Num
{
    LEFT_CHILD,
    RIGHT_CHILD
};

enum Errors
{
    OK = 0,

    NULL_PTR        = 1 << 0,
    TREE_NO_ROOT    = 1 << 1,

};

const char* dump_file_name = "debug\\dump.dot";


FILE* dump_file = NULL;
                   

#define FINISH_DUMP()        

#define DUMP(text...)\
    fprintf(dump_file, text);    



void TreeCtor(Tree* tree, Value root_node_value);
Node* NodeCtor(Value value, Node* parent = NULL, Node* left = NULL, Node* right = NULL);
void Insert_Node(Tree* tree, Node* node, Child_Num child, Value value);
void Delete_Node(Tree* tree, Node* node);
void Recursive_Node_Dump(Node* node);
void Tree_Dump(Tree* tree);
void Add_To_HTML(char* image);


#endif