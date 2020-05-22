#include <assert.h>
#include <string.h>

enum functions {
    wordLen
};

const char* function_name[] = {
    "Длина слова"
};

unsigned long WordLen (const char *word);






unsigned long CountHash (const char *word, int function)
{
    switch (function) {
        case wordLen:
            return WordLen(word);
        
        default:
            return -1;
    }
}

unsigned long WordLen (const char *word)
{
    assert(word);
    return strlen(word);
}

