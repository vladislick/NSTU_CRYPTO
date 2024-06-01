#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "aes128.h"

// Входной файл
#define FILE_IN "input.txt"
// Выходной файл
#define FILE_OUT "output.txt"
// Количество элементов в массиве
#define COUNTOF(x) (sizeof(x)/sizeof(*x))

// Входной и выходной файлы
static FILE *finp, *foutp;

// Ключ шифрования
static uint8_t key[16], key_l;

// Шифрованный и исходный текст
static uint8_t text[2][256];

// Длина текстов
static uint16_t text_len;

// Считываемый/записываемый символ
static char ch;

// Шифрование/расшифрование
static unsigned char op;

// Счётная переменная 
static uint16_t i;

int main() {
    // Если не удалось открыть файл
    if ((finp = fopen(FILE_IN, "r")) == NULL) {
        printf("Cannot open file <"FILE_IN">.\r\n");
        return -1;
    }

    // Если не удалось открыть файл
    if ((foutp = fopen(FILE_OUT, "w")) == NULL) {
        printf("Cannot open file <"FILE_OUT">.\r\n");
        return -1;
    }

    // Сброс ключа
    for (i = 0; i < COUNTOF(key); i++) key[i] = 0; 

    // Пока не получен ключ
    while (!(key_l = strlen((char*)key))) {
        printf("Enter the key: ");
        scanf("%s", key);
    }

    // Запросить вид операции (зашифровать или расшифровать)
    i = 2; 
    while(i > 1) {
        printf("Enter operation (0 - encrypt, 1 - decrypt): ");
        scanf("%hd", &i);
    }
    op = i;

    // Очистить массив входящего текста
    for (i = 0; i < COUNTOF(text[0]); i++) text[0][i] = ' ';

    // Пройтись по входному файлу
    for (i = 0; (ch = fgetc(finp)) != EOF; i++) {
        // Если файл слишком большой, то прервать чтение
        if (i >= COUNTOF(text[op])) break;
        // Записать текст входного файла
        text[0][i] = ch;
    }
    // Закрыть входной файл
    fclose(finp);

    // Дополнить длину входных данных, если нужно
    if (i % BLOCKSIZE) i += BLOCKSIZE - (i % BLOCKSIZE);
    // Запомнить длину текста
    text_len = i;

    // Выполнить операцию
    if (op) aesDecrypt(key, key_l, text[0], text[1], text_len); 
    else aesEncrypt(key, key_l, text[0], text[1], text_len);
    
    // Записать результат в файл
    for (i = 0; i < text_len; i++) fputc(text[1][i], foutp);
    // Закрыть входной файл
    fclose(foutp);

    printf("Done.\r\n");

    return 0;
}