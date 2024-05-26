#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

// Входной файл
#define FILE_IN "input.txt"

// Количество элементов в массиве
#define COUNTOF(x) (sizeof(x)/sizeof(*x))

// Количестов появлений конкретного символа
uint16_t alphabet[256];
// Общее количество символов в файле
uint32_t cnt;
// Считываемый символ
uint8_t ch;
// Счётная переменная
uint16_t i;
// Вероятность появления символа
double ch_frq;
// Энтропия файла
double summ;

// Calculates log2 of number.  
double Log2( double n ) {  return log( n ) / log( 2 ); }  

int main() {
    // Файл с текстом
    FILE *fp;

    // Если не удалось открыть файл
    if ((fp = fopen(FILE_IN, "r")) == NULL) {
        printf("Cannot open file <"FILE_IN">.\r\n");
        return -1;
    }

    // Сброс значений в массиве
    for (i = 0; i < COUNTOF(alphabet); i++) alphabet[i] = 0;

    // Сброс количества символов в файле
    cnt = 0;

    // Пройтись по всем символам файла
    // Собрать количество встречающихся символов
    while ((ch = fgetc(fp)) != (uint8_t)EOF) { 
        alphabet[ch]++;
        cnt++; 
    }

    // Сброс суммы энтропии файла
    summ = 0;

    // Вычислить вероятность для каждого символа
    for (i = 0; i < COUNTOF(alphabet); i++) {
        // Если данного символа не было вообще в тексте
        if (!alphabet[i] || alphabet[i] == '\r' || alphabet[i] == '\n') continue;
        // Записать количество встреченных символов
        ch_frq = alphabet[i];
        // Поделить на общее количество символов
        ch_frq /= cnt;
        // Прибавить результат к сумме
        summ += ch_frq * Log2(ch_frq);
    }

    // Вывести полученную энтропию файла
    printf("The information entropy of the file is equal to %.4f.\r\n", -summ);

    // Закрыть файл
    fclose(fp);

    return 0;
}