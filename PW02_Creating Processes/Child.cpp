#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define ERROR_FILE_OPEN -3
#define LENGTH 30

struct Database
{
    int num;                  // Номер зачетки
    char fam[LENGTH];         // Фамилия студента
    char first_name[LENGTH];  // Имя студента
    char second_name[LENGTH]; // Отчество студента
    double grade;             // Средний балл
};

int main(int argc, char *argv[])
{
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE *fp = NULL;
    struct Database arg;
    struct Database student;

    char file_name[LENGTH] = {"\0"};
    char ch;

    printf("_____________________________________________________________________\n\n");
    // проверка количества аргументов
    if (argc == 1 || argc < 7)
    {
        printf("Неверное количество аргументов.\n");
        ch = getch();
        return -1;
    }

    printf("---| Процесс работает |---\n\n");

    // 1. Выводит на консоль информацию, полученную через командную строку
    printf("\t--- 1 ---\n\n");
    printf("Вывод аргументов командной строки:\n");
    for (int i = 0; i < argc; ++i)
        printf("argv[%d] --> %s\n", i, argv[i]);

    strcpy(file_name, argv[1]);
    arg.num = atoi(argv[2]);
    strcpy(arg.fam, argv[3]);
    strcpy(arg.first_name, argv[4]);
    strcpy(arg.second_name, argv[5]);
    arg.grade = atof(argv[6]);

    fp = fopen(file_name, "rb+");
    if (fp == NULL)
    {
        printf("Ошибка чтения файла\n");
        ch = _getch();
        exit(ERROR_FILE_OPEN);
    }

    // 2. Корректирует в файле, созданном процессом Parent, нужную запись,
    //    т.е. устанавливает новый средний балл студента.
    printf("\n\t--- 2 ---\n\n");
    bool found = false;
    while (fread(&student, sizeof(Database), 1, fp) != NULL)
    {
        if (arg.num == student.num)
        {
            if (strcmp(student.fam, arg.fam) == 0)
            {
                fseek(fp, -(int)sizeof(double), SEEK_CUR);
                fwrite(&arg.grade, sizeof(double), 1, fp);
                found = true;
                break;
            }
        }
    }
    fclose(fp);

    if (found == false)
        printf("Студент с номером зачетки '%d' и фамилией '%s' не найден.\n", arg.num, arg.fam);
    else
        printf("Файл успешно изменен.\n");

    // 3. Завершает свою работу
    printf("\n\t--- 3 ---\n\n");
    printf("---| Процесс завершен |---\n\n");
    printf("_____________________________________________________________________\n");
    printf("___ДЛЯ ПРОДОЛЖЕНИЯ\n");
    printf("____Hажмите-<ENTER>");
    ch = getch();

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);

    return 0;
}
