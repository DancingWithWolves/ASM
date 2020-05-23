#include <assert.h>
#include <string.h>

enum functions {
    word_len, sum_hash, sum_len_hash, rolling_hash, jenkins, jenkins_asm
};

const char* function_name[] = {
    "Word_length", "ASCII_sum", "ASCII_sum_divided_by_len", "Rolling_hash", "Jenkins", "Jenkins_asm"
};

unsigned long WordLen (const char *word);
unsigned long SumHash (const char *word);
unsigned long SumLenHash (const char *word);
unsigned long RollingHash (const char *word);
unsigned long Jenkins (const char *word);
unsigned long Jenkins_asm (const char* word);


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
        case jenkins_asm:
            return Jenkins_asm(word);
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

unsigned long RollingHash (const char* word)
{

    unsigned long hash = 0;

    while (*word != 0){
        hash = ((hash << 1) | (hash >> 31)) ^ *(word++);
    }

    return hash;
}

unsigned long Jenkins (const char* word) 
{   
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

unsigned long Jenkins_asm (const char* word)
{

    unsigned long hash = 0;

    asm ( R"(
    .intel_syntax noprefix

        movsx rax, BYTE PTR [rdi]
        test al, al

        je .Label4
        xor edx, edx

    .Label3:
        add rax, rdx
        add rdi, 1
        mov rdx, rax
        sal rdx, 10
        add rax, rdx
        mov rdx, rax
        shr rdx, 6
        xor rdx, rax
        movsx rax, BYTE PTR [rdi]
        test al, al
        jne .Label3
        lea rdx, [rdx+rdx*8]
        mov rax, rdx
        shr rax, 11
        xor rax, rdx
        mov rdx, rax
        sal rdx, 15
        add rax, rdx
        jmp .Exit

    .Label4:
        xor eax, eax
    .Exit:
        .att_syntax
    )"

        :"=r"(hash)
        :"D"(word)
    );

    return hash;
}
