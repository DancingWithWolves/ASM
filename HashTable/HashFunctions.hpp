#include <assert.h>
#include <string.h>

const int functions_qty = 5;

enum Functions {
    word_length, sum_hash, sum_len_hash, rolling_hash, Jenkins, Jenkins_asm
};
const char* hash_name[] = {"Длина слова", "Сумма аски-кодов", "Сумма аски-кодов, делённая на длину слова", "Rolling xor hash", "Jenkins", "Jenkins asm"};

unsigned long SumHash (const char* word);
unsigned long SumLenHash (const char* word);
unsigned long RollingHash (const char* word);
unsigned long JenkinsC (const char* word);
//unsigned long JenkinsAsm(const char* word);

int CountHash (const char *word, int function)
{
    assert(word);
    switch (function) {
        case word_length:
            return strlen(word);
        case sum_hash:
            return SumHash(word);
        case sum_len_hash:
            return SumLenHash(word);
        case rolling_hash:
            return RollingHash(word);
        case Jenkins:
            return JenkinsC(word);
        //case Jenkins_asm:
        //    return JenkinsAsm(word);
    }
    return 0;
}

unsigned long SumHash (const char* word)
{
    assert(word);
    unsigned long sum = 0;

    while (*word != '\0')
        sum += *(word++); 

    return sum;
}


unsigned long SumLenHash (const char* word)
{
    assert(word);
    unsigned long len = strlen(word);

    if (len != 0){
        return SumHash(word)/len;
    }

    return 0;
}

unsigned long RollingHash (const char* word)
{
    assert(word);
    unsigned long hash = 0;

    while (*word != 0){
        hash = ((hash << 1) | (hash >> 31)) ^ *(word++);
    }

    return hash;
}

unsigned long JenkinsC (const char* word) 
{
    assert(word);
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

/*
unsigned long JenkinsAsm(const char* word){

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
*/