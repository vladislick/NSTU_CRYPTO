#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Выходной файл
#define FILE_OUT "input.txt"

// Количество символов
#define SYM_CNT 60000

// Размер алфавита
unsigned int alphabet;
// Счётная переменная
uint16_t i;

int main() {
    // Файл с текстом
    FILE *fp;

    // Если не удалось открыть файл
    if ((fp = fopen(FILE_OUT, "w")) == NULL) {
        printf("Cannot open file <"FILE_OUT">.\r\n");
        return -1;
    }

    // Сброс размера алфавита
    alphabet = 0;

    // Получить размер алфавита
    while (!alphabet) {
        printf("Enter the alphabet size: ");
        scanf("%d", &alphabet);
    }

    // Получить новое "семя" для генератора случайных чисел
    srand(time(NULL));

    // Заполнить рандомными символами
    for (i = 0; i < SYM_CNT; i++) {
        fputc((rand() % alphabet), fp);
    }

    // Закрыть файл
    fclose(fp);

    return 0;
}