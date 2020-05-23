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

unsigned long JenkinsAsm (const char* word)
{
    unsigned long d = 0;

    asm(R"(
    .intel_syntax noprefix

    movsx rax, BYTE PTR [rdi]
    test al, al
    je .MyLibL4
    xor edx, edx
    .MyLibL3:
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
    jne .MyLibL3
    lea rdx, [rdx+rdx*8]
    mov rax, rdx
    shr rax, 11
    xor rax, rdx
    mov rdx, rax
    sal rdx, 15
    add rax, rdx
    jmp .MyLibExit

    .MyLibL4:
    xor eax, eax
    .MyLibExit:
    .att_syntax
    )"
    :"=r"(d)
    :"D"(word)
    );

    return d;
}

unsigned long Crc32 (const char* word)
{

    unsigned long h = 0;
    asm(R"(
    .intel_syntax noprefix
    lea rax, [%1]
    xor %0, %0

    loooooop%=:
    Crc32 %0, byte ptr [rax]
    inc rax
    cmp byte ptr [rax], 0

    jne loooooop%=

    .att_syntax prefix
    )"
    : "=r"(h)
    : "r"(word)
    : "rax", "rcx"
    );

    return h;
}
