// Написать программы двух консольных процессов Parent и Child,
// которые выполняют следующие действия:

// Процесс Parent:
// 1. Создаёт бинарный файл, записи которого имеют следующую структуру:

// struct emp
//  {
//   int num;              // номер зачётки
//   char fam[30];         // фамилия студента
//   char first_name[30];  // имя студента
//   char second_name[30]; // отчество студента
//   double grade;         // средний бал
//  };

//     Имя файла и данные о студентах вводятся с консоли.
// 2. Выводит созданный файл на консоль.
// 3. Запрашивает с консоли номер зачётки, фамилию, имя и отчество студента и
//    новый средний бал этого студента.
// 4. Формирует командную строку, которая содержит имя созданного файла и
//    информацию, полученную в пункте 3.
// 5. Запускает дочерний процесс Child, которому как параметр передаётся командная
//    строка, сформированная в пункте 4.
// 6. Ждёт завершения работы процесса Child.
// 7. Выводит откорректированный файл на консоль.
// 8. Завершает свою работу.

// Процесс Child:
// 1. Выводит на консоль информацию, полученную через командную строку.
// 2. Корректирует в файле, созданном процессом Parent, нужную запись, т.е.
//    устанавливает новый средний бал студента.
// 3. Завершает свою работу.

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

struct Database fillStudent();
void write(FILE *output);
void read(FILE *input);

int main()
{
    system("cls");
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE *fp = NULL;
    char file_name[LENGTH] = {"\0"};
    char ch;

    printf("\t\t+++ Практическая работа №2 +++\n\n\n");

    // Процесс Parent:
    // 1. Создаёт бинарный файл
    printf("\t=== 1 ===\n\n");
    do
    {
        printf("\nВведите имя файла (до 29 алфавитно-цифровых символов):\n --> ");
        scanf_s("%30s", &file_name, LENGTH);
        while (getchar() != '\n')
            continue;

        if ((int)strlen(file_name) == LENGTH)
        {
            printf("\nНекорректный ввод (длинное имя файла)\n");
            printf("Пожалуйста повторите ввод\n");
        }

    } while (file_name[0] == '\0' || (int)strlen(file_name) == LENGTH);

    fp = fopen(file_name, "wb");
    if (fp == NULL)
    {
        printf("Ошибка открытия файла\n");
        ch = getch();
        exit(ERROR_FILE_OPEN);
    }
    write(fp);
    fclose(fp);

    // 2. Выводит созданный файл на консоль
    system("cls");
    printf("\t=== 2 ===\n\n");

    fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("Ошибка чтения файла\n");
        ch = _getch();
        exit(ERROR_FILE_OPEN);
    }
    read(fp);
    fclose(fp);

    // 3. Запрашивает с консоли номер зачётки,
    //    фамилию, имя и отчество студента и новый средний балл этого студента
    printf("\n\n");
    printf("\t=== 3 ===\n\n");
    printf("Заполняем данные для какого студента поменять средний балл.\n");
    struct Database student = fillStudent();

    // 4. Формирует командную строку, которая содержит имя созданного файла и
    //    информацию, полученную в пункте 3.
    printf("\n\t=== 4 ===\n\n");
    printf("Данные переданы в командную строку.\n");
    char str[80]; // command line
    sprintf_s(str, "Child.exe %s %d %s %s %s %lf",
              file_name, student.num,
              student.fam, student.first_name, student.second_name, student.grade);

    // 5. Запускает дочерний процесс Child, которому как параметр
    //    передаётся командная строка, сформированная в пункте 4.
    printf("\n\t=== 5 ===\n\n");
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Заполняем нулями поля структуры si
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    // Создаем новый консольный процесс
    if (CreateProcessA(
            NULL,               // имя исполняемого модуля (if null use command line)
            str,                // аргументы командной строки
            NULL,               // атрибуты защиты процесса
            NULL,               // атрибуты защиты потока
            FALSE,              // это наследует дескриптор
            CREATE_NEW_CONSOLE, // атрибуты флагов создания процесса
            NULL,               // указатель пути окружающей среды
            NULL,               // текущий каталог
            &si,                // информация о запуске
            &pi                 // информация о процессе и потоке
            ) == NULL)
    {
        printf("Ошибка !!!\n");
        printf("Новый процесс не создан.\n");
        printf("Нажмите любую клавишу, чтобы закончить.\n");
        ch = getch();
        return -1;
    }
    else
        printf("Новый процесс создан.\n\n");

    // 6. Ждёт завершения работы процесса Child
    printf("\t=== 6 ===\n\n");
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закрываем дескрипторы этого процесса
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    printf("Новый процесс закончил работу и закрыт.\n");
    printf("Продолжить нажмите-<ENTER>");
    ch = getch();

    // 7. Выводит откорректированный файл на консоль
    printf("\n\n\t=== 7 ===\n\n");

    fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("Ошибка чтения файла\n");
        ch = _getch();
        exit(ERROR_FILE_OPEN);
    }
    printf("\t\t\t   | ИЗМЕНЕННЫЙ\n");
    read(fp);
    fclose(fp);

    // 8. Завершает свою работу
    printf("\n\t=== 8 ===\n\n");
    printf("\t| До свидания |\n\n");
    printf("=====================================================================\n");
    printf("_ПРОГРАММА ЗАВЕРШЕНА\n");
    printf("_____Hажмите-<ENTER>");
    ch = getch();

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);
    return 0;
}

