#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "List.hpp"
#include "HashFunctions.hpp"

const int table_size = 17;

size_t GetFileSize (const char *file_name);
void TestFunctions (const char *file_name);
void TestFunction (const char *file_name, int hash_function);
int InsertWord (List* table, char *word, int hash_function);
int PrintResults (List* table, int hash_function_id);

void TestFunctions (const char *file_name)
{
    for (int i = 0; i < functions_qty; ++i) {
        TestFunction(file_name, i);
    }
}


void Plot (const char *fname)
{
    char command[256] = "";
    sprintf(command, "gnuplot -e \"set terminal png; set output 'graphs/%s.png'; plot 'graphs/%s.dat' u 1:2\"", fname, fname);
   
    
    system(command);
    //system("  ");
}

int PrintResults (List* table, int hash_function_id)
{
    char fname[64];
    sprintf(fname, "%s.dat", hash_name[hash_function_id]);
    size_t summary = 0;
    FILE *out = fopen(fname, "w");
    printf("Функция: %s, количество элементов в списках:\n", hash_name[hash_function_id]);
    for (int i = 0; i < table_size; ++i) {
        summary += table[i].size;
        printf("Список %d: %lu \n", table[i].id, table[i].size);
        fprintf(out, "%d\t%lu\n", table[i].id, table[i].size);
    }    
    printf("sum = %lu\n", summary);
    fclose(out);

    Plot(hash_name[hash_function_id]);

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

char **SplitText (char *text, int text_len, int *qty)
{
    assert(text);
    assert(qty);

    *qty = 0;
    for (int i = 0; i < text_len; ++i) {
        if (text[i] == '\n' || 
            text[i] == ' ') {
                text[i] = '\0';
                (*qty)++;          
            }
    }
    text[text_len-1] = '\0';

    char **output = new char* [*qty];
    assert(output);
    int cur_word = 0;

    char *word = &(text[0]);
    for (int i = 0; i < text_len; ++i) {
        if (text[i] == '\0') {
            output[cur_word++] = word;
            //printf("выцепил %s\n", word);
            word = &(text[i+1]);
        }
    }

    return output;
}

void TestFunction (const char *file_name, int hash_function_id)
{   
    assert(file_name);

    List* table = new List[table_size]();
    
    size_t file_size = GetFileSize(file_name);
    char *sausage_data = new char [file_size];

    FILE *in = fopen (file_name, "rb");
    assert(in);
    fread(sausage_data, sizeof(char), file_size, in);
    fclose(in);

    int words_qty = 0;
    char **words = SplitText(sausage_data, file_size, &words_qty);
    printf("words_qty = %d\n", words_qty);
    for (int i = 0; i < words_qty; ++i) {
        table[CountHash(words[i], hash_function_id) % table_size].PushBack(words[i]);
    }

    PrintResults(table, hash_function_id);
    free(words);
    free(sausage_data);
    //delete[] table;
    //delete[] begin;
}

int InsertWord (List* table, char *word, int hash_function_id)
{
    assert(word);
    int hash = CountHash (word, hash_function_id);
    table[hash % table_size].PushBack(word);
    return 0;
}

