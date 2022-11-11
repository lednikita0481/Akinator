#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>
#include <locale.h>

//#include "Tree.h"
#include "Akinator.h"

FILE* source_file = NULL;
FILE* out_file = NULL;
const int SOURCE_FILE_NAME_LENDTH = 100;


void Menu(Tree* tree)
{
    printf("Hello! I'm your only subscriber. Let's play phistech akinator\n"
            "we have some regimes:\n"
            "1. guessing \n2. Find definition \n3. Comparison \n4. Tree output as a png \n5.Out\n");

    int regime = -1;
    while ((regime < 1) || (regime > 5))
    {
        printf("What are we going to do?\n>");
        scanf("%d", &regime);
    }

    switch (regime)
    {
    case 1:
        /* code */
        break;
    
    default:
        break;
    }
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
    printf("We have %d symbols\n\n\n", text->file_size);
    while(i < text->file_size)
    {
        if (text->buffer[i] == '\n')
        {
            text->lines[line-1].lenth = text->buffer + i - text->lines[line-1].line_begin;
            i++;
            text->buffer[i-1] = '\0';
            while ((i < text->file_size) && (text->buffer[i] == '\n' || text->buffer[i] == '\t' || text->buffer[i] == ' ')) 
            {
                printf("%d\n", i);
                i++;
            }
            text->lines[line].line_begin = text->buffer + i;
            line++;
        }
        i++;
    }
    text->lines[line-1].lenth = text->buffer + i - text->lines[line-1].line_begin;

    for (int j = 0; j < text->lines_amount; j++)
    {
        printf(text->lines[j].line_begin);
        printf("    lenth = %d\n", text->lines[j].lenth);
    }
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
    int line = 0;
    if (text->lines[line].line_begin[0] == '[') line++;

    char root_value[text->lines[line].lenth] = {};
    strncpy(root_value, text->lines[line].line_begin + 1, text->lines[line].lenth - 2);
    TreeCtor(tree, root_value); printf("HEPL\n");
    printf("HEPL\n");
    line++;

    printf("I'm here\n");

    Add_Nodes_to_Acinator_Tree(tree, text, tree->root, &line);
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
    Open_Output_File();
    Tree_Text text = {};

    text.file_size = File_Size(source_file);

    text.buffer = (char* ) calloc(text.file_size + 1, sizeof(char));
    text.buffer[text.file_size] = '\0';

    text.file_size = fread(text.buffer, sizeof(char), text.file_size, source_file);

    fclose(source_file);

    printf("%s", text.buffer);
    printf("\n\n\n\n\n\n");

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
    printf("%d\n", text.lines_amount);

    Distinguish_Strings(&text);

    Tree tree = {};

    Akinator_Tree_Ctor(&tree, &text);

    Out_Base(0, tree.root);

    return 0;
}
