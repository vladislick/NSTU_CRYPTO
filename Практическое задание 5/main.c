#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Файл с паролями
#define PASSWORD_FILE "passwd.txt"
// Количество элементов в массиве
#define COUNTOF(x) (sizeof(x)/sizeof(*x))

// Описание таблицы доступных команд
typedef struct {
    // Имя команды
    const char* name;
    // Описание команды
    const char* desc;
    // Кол-во аргументов
    uint8_t args;
    // Описание аргументов
    const char* args_desc;
    // Функция-обработчик команды
    void (*func)();
} cmds_t;

// Строка ввода от пользователя
static char input[64];
// Слова ввода от пользователя
static char word[4][16];
// Имя текущего пользователя
static char username[16];
// Счётная переменная
static uint8_t i, j;
// Символ
static char ch;
// Количество слов
static uint8_t wrds;
// Файл с паролями
static FILE *fpasswd;
// Значение контрольной суммы
static uint8_t* md5_value;

// Разбить ввод пользователя на слова
uint8_t getWords(const char* text) {
    // Вспомогательные переменные
    static uint8_t wrd, i, j;
    // Пройтись по всем символам
    for (wrd = j = i = 0; i < (strlen(text) + 1); i++) {
        // Если слов слишком много
        if (wrd >= COUNTOF(word)) break;
        // Если символ принадлежит слову
        if (text[i] != ' ' && text[i] != '\0') { word[wrd][j++] = text[i]; word[wrd][j] = '\0'; }
        // Если символ разделяет слова между собой
        else { wrd++; j = 0; }
    }
    // Вернуть количество распознанных слов
    return wrd;
}

// Функция вывода справки
void help();

// Попробовать войти в пользователя
void login() {
    // Если файла базы данных не существует
    if ((fpasswd = fopen(PASSWORD_FILE, "r")) == NULL) {
        printf("ERROR: database <"PASSWORD_FILE"> not found.\r\n");
        return;
    }
    // Считывать базу данных построчно
    while (fgets(input, COUNTOF(input), fpasswd) != 0) {
        // Нахождение имени пользователя в базе данных
        for (i = 0; i < COUNTOF(input); i++) 
            if (input[i] == '\t' || input[i] == '\r' || input[i] == '\n') input[i] = '\0';
        // Если пользователь с таким именем существует
        if (!strcmp(input, word[1])) {
            // Подсчёт хэш-функции введённого пароля
            md5_value = md5sum(word[2], strlen(word[2]));
            // Проверка на совпадение пароля
            for (i = 0; i < 16; i++) {
                for (j = 0; j < 2; j++) {
                    // Получить полубайт
                    ch = (md5_value[i] >> (j * 4)) & 0xF;
                    // Перевести полубайт в шестнадцатеричный код
                    if (ch > 9) ch = 'a' + ch - 10;
                    else ch += '0';
                    // Если хэш не совпал
                    if (ch != input[strlen(input) + 1 + (i * 2) + j]) {
                        printf("ERROR: incorrect password.\r\n");
                        fclose(fpasswd);
                        return;
                    }
                }   
            }
            // Указать имя текущего пользователя 
            strcpy(username, word[1]);
            printf("User <%s> logged in successfully.\r\n", username);
            // Закрыть файл
            fclose(fpasswd);
            return;
        }
    }
    // Вывести сообщение об ошибке
    printf("ERROR: user <%s> doesn't exists.\r\n", word[1]);
    // Закрыть файл
    fclose(fpasswd);
}

// Добавить нового пользователя
void useradd() {
    // Если файла базы данных не существует
    if ((fpasswd = fopen(PASSWORD_FILE, "r")) == NULL) {
        printf("NOTICE: database <"PASSWORD_FILE"> not found, it will be created automatically.\r\n");
    }
    // Если база данных есть, то проверить, нет ли такого пользователя
    else {
        // Считывать базу данных построчно
        while (fgets(input, COUNTOF(input), fpasswd) != 0) {
            // Нахождение имени пользователя в базе данных
            for (i = 0; i < COUNTOF(input); i++) 
                if (input[i] == '\t' || input[i] == '\r' || input[i] == '\n') input[i] = '\0';
            // Если пользователь с таким именем уже существует
            if (!strcmp(input, word[1])) {
                printf("ERROR: user <%s> exists in database.\r\n", word[1]);
                fclose(fpasswd);
                return;
            }
        }
        // Закрыть файл
        fclose(fpasswd);
    }

    // Если не удалось открыть файл базы данных для добавления
    if ((fpasswd = fopen(PASSWORD_FILE, "a")) == NULL) {
        printf("ERROR: cannot create database <"PASSWORD_FILE">.\r\n");
        return;
    }

    // Добавление новой записи
    fputs(word[1], fpasswd);
    fputc('\t', fpasswd);
    // Подсчёт хэш-функции от пароля
    md5_value = md5sum(word[2], strlen(word[2]));
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 2; j++) {
            // Получить полубайт
            ch = (md5_value[i] >> (j * 4)) & 0xF;
            // Перевести полубайт в шестнадцатеричный код
            if (ch > 9) ch = 'a' + ch - 10;
            else ch += '0';
            // Положить полученный символ в файл
            fputc(ch, fpasswd);
        }
    }
    fputc('\r', fpasswd);
    fputc('\n', fpasswd);

    // Вывод сообщения
    printf("User <%s> added to database <"PASSWORD_FILE"> successfully.\r\n", word[1]);

    // Закрыть файл
    fclose(fpasswd);
}


