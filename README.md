# Лабораторная работа
# “Исследование спектральных свойств хеш-функций”.
# Выполнил Тетерин Дмитрий, Б05-932.

Итак, “за кадром” была написана программа (HashTableTexst.cpp), которая создаёт хеш-таблицу на 8747 списков и хеширует заданный файл (в нашем случае *words.txt*, в котором 466551 слово).

Протестированные функции:

1) Длина слова: 

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/Word_length.png)

2) Сумма ASCII кодов символов строки: 

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/ASCII_sum.png)

3) Сумма ASCII-кодов, делённая на длину слова:

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/ASCII_sum_divided_by_len.png)

4) Rolling hash: 

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/Rolling_hash.png)

5) Хеш-функция Дженкинса: 

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/Jenkins.png)

Стоит обратить внимание на диапазон значений по оси Y: он самый маленький у Дженкинса (0 - 90), самый большой -- у длины слова (0 - 70000). Это значит, что наиболее оптимальной хеш-функцией с точки зрения компактности из предложенных является именно хеш-функция Дженкинса. С ней может потягаться только Rolling hash (его рейндж -- 0 - 120), остальные 3, как и ожидалось, дают совсем плохую компактность, неприемлемую в реальных задачах.


# Сделаем профилирвание.
Вот что говорит нам многоуважаемый perf.

# Скриншот из отчёта Perf:
![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/opt_0.png)

Понятно, что следует оптимизировать функцию Jenkins.

Случилось.
# Было:
```
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
```
# Стало:
```
unsigned long Jenkins_asm (const char* word)
{

    unsigned long hash = 0;

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
    :"=r"(hash)
    :"D"(word)
    );

    return hash;
}
```

# Результаты ускорения:

# -O1:

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/opt_1_out.png)

# -O3:

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/opt_3_out.png)

# Асмблерная вставка:

![Image alt](https://github.com/DancingWithWolves/ASM/raw/newHashTable/NewHashTable/graphs/after_asm.png)

Итак, мы сравнялись (или даже побили) *-O3* и выиграли *-O1* в 1,56 раза.

# В заключение хочется сказать: Пишешь плохой код - ты плохой кот. Пиши хороший код, хороший кот!

