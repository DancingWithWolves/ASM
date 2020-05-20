#include <stdio.h>
#include <stdlib.h>
#include "HashTable.hpp"



int main(int argc, const char *argv[])
{
    if (!argv[1]) {
        printf("Укажите имя файла, слова которого будем хешировать!\n");
        return 0;
    }
    if (argv[2]) {
        TestFunction(argv[1], atoi(argv[2]));
    } else {
        TestFunctions(argv[1]);
    }
    

    return 0;
}

