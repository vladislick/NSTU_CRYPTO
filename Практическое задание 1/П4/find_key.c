#include "stdio.h"
#include "string.h"
#include <stdint.h>

// Имя файла с зашифрованным текстом
#define FILE_ENCRYPTED "encrypted.txt"

// Количество элементов в массиве
#define COUNTOF(x) ((sizeof(x))/(sizeof(*x)))

// Словарь
const char* dictionary[] = { 
    "to", "be", "the", "are", 
    "you", "my", "me", "a",
    "i", "this", "it", "that",
    "can", "will", "not"
};


// Счётная переменная
uint8_t i;
// Ключ шифрования
uint8_t key;
// Считываемый символ
char ch;

// Частота совпадения ключа
// На случай нескольких совпадений
uint8_t wrd_frq[25];
// Буфер для хранения одного слова
char wrd[10];

int main() {
    // Файл для чтения данных
    FILE *fecrypted;

    // Очистить таблицу частоты совпадения слова
    for (i = 0; i < COUNTOF(wrd_frq); i++) wrd_frq[i] = 0;

    // Перебор всех возможных ключей
    for (key = 1; key < 26; key++) {
        // Попытка открыть файл на чтение
        fecrypted = fopen(FILE_ENCRYPTED, "r");

        // Если файла не существует
        if (fecrypted == NULL) {
            printf("ERROR: cannot open "FILE_ENCRYPTED" for read.\r\n");
            return -1;
        }

        // Очистить буфер слова
        for (i = 0; i < COUNTOF(wrd); i++) wrd[i] = '\0';

        // Бесконечно идти по символам файла
        while (1) {
            // Считать символ
            ch = fgetc(fecrypted);
            // Если попался разделитель слова
            if (ch == ' ' || ch == '\r' || ch == '\n' || ch == EOF || ch == ',' || ch == '.') {
                // Если слово есть
                if (strlen(wrd)) {
                    //printf("Read word: <%s>\r\n", wrd);
                    // Сравнить полученное слово со словами в словаре
                    for (i = 0; i < COUNTOF(dictionary); i++)
                        // Если слово найдено, то увеличить счётчик слов у ключа
                        if (strcmp(wrd, dictionary[i]) == 0) { wrd_frq[key-1]++; break; }
                }
                // Очистить буфер слова
                for (i = 0; i < COUNTOF(wrd); i++) wrd[i] = '\0';
                // Если конец файла
                if (ch == EOF) break;
            } 
            // Если это обычный символ
            else {
                // Если считали заглавную букву, сделать её строчной
                if (ch > 64 && ch < 91) ch += 32;
                // Если считали строчную букву
                if (ch > 96 && ch < 123) {
                    // Сместить букву на размер ключа
                    ch -= key;
                    // Если вышли за диапазон
                    if (ch < 97) ch += 26;
                    // Добавить символ в буфер слова
                    wrd[strlen(wrd)] = ch;
                }
            }
        }

        // Закрыть файл
        fclose(fecrypted);
    }

    // Сбросить ключ
    key = 0;
    // Сбросить символ
    ch = 0;

    // Пройтись по всем ключам
    for (i = 0; i < COUNTOF(wrd_frq); i++) {
        // Если попался ключ с наибольшим кол-вом совпадений
        if (wrd_frq[i] > ch) {
            // Запомнить количество совпадений
            ch = wrd_frq[i];
            // Запомнить ключ с наибольшим кол-вом совпадений
            key = i + 1;
        }
    }

    // Если ключ был найден
    if (key) 
        printf("Found key: %d (matched %d words from the dictionary).\r\n", key, ch);
    else
        printf("Failed to find the key.\r\nThere may not be enough words in the dictionary.\r\n");

    return 0;
}