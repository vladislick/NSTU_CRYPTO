#include <stdio.h>
#include <stdint.h>

int main() {
	// Выражение x^y mod z
	static uint32_t x = 17, y = 573, z = 17;
	// Число для возведения в степень
    uint16_t power = x;
	// Остаток после операций
	uint16_t remainder = 1;
	// Показатель степени
	uint16_t step = 1;
	// Счётчик операций
	uint16_t cnt = 0;

	// Отобразить выражение, которое будет вычисляться
	printf("Calculate %d^%d mod %d.\r\n", x, y, z);

    // Пока степень не равна нулю
	while (y) {
		// Отобразить состояние переменных
		printf("%d^%d\t%d\t", x, step, power);
		// Получить остаток от деления
		power %= z;
		// Отобразить остаток от деления
		printf("%d\t", power);
		// Если у показателя степени выйдет 1-ца
		if (y & 1) {
			// Показать 1-цу
			printf("1\t");
			// Умножить остаток
			remainder *= power;
			// Если это не первое включение
			if (cnt) printf("%d\t", remainder);
			// Если это первое включение 
			else printf(" \t");
			// Увеличить счётчик операции умножения
			cnt++;
			// Получить остаток от деления
			remainder %= z;
			// Вывести полученный отсаток от деления
			printf("%d\r\n", remainder);
		}
		// Если у показателя степени выйдет 0
		else printf("0\t-\t-\r\n");
		// Возвести в степень
		power *= power;
		// Сдвинуть показатели степени
		y >>= 1; step <<= 1;
		// Увеличить счётчик операции умножения
		cnt++;
	}

	// Вывести результат
    printf("Number of multiplications is %d.\r\n", cnt);
    printf("Result is %d.\r\n", remainder);
}