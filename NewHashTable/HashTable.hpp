#include <stdlib.h>
#include <assert.h>

#include "List.hpp"
#include "HashFunctions.hpp"

class HashTable {
    private:
    List *table;
    unsigned long FileSize (const char *fname)
    {
        FILE *in = fopen(fname, "r");
        assert(in);
        fseek(in, 0, SEEK_END);
        unsigned long ans = ftell(in);
        fclose(in);
        return ans;
    }
    
    size_t HandleInput (char *text, size_t text_size)
    {
        if (text[text_size-1] != '\n') printf("what the hug?\n");
        char *to = text, *from = text;
        size_t new_size = 0;
        for (size_t i = 0; i < text_size; ++i) {
            while ( (i < text_size - 1) && (
                    (*from == '\n' && *(from + 1) == '\n') ||
                    (*from == ' ' && *(from + 1) == ' ') )
                  ) {
                from++;
                *from = '\0';
            }
            
            if (*from == '\n' || *from == ' ') {
                *from = '\0';
            }

            *to++ = *from++;
            new_size++;
        }
        return new_size;
    }

    char **SplitText (char *text, size_t text_size, size_t *words_qty)
    {
        for (size_t i = 0; i < text_size; ++i) {
            if (text[i] == '\0') { (*words_qty)++; }
        }

        char **words = new char* [(*words_qty)];
        char *cur_word = text;
        int word_num = 0;
        for (size_t i = 0; i < text_size; ++i) {
            if (text[i] == '\0') {
                words[word_num++] = cur_word;
                cur_word = &(text[i+1]);
            }
        }
        
        return words;
    }

    void Plot (const char *fname)
    {
        char command[256] = "";
        sprintf(command, "gnuplot -e \"set nokey; set terminal png; set output '/home/dimoha_zadira/Work/Asm/ASM/NewHashTable/graphs/%s.png'; plot '/home/dimoha_zadira/Work/Asm/ASM/NewHashTable/graphs/%s.dat' u 1:2\"", fname, fname);
        
        system(command);
    }
    
    int PrintResults (int function_id)
    {
        char fname[128];
        sprintf(fname, "/home/dimoha_zadira/Work/Asm/ASM/NewHashTable/graphs/%s.dat", function_name[function_id]);
        size_t summary = 0;
        FILE *out = fopen(fname, "w");

        for (int i = 0; i < table_size; ++i) {
            summary += table[i].size;
            fprintf(out, "%d\t%lu\n", table[i].id, table[i].size);
        }    

        fclose(out);
        
        Plot(function_name[function_id]);

        return 0;
    }

    public:

    const size_t table_size = 8747;

    void FillTable (const char *fname, int function_id)
    {
        const unsigned long file_size = FileSize(fname);
        char *sausage_data = new char [file_size];

        FILE *in = fopen (fname, "rb");
        fread(sausage_data, sizeof (char), file_size, in);
        fclose(in);


        size_t text_size = HandleInput(sausage_data, file_size);

        size_t words_qty = 0;
        char **text = SplitText(sausage_data, text_size, &words_qty);

        for (int i = 0; i < words_qty; ++i) {
            table[Jenkins(text[i]) % table_size].PushBack(text[i]);
            //table[CountHash(text[i], function_id) % table_size].PushBack(text[i]);
        }

        PrintResults(function_id);

    }

    
    HashTable () { table = new List [table_size](); }
    //~HashTable () { delete[] table; }

};