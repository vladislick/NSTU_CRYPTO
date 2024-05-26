#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Входной файл
#define FILE_IN "input.txt"
// Файл ключа
#define FILE_KEY "key.txt"
// Выходной файл
#define FILE_OUT "output.txt"

// Количество элементов в массиве
#define COUNTOF(x) (sizeof(x)/sizeof(*x))

// Считанный символ
char ch, key;

int main() {
    // Файл с текстом
    FILE *finp, *foutp, *fkeyp;

    // Если не удалось открыть файл
    if ((finp = fopen(FILE_IN, "r")) == NULL) {
        printf("Cannot open input file <"FILE_IN">.\r\n");
        return -1;
    }

    // Если не удалось открыть файл
    if ((fkeyp = fopen(FILE_KEY, "r")) == NULL) {
        printf("Cannot open key file <"FILE_KEY">.\r\n");
        return -1;
    }

    // Если не удалось открыть файл
    if ((foutp = fopen(FILE_OUT, "w")) == NULL) {
        printf("Cannot open output file <"FILE_OUT">.\r\n");
        return -1;
    }

    // Пройти по всем символам входного файла
    while((ch = fgetc(finp)) != EOF) {
        // Получить символ ключа
        key = fgetc(fkeyp);
        // Если ключ закончился
        if (key == EOF) break;
        // Положить результат в выходной файл
        fputc(ch ^ key, foutp);
    }

    // Закрыть файл
    fclose(finp);
    fclose(fkeyp);
    fclose(foutp);

    return 0;
}