#ifndef FFF
#define FFF

#include <stdio.h>


enum work_regimes
{
    guess = 1,
    add   = 2,

};

struct line 
{
    char* line_begin;
    int lenth;
};

struct Tree_Text
{
    char * buffer;
    line* lines;
    size_t file_size;
    size_t lines_amount;
};

struct Node
{
    char* value;

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
                   

#define FINISH_DUMP()        

#define DUMP(text...)\
    fprintf(dump_file, text);    


const int NODE_NAME_LEN = 50;


//#ifndef source_file_name 
    #define source_file_name "source.txt"
//#endif

void Menu(Tree* tree);
void Guess(Node* node);
void Naidetsia_Vse___So_Vremenem(Node* node);
int File_Size(FILE *stream);
void Distinguish_Strings(Tree_Text* text);
void Open_Source_File();
void Open_Output_File();
void Akinator_Tree_Ctor(Tree* tree, Tree_Text* text);
void Add_Nodes_to_Acinator_Tree(Tree* tree, Tree_Text* text, Node* node, int* line);
void Exit_Bad_Tree(int line);
void Out_Base(int tabs, Node* node);


void TreeCtor(Tree* tree, char* root_node_value);
Node* NodeCtor(char* value, Node* parent = NULL, Node* left = NULL, Node* right = NULL);
void Insert_Node(Tree* tree, Node* node, Child_Num child, char* value);
void Delete_Node(Tree* tree, Node* node);
void Recursive_Node_Dump(Node* node);
void Tree_Dump(Tree* tree);
void Add_To_HTML(char* image);

#endif