// Попробовать выйти из пользователя
void logout() {
    // Если пользователь и так не вошёл
    if (!strcmp(username, "$")) {
        printf("No authorized user.\r\n");
        return;
    }
    strcpy(username, "$");
    printf("You are logged out of your user.\r\n");
}

// Завершить программу
void exitf() {
    // Добавить пустую строку
    printf("\r\n");
    // Выйти из программы
    exit(0);
}

// Удалить пользователя из базы данных
void userdel() {
    // Файл для копирования
    static FILE *fcopy;
    // Второй буфер строки
    static char input2[COUNTOF(input)];

    // Если файла базы данных не существует
    if ((fpasswd = fopen(PASSWORD_FILE, "r")) == NULL) {
        printf("ERROR: database <"PASSWORD_FILE"> not found.\r\n");
        return;
    }

    // Если файла базы данных не существует
    if ((fcopy = fopen(PASSWORD_FILE".copy", "w")) == NULL) {
        printf("ERROR: cannot create copied database.\r\n");
        return;
    }

    // Считывать базу данных построчно
    while (fgets(input, COUNTOF(input), fpasswd) != 0) {
        // Копировать строку из базы данных
        strcpy(input2, input);
        // Нахождение имени пользователя в базе данных
        for (i = 0; i < COUNTOF(input); i++)
            if (input[i] == '\t' || input[i] == '\r' || input[i] == '\n') input[i] = '\0';
        // Если пользователь с таким именем существует
        if (!strcmp(input, word[1])) {
            // Подсчёт хэш-функции введённого пароля
            md5_value = md5sum(word[2], strlen(word[2]));
            // Проверка на совпадение пароля
            for (i = 0; i < 16; i++) {
                for (j = 0; j < 2; j++) {
                    // Получить полубайт
                    ch = (md5_value[i] >> (j * 4)) & 0xF;
                    // Перевести полубайт в шестнадцатеричный код
                    if (ch > 9) ch = 'a' + ch - 10;
                    else ch += '0';
                    // Если хэш не совпал
                    if (ch != input[strlen(input) + 1 + (i * 2) + j]) {
                        printf("ERROR: incorrect password.\r\n");
                        fclose(fpasswd);
                        fclose(fcopy);
                        remove(PASSWORD_FILE".copy");
                        return;
                    }
                }
            }
            // Если удаляли сами себя
            if (!strcmp(word[1], username)) {
                strcpy(username, "$");
                printf("You are logged out of your user.\r\n");
            }
            // Вывести сообщение об успешном удалении
            printf("User <%s> successfully deleted.\r\n", word[1]);
        } 
        // Если это другой пользователь, то просто скопировать строку
        else fputs(input2, fcopy);
    }

    // Закрыть файл
    fclose(fpasswd);
    fclose(fcopy);

    // Удалить оригинальную базу данных
    remove(PASSWORD_FILE);
    // Поставить на её место новую базу данных
    rename(PASSWORD_FILE".copy", PASSWORD_FILE);
}

// Таблица доступных команд
const cmds_t cmds[] = {
    { // Команда входа пользователя
        "login", "log in using %username with password %password", 
        2, "%username %password", &login
    },
    { // Команда выхода пользователя
        "logout", "logout current user", 
        0, "", &logout
    },
    { // Команда добавления пользователя
        "useradd", "add new user %username with password %password", 
        2, "%username %password", &useradd
    },
    { // Команда удаления пользователя
        "userdel", "delete user %username with password %password", 
        2, "%username %password", &userdel
    },
    { // Команда выхода из программы
        "exit", "terminate this app", 
        0, "", &exitf
    },
};

// Вывести справку о доступных командах
void help() {
    printf("NOTE: try one of this commands:\r\n");
    for (i = 0; i < COUNTOF(cmds); i++) {
        printf("<%s> - %s, usage: %s %s\r\n", 
            cmds[i].name, cmds[i].desc, 
            cmds[i].name, cmds[i].args_desc
        );
    }
}

int main() {
    // По умолчанию имени пользователя нет
    strcpy(username, "$");
    
    while(1) {
        // Очистить ввод после предыдущего
        for (i = 0; i < COUNTOF(word); i++) memset(word[i], '\0', COUNTOF(word[0]));
        // Запрос команды у пользователя
        printf("\r\n%s> ", username);
        fgets(input, COUNTOF(input), stdin);
        // Очистить ввод от символа ввода
        for (i = 0; i < COUNTOF(input); i++) if (input[i] == '\r' || input[i] == '\n') input[i] = '\0';
        // Разбить ввод на слова и получить их кол-во
        wrds = getWords(input);
        // Если ничего не ввели
        if (!wrds) continue;
        // Вычесть из количества слов название команды
        wrds--;
        // Поиск введённой команды в списке доступных команд
        for (i = 0; i < COUNTOF(cmds); i++) {
            // Если название команды существует
            if (!strcmp(word[0], cmds[i].name)) {
                // Если количество аргументов неверное
                if (wrds != cmds[i].args) printf("ERROR: incorrect args, use: %s.\r\n", cmds[i].args_desc);
                // Если всё хорошо, то перейти к выполнению команды
                else (*cmds[i].func)();
                break;
            }
        }
        // Если введённой команды не существует
        if (i == COUNTOF(cmds)) { printf("ERROR: command <%s> doesn't exist.\r\n", word[0]); help(); }
    }


    return 0;
}