#include "stdio.h"
#include "string.h"
#include <stdint.h>

// Имя файла с зашифрованным текстом
#define FILE_ENCRYPTED "encrypted.txt"
// Имя файла с открытым текстом
#define FILE_OPENNED "openned.txt"

// Ключ шифрования
uint32_t key = 0;
// Для промежуточных вычислений
uint8_t tmp;
// Количество букв
uint16_t cnt = 0;
// Символы для обработки
char ch1, ch2 = 0;

int main() {
    // Файл для чтения данных
    FILE *fecrypted, *foppened;
    
    // Попытка открыть файл на чтение
    fecrypted = fopen(FILE_ENCRYPTED, "r");

    // Если файла не существует
    if (fecrypted == NULL) {
        printf("ERROR: file "FILE_ENCRYPTED" is not found.\r\n");
        return -1;
    }

    // Попытка открыть файл на чтение
    foppened = fopen(FILE_OPENNED, "r");

    // Если файла не существует
    if (foppened == NULL) {
        printf("ERROR: file "FILE_OPENNED" is not found.\r\n");
        fclose(fecrypted);
        return -1;
    }

    // Пока не дошли до конца файла
    while ((ch1 = fgetc(fecrypted)) != EOF) {

        // Считать символ из открытого текста
        ch2 = fgetc(foppened);

        // Сбросить ключ
        tmp = 0;

        // Если считали букву
        if ((ch1 > 64 && ch1 < 91) || (ch1 > 96 && ch1 < 123)) {
            // Получить разность букв
            while (ch1 != ch2) { if (++ch2 > (90 + (32 * (ch1 > 96)))) ch2 -= 26; tmp++; }
            // Прибавить найдённое значение ключа
            key += tmp;
            // Прибавить количество 
            cnt++;
        }
    }

    // Получить среднее арифметическое
    key /= cnt;

    // Вывести результат
    printf("Found key: %d.\r\n", key);

    // Закрыть файлы
    fclose(fecrypted);
    fclose(foppened);

    return 0;  
}