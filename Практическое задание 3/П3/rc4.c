#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Входной файл
#define FILE_IN "input.txt"
// Выходной файл
#define FILE_OUT "output.txt"

// Количество элементов в массиве
#define COUNTOF(x) (sizeof(x)/sizeof(*x))

// Массив S
uint16_t S[256];
// Ключ шифрования
char key[256];
// Число K
uint16_t K;
// Переменная-итератор
uint16_t i, j;

// Длина ключа шифрования в байтах
uint8_t key_l;

// Считываемый символ
char ch;


int main() {
    // Файл с текстом
    FILE *finp, *foutp;

    // Если не удалось открыть файл
    if ((finp = fopen(FILE_IN, "r")) == NULL) {
        printf("Cannot open file <"FILE_IN">.\r\n");
        return -1;
    }

    // Сброс ключа
    for (i = 0; i < COUNTOF(key); i++) key[i] = '\0'; 

    // Пока не получен ключ
    while ((key_l = strlen(key)) == 0) {
        printf("Enter the key: ");
        scanf("%s", key);
    }

    // Продлить ключ до конца массива
    for (i = 0; (i + key_l) < 256; i++) key[key_l + i] = key[i];

    // Сброс значений в массиве S
    for (i = 0; i < 256; i++) S[i] = i; 

    // Инициализация KSA блока
    for (i = 0, j = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key_l]) % 256;
        // Поменять местами S[i] и S[j]
        K = S[i];
        S[i] = S[j];
        S[j] = K;
    }

    // Если не удалось открыть файл
    if ((foutp = fopen(FILE_OUT, "w")) == NULL) {
        printf("Cannot open file <"FILE_OUT">.\r\n");
        return -1;
    }
    
    // Генерация псевдослучайной последовательности
    i = 0;
    j = 0;
    // Пока не достигли конца файла
    while ((ch = fgetc(finp)) != EOF) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        K = S[i];
        S[i] = S[j];
        S[j] = K;
        K = (S[i] + S[j]) % 256;
        fputc(S[K] ^ ch, foutp);
    }

    // Закрыть файл
    fclose(foutp);
    fclose(finp);

    return 0;
}