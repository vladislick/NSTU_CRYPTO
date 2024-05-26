#include "stdio.h"
#include "string.h"
#include <stdint.h>

// Имя входного файла
#define FILE_IN "input.txt"
// Имя выходного файла
#define FILE_OUT "output.txt"

// Ключ шифрования
int key = 0;
// Символ для обработки
char ch = 0;
// Беззнаковый код символа
uint8_t uch = 0;

int main() {
    // Файл для чтения данных
    FILE *finp, *foutp;
    
    // Попытка открыть файл на чтение
    finp = fopen(FILE_IN, "r");

    // Если файла не существует
    if (finp == NULL) {
        printf("ERROR: file "FILE_IN" is not found.\r\n");
        return -1;
    }

    // Попытка открыть файл на чтение
    foutp = fopen(FILE_OUT, "w");

    // Если файла не существует
    if (foutp == NULL) {
        printf("ERROR: cannot create "FILE_OUT" file.\r\n");
        fclose(finp);
        return -2;
    }

    // Запрос ключа шифрования
    while (!key || key > 25) {
        printf("Enter the key: ");
        scanf("%d", &key);
    }

    // Пока не дошли до конца файла
    while ((ch = fgetc(finp)) != EOF) {
        // Записать код символа
        uch = ch;
        // Если считали заглавную букву
        if (uch > 64 && uch < 91) {
            // Сместить букву на размер ключа
            uch -= key;
            // Если вышли за диапазон
            if (uch < 65) uch += 26;
        }
        // Если считали строчную букву
        else if (uch > 96 && uch < 123) {
            // Сместить букву на размер ключа
            uch -= key;
            // Если вышли за диапазон
            if (uch < 97) uch += 26;
        }

        // Положить полученный символ в файл
        fputc(uch, foutp);
    }

    // Закрыть файлы
    fclose(finp);
    fclose(foutp);
    
    return 0;  
}