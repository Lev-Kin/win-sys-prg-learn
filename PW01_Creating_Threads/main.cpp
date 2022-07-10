// Разработать программу для консольного процесса, который состоит из двух
// потоков: main и worker.

// Поток main должен выполнить следующие действия:
// 1. Создать массив целых чисел, размер и элементы которого вводятся с
//    консоли.
// 2. Создать поток worker.
// 3. Найти минимальный и максимальный элементы массива и вывести их на консоль.
//    После каждого сравнения элементов «спать» 7 миллисекунд.
// 4. Дождаться завершения потока worker.
// 5. Подсчитать количество элементов в массиве, значение которых больше среднего
//     значения элементов массива, и вывести его на консоль.
// 6. Завершить работу.

// Поток worker должен выполнить следующие действия:
// 1. Работа выполняется согласно индивидуальному заданию. После каждой
//    итерации с элементами «спать» 12 миллисекунд.
// 2. Завершить свою работу.

// Поток worker должен найти количество нечётных элементов массива.

#include <windows.h>
#include <stdio.h>
#include <limits.h> // INT_MAX

using namespace std;

int n;

int worker(int *array)
{
    int k = 0; // количество нечетных элементов в массиве
    printf("Начала работы потока worker.\n\n");
    printf("Массив в потоке:\n");
    for (int i = 0; i < n; i++)
    {
        printf("A[%d] = %d\n", i, array[i]);
    }

    printf("\n----------------------------------------------\n");
    printf("Проводим вычисления в потоке согласно заданию.\n");
    for (int i = 0; i < n; i++)
    {
        if (array[i] % 2 != 0)
        {
            k++;
        }
        Sleep(12);
    }
    printf("Количество нечетных элементов = %d", k);
    printf("\n----------------------------------------------\n");
    return 0;
}

int main()
{
    system("cls");
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("\t\t+++ Практическая работа №1 +++\n\n\n");
    printf("\t=== 1 ===\n");
    while (n < 3 || n > 10)
    {
        printf("\nВведите размер массива (3 - 10): ");
        scanf_s("%d", &n);
    }

    int *array = new int[n];
    if (!array)
    {
        printf("Память для массива не выделена.\n");
        printf("Программа будет закрыта.\n");
    }
    else
    {
        HANDLE hThread;
        DWORD IDThread;

        int max = INT_MIN;
        int min = INT_MAX;

        printf("Массив A[%d] создан.\n\n", n);
        printf("\t=== 2 ===\n\n");
        printf("Заполните созданный массив целыми числами:\n");
        for (int i = 0; i < n; i++)
        {
            printf("A[%d] = ", i);
            scanf_s("%d", &array[i]);
        }

        for (int i = 0; i < n; i++)
        {
            if (max < array[i])
            {
                max = array[i];
                Sleep(7);
            }
            if (min > array[i])
            {
                min = array[i];
                Sleep(7);
            }
        }
        printf("\n\t=== 3 ===\n\n");
        printf("Максимальный элемент массива = %d\n", max);
        printf("Минимальный  элемент массива = %d\n", min);

        printf("\n\t=== 4 ===\n\n");
        // запускаем поток worker
        hThread = CreateThread(NULL,                           // атрибуты защиты
                               0,                              // размер стека потока в байтах
                               (LPTHREAD_START_ROUTINE)worker, // адрес исполняемой функции
                               array,                          // адрес параметра
                               0,                              // флаги создания потока
                               &IDThread);                     // идентификатор потока

        if (hThread == NULL)
            return GetLastError();

        // ждем, пока поток worker закончит работу
        WaitForSingleObject(hThread, INFINITE);
        printf("\nПоток worker завершил работу.\n");

        // закрываем дескриптор потока worker
        CloseHandle(hThread);

        printf("\n\t=== 5 ===\n\n");

        int sum = 0;
        int count = 0;
        double middle = 0;
        for (int i = 0; i < n; i++)
        {
            sum += array[i];
        }
        printf("Среднее значение элементов массива = %lg.\n", middle = ((double)sum / n));

        printf("Элемент(ы) больше среднего значения массива: ");
        for (int i = 0; i < n; i++)
        {
            if (array[i] > middle)
            {
                printf("%d | ", array[i]);
                count++;
            }
        }
        printf("\nКоличество элементов в массиве, значение которых больше среднего = %d\n", count);

        delete[] array;
    }

    printf("\n\t=== 6 ===\n\n");
    printf("Работа программы завершена.\n");
    printf("| До свидания |\n\n");
    system("pause");

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);

    return 0;
}
