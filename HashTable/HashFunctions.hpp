#include <assert.h>
#include <string.h>

const int functions_qty = 5;

enum Functions {
    word_length, sum_hash, sum_len_hash, rolling_hash, Jenkins, Jenkins_asm, crc32
};
const char* hash_name[] = {"Длина слова", "Сумма аски-кодов", "Сумма аски-кодов, делённая на длину слова", "Rolling xor hash", "Jenkins", "Jenkins asm", "crc32"};

int SumHash (const char* word);
int SumLenHash (const char* word);
int RollingHash (const char* word);
int JenkinsC (const char* word);
//int JenkinsAsm( const char* word);
int crc32_func (const char* word);

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
        case crc32:
            return crc32_func (word);
        default:
            return -1;
        //case Jenkins_asm:
        //    return JenkinsAsm(word);
    }
    return 0;
}

int SumHash (const char* word)
{
    assert(word);
    int sum = 0;

    while (*word != '\0')
        sum += *(word++); 

    return sum;
}


int SumLenHash (const char* word)
{
    assert(word);
    int len = strlen(word);

    if (len != 0){
        return SumHash(word)/len;
    }

    return 0;
}

int RollingHash (const char* word)
{
    assert(word);
    int hash = 0;

    while (*word != 0){
        hash = ((hash << 1) | (hash >> 31)) ^ *(word++);
    }

    return hash;
}
int crc32_func (const char* data)
{

    int h = 0;
    asm(R"(
    .intel_syntax noprefix
    lea rax, [%1]
    xor %0, %0

    loooooop%=:
    crc32 %0, byte ptr [rax]
    inc rax
    cmp byte ptr [rax], 0

    jne loooooop%=

    .att_syntax prefix
    )"
    : "=r"(h)
    : "r"(data)
    : "rax", "rcx"
    );

    return h;
}
int JenkinsC (const char* word) 
{
    assert(word);
    int hash = 0;

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
int JenkinsAsm(const char* word){

    int d = 0;

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