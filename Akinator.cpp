#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <windows.h>
#include <locale.h>

//#include "Tree.h"
#include "Akinator.h"



/*
void Menu()
{
    printf("Hello, my dear friend. Today we're going to play Akinator. Choose what to do:");
    
    int regime = 0;

    scanf("%d", &regime);
    while ((0 > regime || regime < ))
    {
        
    }
    
    
}
*/



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

void Akinator_Tree_Ctor(Tree* tree, Tree_Text* text)
{
    int line = 0;

    //if (sscanf(text->lines[line].line_begin, "[") == 0)
    if (text->lines[line].line_begin[0] != '[')
    {
        printf("Broken tree file. gg\n");
        exit(666);
    }

    line++;

    char root_value[text->lines[line].lenth+1] = {};
    strncpy(root_value, text->lines[line].line_begin+1, text->lines[line].lenth-2);
    TreeCtor(tree, root_value); printf("HEPL\n");
    printf("HEPL\n");
    line++;

    printf("I'm here\n");
}

void Add_Nodes_to_Acinator_Tree(Tree* tree, Tree_Text* text, Node* node, int* line)
{
    if (sscanf(text->lines[*line].line_begin, "[") == 0)
    {
        printf("Broken tree file. gg\n");
        exit(666);
    }
    *(line)++;

    char add_value[text->lines[*line].lenth-1] = {};
    strncpy(add_value, text->lines[*line].line_begin + 1, text->lines[*line].lenth-2);
    Insert_Node(tree, node, LEFT_CHILD, add_value);
    *(line)++;

    if (text->lines[*line].line_begin[0] == '?')
    { 
        Add_Nodes_to_Acinator_Tree(tree, text, node->left, line);
        Add_Nodes_to_Acinator_Tree(tree, text, node->right, line);
    }

    if (sscanf(text->lines[*line].line_begin, "]") == 0)
    {
        printf("Broken tree file. gg\n");
        exit(666);
    }
    *(line)++;

    return;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
    FILE* file = fopen(source_file_name, "r");
    assert(file != NULL);

    Tree_Text text = {};

    text.file_size = File_Size(file);

    text.buffer = (char* ) calloc(text.file_size + 1, sizeof(char));
    text.buffer[text.file_size] = '\0';

    text.file_size = fread(text.buffer, sizeof(char), text.file_size, file);

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

    return 0;
}
