#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>
#include <locale.h>

//#include "Tree.h"
#include "Akinator.h"
#include "config.h"
#include "stack.h"

FILE* source_file = NULL;
FILE* out_file = NULL;
const int SOURCE_FILE_NAME_LENDTH = 100;
const int NODE_NAME_LENDTH = 30;


void Menu(Tree* tree)
{
    printf("Hello! I'm your only subscriber. Let's play phistech akinator\n"
            "we have some regimes:\n"
            "1. guessing \n2. Find definition \n3. Comparison \n4. Tree output as a png \n5.Out\n");

    int regime = -1;
    int g = 0;
    while ((regime < 1) || (regime > 5))
    {
        printf("What are we going to do?\n>");
        g = scanf("%d", &regime);
        if (g == 0) getchar();
    }
    getchar();

    switch (regime)
    {
    case 1:
    {
        Guess(tree->root);
        Tree_Dump(tree);
        break;
    }

    case 2:
    {
        printf("Whose definition I need to say?\n");
        char find_node_name[NODE_NAME_LENDTH] = {};
        Get_name(find_node_name);
        Node* find_node = Find_Node(tree->root, find_node_name);
        if (find_node == NULL) printf("Sorry, I don't now it\n");
        else Definition(find_node, tree);
        break;
    }
    
    default:
        break;
    }
}

void Guess(Node* node)
{
    printf("Is it %s?\n", node->value);
    char c = 'a';

    while ((c != 'y') && (c != 'n'))
    {
        printf("[y/n]>");
        scanf("%c", &c);
        getchar();
    }
    
    if (node->left == NULL)
    {
        if (c == 'y')
        {
            printf("You see! I know everything!\n");
        }
        else
        {
            printf("Well, to be honest, I don't know everything\n");
            Naidetsia_Vse___So_Vremenem(node);
        }
    }

    else
    {
        if (c == 'y') Guess(node->right);
        else Guess(node->left);
    }
}

void Naidetsia_Vse___So_Vremenem(Node* node)
{
    printf("So, what was it?\n>");

    char new_list_node_name[NODE_NAME_LENDTH] = {};
    Get_name(new_list_node_name);

    printf("And what is difference with %s?\n", node->value);

    char diff_node_name[NODE_NAME_LEN] = {};
    Get_name(diff_node_name);
    
    Node* diff_node = NodeCtor(diff_node_name, node->parent);
    if (node->parent->left == node) node->parent->left = diff_node;
    else node->parent->right = diff_node;
    Node* new_list_node = NodeCtor(new_list_node_name, diff_node);
    node->parent = diff_node;

    printf("%s %s (1) or %s %s (2)?\n", node->value, diff_node_name, new_list_node_name, diff_node_name);

    int list = 0;
    while ((list < 1) || (list > 2))
    {
        printf("[1/2]\n>");
        scanf("%d", &list);
    }

    if (list == 1)
    {
        diff_node->right = node;
        diff_node->left = new_list_node;
    }
    else
    {
        diff_node->left = node;
        diff_node->right = new_list_node;
    }

}

void Get_name(char* s)
{
    printf("I'm in get name\n");
    int i = 0;
    char a = getchar();
    printf("%c\n", a);
    if (a == '\n') a = getchar();
    while ((a != '\n') && (a != '\0') && (i < NODE_NAME_LENDTH))
    {
        printf("%c\n", a);
        s[i] = a;
        a = getchar();
        i++;
    }
    printf("I'm here\n");
    
}

Node* Find_Node(Node* node, char* s)
{
    printf("I'm trying to find node. Now value is %s\n", node->value);
    if (node->left == NULL)
    {
        if (strcmp(s, node->value) == 0) return node;
        else return NULL;
    }

    else
    {
        Node* ret_node = Find_Node(node->left, s);
        if (ret_node == NULL) ret_node = Find_Node(node->right, s);
        return ret_node;
    }
}

void Definition(Node* node, Tree* tree)
{
    printf("%s is ", node->value);
    Stack stk = {};
    StackCtor(&stk, 5);

    char* s = "not";

    while (node != tree->root)
    {
        StackPush(&stk, node->parent->value);
        if (node->parent->left == node) StackPush(&stk, s);
        node = node->parent;
    }
    //StackPush(&stk, node->value);
    
    int gg = stk.size;
    for (int i = 0; i < gg; i++)
    {
        char* print = StackPop(&stk);
        printf("%s", print);
        if (print == s) printf(" ");
        else printf(", ");
    }

    printf("\n");
}

int File_Size(FILE *stream)
{
    struct stat buff = {};
        
    fstat(fileno(stream), &buff);
    
    int size = buff.st_size;

    return size;
}


