#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

const int estimated_word_length = 8;
const int list_load = 100;
size_t GetFileSize (char *file_name);
void FillTable (List_t *table, char *file_name);
int FindNextSimple (int n);


int main(int argc, char **argv)
{
    assert(argv[1]);
    if (!argv[1]) {
        printf("Введите имя файла, слова которого будем хешировать!\n");
        return 0;
    }
    size_t file_size = GetFileSize(argv[1]);
    int table_size = FindNextSimple(file_size / (estimated_word_length * list_load));
    List_t* table = new List_t[table_size];

    FillTable(table, argv[1]);
    return 0;
}

int FindNextSimple (int n)
{
    
    return 0;
}
void FillTable (List_t *table, char *file_name)
{

}

size_t GetFileSize (char *file_name)
{
    assert(file_name);
    FILE *in = fopen(file_name, "r");
    fseek(in, SEEK_SET, SEEK_END);
    size_t ans = ftell(in);
    fclose(in);
    return ans;
}