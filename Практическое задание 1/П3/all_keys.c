#include "stdio.h"
#include "string.h"

// Имя файла с зашифрованным текстом
#define FILE_ENCRYPTED "encrypted.txt"
// Имя файла с возможными комбинациями
#define FILE_RESULT "result.txt"

// Ключ шифрования
unsigned int key = 0;
// Считываемый символ
char ch = 0;

int main() {
    // Файл для чтения данных
    FILE *fecrypted, *fresult;

    // Попытка открыть файл на чтение
    fresult = fopen(FILE_RESULT, "w");

    // Если файла не существует
    if (fresult == NULL) {
        printf("ERROR: cannot open "FILE_RESULT" for writing.\r\n");
        fclose(fresult);
        return -1;
    }

    // Перебор всех возможных ключей
    for (key = 1; key < 26; key++) {
        // Попытка открыть файл на чтение
        fecrypted = fopen(FILE_ENCRYPTED, "r");

        // Если файла не существует
        if (fecrypted == NULL) {
            printf("ERROR: file "FILE_ENCRYPTED" is not found.\r\n");
            return -1;
        }

        // Вставить заголовок
        fprintf(fresult, "[ Key = %d ]\r\n", key);
        
        // Пока не дошли до конца файла
        while ((ch = fgetc(fecrypted)) != EOF) {
            // Если считали заглавную букву
            if (ch > 64 && ch < 91) {
                // Сместить букву на размер ключа
                ch -= key;
                // Если вышли за диапазон
                if (ch < 65) ch += 26;
            }
            // Если считали строчную букву
            else if (ch > 96 && ch < 123) {
                // Сместить букву на размер ключа
                ch -= key;
                // Если вышли за диапазон
                if (ch < 97) ch += 26;
            }

            // Положить полученный символ в файл
            fputc(ch, fresult);
        }

        // Вставить пустые строки
        fprintf(fresult, "\r\n");

        // Закрыть файл
        fclose(fecrypted);
    }

    // Закрыть файл
    fclose(fresult);

    return 0;  
}