#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List.hpp"
#include "HashFunctions.hpp"

const int hash_size = 47;

size_t GetFileSize (const char *file_name);
void TestFunctions (const char *file_name);
void TestFunction (const char *file_name, int hash_function);
int InsertWord (List* table, char *word, int hash_function);
int PrintResults (List* table, char *sausage_data, int hash_function_id);

void TestFunctions (const char *file_name)
{
    for (int i = 0; i < functions_qty; ++i) {
        TestFunction(file_name, i);
    }
}

int PrintResults (List* table, char *sausage_data, int hash_function_id)
{
    printf("Функция: %s, количество элементов в списках:\n", hash_name[hash_function_id]);
    for (int i = 0; i < hash_size; ++i) {
        printf("Список %d: %lu \n", table[i].id, table[i].size);
    }
    printf("\n");
    return 0;
}

size_t GetFileSize (const char *file_name)
{
    assert(file_name);

    FILE *in = fopen(file_name, "r");
    assert(in);
    fseek(in, SEEK_SET, SEEK_END);
    size_t ans = ftell(in);

    fclose(in);
    return ans;
}

void TestFunction (const char *file_name, int hash_function_id)
{   
    assert(file_name);

    List* table = new List[hash_size]();
    
    size_t file_size = GetFileSize(file_name);
    char *sausage_data = new char [file_size];

    FILE *in = fopen (file_name, "r");
    assert(in);
    fread(sausage_data, sizeof(char), file_size, in);
    fclose(in);

    char *word = sausage_data, *begin = sausage_data;
    for (int i = 0; i < file_size; ++i) {
        if (    *sausage_data == '\n' || 
                *sausage_data == ' ' ||
                *sausage_data == '.' || 
                *sausage_data == '!' || 
                *sausage_data == '?' || 
                *sausage_data == 97 || //длинное тире 
                *sausage_data == ',' || 
                *sausage_data == ';') {

            *sausage_data = '\0';
            InsertWord(table, word, hash_function_id);
            word = sausage_data + 1;

        }
        sausage_data++;
    }
    PrintResults(table, sausage_data, hash_function_id);
    //delete[] table;
    //delete[] begin;
}

int InsertWord (List* table, char *word, int hash_function_id)
{
    assert(word);
    int hash = CountHash (word, hash_function_id);
    table[hash % hash_size].PushBack(word);
    return 0;
}

