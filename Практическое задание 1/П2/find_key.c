#include "stdio.h"
#include "string.h"

// Имя файла с зашифрованным текстом
#define FILE_ENCRYPTED "encrypted.txt"
// Имя файла с открытым текстом
#define FILE_OPENNED "openned.txt"

// Ключ шифрования
unsigned int key = 0;
// Количество букв
unsigned int cnt = 0;
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

        // Если считали букву
        if ((ch1 > 64 && ch1 < 91) || (ch1 > 96 && ch1 < 123)) {
            // Получить разность букв
            if (ch1 > ch2) key += ch1 - ch2;
            else key += ch2 - ch1;
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