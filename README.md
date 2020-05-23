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