struct Database fillStudent()
{
    struct Database student;
    int correct_number;
    char correct_[LENGTH] = {"\0"};
    double correct_grade;

    printf("\n.    .    .    .    .    .    .    .    .    .    .    .    .    .\n\n");
    printf("Введите данные о студенте:\n");
    do
    {
        while (1)
        {
            char ch = 0;
            printf("\n1.  Номер зачетки (не более 5 цифр)\n --> ");

            if (!scanf_s("%d", &correct_number) || getchar() != '\n')
            {
                printf("\nНекорректный ввод (только целые числа)\n");
                printf("Пожалуйста повторите ввод\n");
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
            }
            else
                break;
        }

    } while (correct_number < 0 || correct_number > 10000);
    student.num = correct_number;

    do
    {
        printf("\n2.  Фамилия студента (не более 29 букв)\n --> ");
        scanf_s("%30s", correct_, LENGTH);
        while (getchar() != '\n')
            continue;

        if ((int)strlen(correct_) == LENGTH)
        {
            printf("\nНекорректный ввод (длина фамилии превышена)\n");
            printf("Пожалуйста повторите ввод\n");
        }

    } while (correct_[0] == '\0' || (int)strlen(correct_) == LENGTH);
    strcpy(student.fam, correct_);

    do
    {
        printf("\n3.  Имя студента (не более 29 букв)\n --> ");
        scanf_s("%30s", correct_, LENGTH);
        while (getchar() != '\n')
            continue;

        if ((int)strlen(correct_) == LENGTH)
        {
            printf("\nНекорректный ввод (длина имени превышена)\n");
            printf("Пожалуйста повторите ввод\n");
        }

    } while (correct_[0] == '\0' || (int)strlen(correct_) == LENGTH);
    strcpy(student.first_name, correct_);

    do
    {
        printf("\n4.  Отчество студента (не более 29 букв)\n --> ");
        scanf_s("%30s", correct_, LENGTH);
        while (getchar() != '\n')
            continue;

        if ((int)strlen(correct_) == LENGTH)
        {
            printf("\nНекорректный ввод (длина отчества превышена)\n");
            printf("Пожалуйста повторите ввод\n");
        }

    } while (correct_[0] == '\0' || (int)strlen(correct_) == LENGTH);
    strcpy(student.second_name, correct_);

    do
    {
        while (1)
        {
            char ch = 0;
            printf("\n5.  Средний балл (от 0 до 10)\n --> ");

            if (!scanf_s("%lf", &correct_grade) || getchar() != '\n')
            {
                printf("\nНекорректный ввод (от 0 до 10)\n");
                printf("Пожалуйста повторите ввод\n");
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
            }
            else
                break;
        }

    } while (correct_grade < 0 || correct_grade > 10);
    student.grade = correct_grade;

    return student;
}

void write(FILE *output)
{
    char repeat;
    do
    {
        Database student = fillStudent();
        fwrite(&student, sizeof(Database), 1, output);

        printf("\nДобавить еще студента?\nY/N --> ");
        repeat = getchar();
    } while (repeat == 'y' || repeat == 'Y');
}

void read(FILE *input)
{
    Database student;
    int count_student = 1;

    printf("\t\t\t___| Список студентов находящийся в базе |___\n");
    while (fread(&student, sizeof(Database), 1, input) != NULL)
    {
        printf("\t-------------------------------------------------------------------\n");
        printf("\t\t === %d-й студент ===\n", count_student++);
        printf("\t1.     Номер зачетки:\t%d\n", student.num);
        printf("\t2.  Фамилия студента:\t%s\n", student.fam);
        printf("\t3.      Имя студента:\t%s\n", student.first_name);
        printf("\t4. Отчество студента:\t%s\n", student.second_name);
        printf("\t5.       Средний бал:\t%.1f\n", student.grade);
        printf("\t-------------------------------------------------------------------\n");
    }
}

