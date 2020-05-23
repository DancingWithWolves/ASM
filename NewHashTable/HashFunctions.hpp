#include <assert.h>
#include <string.h>

enum functions {
    word_len, sum_hash, sum_len_hash, rolling_hash, jenkins
};

const char* function_name[] = {
    "Word length", "ASCII sum", "ASCII sum divided by len", "Rolling hash", "Jenkins"
};

unsigned long WordLen (const char *word);
unsigned long SumHash (const char *word);
unsigned long SumLenHash (const char *word);
unsigned long RollingHash (const char *word);
unsigned long Jenkins (const char *word);


unsigned long CountHash (const char *word, int function)
{
    switch (function) {
        case word_len:
            return WordLen(word);
        case sum_hash:
            return SumHash(word);
        case sum_len_hash:
            return SumLenHash(word);
        case rolling_hash:
            return RollingHash(word);
        case jenkins:
            return Jenkins(word);
        default:
            return -1;
    }
}

unsigned long WordLen (const char *word)
{
    assert(word);
    return strlen(word);
}


unsigned long SumHash (const char* word)
{
    unsigned long sum = 0;

    while (*word != 0){
        sum += *(word++);
    }

    return sum;
}

unsigned long SumLenHash (const char* word)
{

    unsigned long len = WordLen(word);

    if (len != 0){
        return SumHash(word)/len;
    }

    return 0;
}

unsigned long RollingHash(const char* word){

    unsigned long hash = 0;

    while (*word != 0){
        hash = ((hash << 1) | (hash >> 31)) ^ *(word++);
    }

    return hash;
}

unsigned long Jenkins(const char* word) {

    unsigned long hash = 0;

    int i = 0;
    int len = strlen(word);

    while (i != len) {
        hash += word[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

