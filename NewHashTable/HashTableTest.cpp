#include "HashTable.hpp"
#include <stdio.h>

int main (int argc, const char *argv[])
{
    HashTable table;
    table.FillTable("/home/dimoha_zadira/Work/Asm/ASM/NewHashTable/words.txt", 4);
    return 0;
}