void Distinguish_Strings(Tree_Text* text)
{
    text->lines = (line*) calloc(text->lines_amount, sizeof(line));
    text->lines[0].line_begin = text->buffer;
    int line = 1;
    int i = 0;
    while(i < text->file_size)
    {
        if (text->buffer[i] == '\n')
        {
            text->lines[line-1].lenth = text->buffer + i - text->lines[line-1].line_begin;
            i++;
            text->buffer[i-1] = '\0';
            while ((i < text->file_size) && (text->buffer[i] == '\n' || text->buffer[i] == '\t' || text->buffer[i] == ' ')) 
            {
                i++;
            }
            text->lines[line].line_begin = text->buffer + i;
            line++;
        }
        i++;
    }
    text->lines[line-1].lenth = text->buffer + i - text->lines[line-1].line_begin;
    /*
    for (int j = 0; j < text->lines_amount; j++)
    {
        printf(text->lines[j].line_begin);
        printf("    lenth = %d\n", text->lines[j].lenth);
    }
    */
}


void Open_Source_File()
{
    char file_name[SOURCE_FILE_NAME_LENDTH] = {};
    while (source_file == NULL)
    {
        printf("Write the name of source file, pls: ");
        scanf("%s", file_name);
        source_file = fopen(file_name, "r");
    } 
}

void Open_Output_File()
{
    char* file_name = "base.txt";
    out_file = fopen(file_name, "w");
}

void Akinator_Tree_Ctor(Tree* tree, Tree_Text* text)
{
    printf("Wait a bit. Openning tha akinator base\n");
    int line = 0;
    if (text->lines[line].line_begin[0] == '[') line++;

    char root_value[text->lines[line].lenth] = {};
    strncpy(root_value, text->lines[line].line_begin + 1, text->lines[line].lenth - 2);
    TreeCtor(tree, root_value);
    line++;

    Add_Nodes_to_Acinator_Tree(tree, text, tree->root, &line);

    printf("I have finished! Thanks for waiting.\n");
}

void Add_Nodes_to_Acinator_Tree(Tree* tree, Tree_Text* text, Node* node, int* line)
{
    if (text->lines[*line].line_begin[0] != '[') Exit_Bad_Tree(*line);
    (*line)++;

    bool not_list = text->lines[*line].line_begin[0] == '?';

    char add_value_left[text->lines[*line].lenth] = {};
    strncpy(add_value_left, text->lines[*line].line_begin + 1, text->lines[*line].lenth-2);
    Insert_Node(tree, node, RIGHT_CHILD, add_value_left);
    (*line)++;

    if (not_list) Add_Nodes_to_Acinator_Tree(tree, text, node->right, line);

    if (text->lines[*line].line_begin[0] != ']') Exit_Bad_Tree(*line);
    (*line)++;

    if (text->lines[*line].line_begin[0] != '[') Exit_Bad_Tree(*line);
    (*line)++;
    
    not_list = text->lines[*line].line_begin[0] == '?';

    char add_value_right[text->lines[*line].lenth] = {};
    strncpy(add_value_right, text->lines[*line].line_begin + 1, text->lines[*line].lenth-2);
    Insert_Node(tree, node, LEFT_CHILD, add_value_right);
    (*line)++;

    if (not_list) Add_Nodes_to_Acinator_Tree(tree, text, node->left, line);

    if (text->lines[*line].line_begin[0] != ']') Exit_Bad_Tree(*line);
    (*line)++;

    return;
}

void Exit_Bad_Tree(int line)
{
    printf("Broken tree file. gg line %d\n", line);
    exit(666);
}

void Out_Base(int tabs, Node* node)
{
    for (int i = 0; i < tabs; i++) fprintf(out_file, "\t");
    
    int value_len = strlen(node->value);
    char node_name[value_len + 3] = {};

    if (node->left != NULL)
    {
        node_name[0] = '?';
        strcpy(node_name + 1, node->value);
        node_name[value_len + 1] = '?';
    }
    else
    {
        node_name[0] = '\'';
        strcpy(node_name + 1, node->value);
        node_name[value_len + 1] = '\'';
    }

    fprintf(out_file, "%s\n", node_name);

    if (node->left != NULL)
    {
        for (int i = 0; i < tabs; i++) fprintf(out_file, "\t");
        fprintf(out_file, "[\n");
        Out_Base(tabs + 1, node->right);
        for (int i = 0; i < tabs; i++) fprintf(out_file, "\t");
        fprintf(out_file, "]\n");

        for (int i = 0; i < tabs; i++) fprintf(out_file, "\t");
        fprintf(out_file, "[\n");
        Out_Base(tabs + 1, node->left);
        for (int i = 0; i < tabs; i++) fprintf(out_file, "\t");
        fprintf(out_file, "]\n");
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    Open_Source_File();
    Tree_Text text = {};

    text.file_size = File_Size(source_file);

    text.buffer = (char* ) calloc(text.file_size + 1, sizeof(char));
    text.buffer[text.file_size] = '\0';

    text.file_size = fread(text.buffer, sizeof(char), text.file_size, source_file);

    fclose(source_file);

    int i = 0;
    while(i < text.file_size)
    {
        if (text.buffer[i] == '\n') 
        {
            text.lines_amount++;
            while (i < text.file_size && (text.buffer[i] == '\n')) i++;
            i--;
        }
        i++;
    }
    text.lines_amount++;

    Distinguish_Strings(&text);

    Tree tree = {};

    Akinator_Tree_Ctor(&tree, &text);

    Menu(&tree);

    Open_Output_File();
    Out_Base(0, tree.root);

    return 0;
}
