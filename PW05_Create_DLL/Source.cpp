// Создать DLL, содержащую функцию, определяющую:
// удваивающую все чётные элементы. (умножать на 2)

#include <windows.h>
#include <stdio.h>
#include <conio.h>
using namespace std;

int main()
{
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    system("cls");
    printf("    +++ Практическая работа №5 +++\n\n");

    HMODULE hDll; // дескриптор DLL

    int *array_size; // для адреса переменной array_size из DLL
    int (*Add)(int);
    int (*evenElementBoost)(int *&);
    int (*printArray)(int *);

    int size;
    int *array = nullptr;
    char any_key;

    ////////// load dll (address exported function) //////////////////
    hDll = LoadLibrary(L"EvenElementBoost.dll");
    if (hDll == NULL)
    {
        printf("Ошибка загрузки библиотеки.\n");
        return GetLastError();
    }
    array_size = (int *)GetProcAddress(hDll, "array_size");
    if (array_size == NULL)
    {
        printf("Ошибка получения адреса переменной.\n");
        return GetLastError();
    }
    Add = (int (*)(int))GetProcAddress(hDll, "Add");
    if (Add == NULL)
    {
        printf("Ошибка получения адреса процедуры.\n");
        return GetLastError();
    }
    evenElementBoost = (int (*)(int *&))GetProcAddress(hDll, "evenElementBoost");
    if (evenElementBoost == NULL)
    {
        printf("Ошибка получения адреса процедуры.\n");
        return GetLastError();
    }
    printArray = (int (*)(int *))GetProcAddress(hDll, "printArray");
    if (printArray == NULL)
    {
        printf("Ошибка получения адреса процедуры.\n");
        return GetLastError();
    }
    /////////////////////////////////////////////////////////////////

    printf("=== Инсталляция массива ===\n");
    do
    {
        while (1)
        {
            char ch = 0;
            printf("\nВведите размер массива (5 - 20):\n --> ");

            if (!scanf_s("%d", &size) || getchar() != '\n')
            {
                printf("\nНекорректный ввод (только целые числа)\n");
                printf("Пожалуйста повторите ввод\n");
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
            }
            else
                break;
        }

    } while (size < 5 || size > 20);
    Add(size);

    array = new int[size];
    if (array)
    {
        int element;

        printf("Массив A[%d] создан.\n\n", size);
        printf("Заполните созданный массив числами:\n");
        for (int i = 0; i < size; i++)
        {
            do
            {
                while (1)
                {
                    char ch = 0;
                    printf("A[%2d] = ", i + 1);

                    if (!scanf_s("%d", &element) || getchar() != '\n')
                    {
                        printf("\nНекорректный ввод (только целые числа)\n");
                        printf("Пожалуйста повторите ввод\n\n");
                        while ((ch = getchar()) != '\n' && ch != EOF)
                            ;
                    }
                    else
                        break;
                }

            } while (element <= -100 || element >= 100);
            array[i] = element;
        }

        printf("Массив успешно сформирован.\n\n");
    }
    else
    {
        printf("Память не выделена.\n");
        printf("Программа будет закрыта.\n");
        printf("________Hажмите-<ENTER>");
        any_key = getch();
        exit(0);
    }

    printf("=== Вывод исходного массива через .dll ===\n");
    printArray(array);

    printf("=== Удваиваем все четные элементы массива через .dll ===\n");
    evenElementBoost(array);

    printf("=== Вывод обработанного массива через .dll ===\n");
    printArray(array);

    // отключаем библиотеку
    if (FreeLibrary(hDll) == FALSE)
    {
        printf("Ошибка освобождения библиотеки.\n");
        return GetLastError();
    }

    delete[] array;
    array = nullptr;

    printf("\n");
    printf("   ..........................\n");
    printf("   |       До свидания      |\n");
    printf("   ''''''''''''''''''''''''''\n\n");

    printf("_ПРОГРАММА ЗАВЕРШЕНА\n");
    printf("_____Hажмите-<ENTER>");
    any_key = getch();
    printf("\n");

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);

    return 0;
}
