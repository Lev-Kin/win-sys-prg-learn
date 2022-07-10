// Общее условие задачи.
// Написать программу для консольного процесса,
// который состоит из трёх потоков:
// main, MyThread_Work, MyThread_SumElement.

// Поток main должен выполнить следующие действия:
// 1. Создать массив, размерность и элементы которого вводятся пользователем с
//    консоли. Элементы массива - вещественные числа;
// 2. Вывести размерность и элементы исходного массива на консоль;
// 3. Запустить поток MyThread_Work;
// 4. Запустить поток MyThread_SumElement;
// 5. Освобождать выходной поток stdout после вывода на консоль каждого
//    нового элемента массива.
// 6. Выводить на экран поэлементно элементы массива (итогового) параллельно с
//    работой потока MyThread_Work;
// Поток MyThread_Work должен выполнить следующие действия
// (Для синхронизации с потоком main использовать семафор либо критическую секцию).
// 1. Запросить у пользователя временной интервал, требуемый для отдыха после
//    подготовки одного элемента в массиве;
// 2. Найти в массиве повторяющиеся элементы (разместить их группы в массиве
//    слева, остальные соответственно справа);
// 3. Извещать поток main о новом элементе;
// 4. После каждого готового элемента отдыхать в течение заданного интервала
//    времени;
// 5. Известить поток MyThread_SumElement о начале суммирования
//    (момент запуска произойдёт после того, будет сформирован итоговый массив.)
// Поток MyThread_SumElement должен выполнить следующие действия
// (Для синхронизации с потоком MyThread_Work использовать критическую секцию!):
//  1. Ждёт от потока MyThread_Work сигнал о начале суммирования;
//  2. Выполнить суммирование элементов итогового массива;
//  3. Вывести итоговую сумму.

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

volatile int g_array_size;
volatile double *g_array;

HANDLE hSemaphore;
CRITICAL_SECTION cs;

DWORD WINAPI MyThreadWork(LPVOID);
DWORD WINAPI MyThreadSumElement(LPVOID);

int main()
{
    system("cls");
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    printf("\t\t+++ Практическая работа №3 +++\n\n");

    HANDLE hThread;
    HANDLE hThread_1;
    DWORD IDThread;
    DWORD IDThread_1;

    // инициализируем критическую секцию
    InitializeCriticalSection(&cs);

    int array_size = 0;
    double element = 0.0;

    // 1. Создать массив, размер и элементы которого вводятся пользователем с консоли.
    //    Элементы массива - вещественные числа;
    printf("\n\t=== 1 ===\t| main |\n");
    do
    {
        printf("\nВведите размер массива (5 - 15): ");
        scanf_s("%d", &array_size);
        while (getchar() != '\n')
            continue;

    } while (array_size < 5 || array_size > 15);
    g_array_size = array_size;

    g_array = new double[g_array_size];
    if (!g_array)
    {
        printf("Память для массива не выделена.\n");
        printf("Программа будет закрыта.\n");
        exit(0);
    }

    printf("Массив A[%d] создан.\n\n", g_array_size);
    printf("Заполните созданный массив числами:\n");
    for (int i = 0; i < g_array_size; i++)
    {
        do
        {
            while (1)
            {
                char ch = 0;
                printf("A[%d] = ", i + 1);

                if (!scanf_s("%lf", &element) || getchar() != '\n')
                {
                    printf("\nНекорректный ввод (только числа)\n");
                    printf("Пожалуйста повторите ввод\n\n");
                    while ((ch = getchar()) != '\n' && ch != EOF)
                        ;
                }
                else
                    break;
            }

        } while (element < 0 || element >= 100);
        g_array[i] = element;
    }

    // 2. Вывести размерность и элементы исходного массива на консоль;
    printf("\n\t=== 2 ===\t| main |\n\n");
    printf("Исходный массив:\n");
    printf("A[%d] = {", g_array_size);
    for (int i = 0; i < g_array_size; i++)
    {
        printf("%.2lf", g_array[i]);
        if (i + 1 < g_array_size)
            printf(", ");
        if ((i + 1) % g_array_size == 0)
            printf("}\n\n");
    }

    // 3. Запустить поток MyThread_Work;
    printf("\t=== 3 ===\t| main |\n\n");
    printf("Создаем семафор и поток и запускаем поток MyThread_Work.\n\n");
    // создаем семафор
    hSemaphore = CreateSemaphoreA(
        NULL,         // атрибуты защиты
        0,            // начальное значение семафора
        g_array_size, // максимальное значение семафора
        NULL          // имя семафора
    );
    if (hSemaphore == NULL)
        return GetLastError();

    // создаем поток, который готовит элементы массива
    hThread = CreateThread(
        NULL,
        0,
        MyThreadWork, // адрес исполняемой функции
        NULL,
        0,
        &IDThread // идентификатор потока
    );
    if (hThread == NULL)
        return GetLastError();

    // 4. Запустить поток MyThread_SumElement;
    printf("\t=== 4 ===\t| main |\n\n");
    printf("Создаем поток и запускаем поток MyThread_SumElement.\n\n");
    hThread_1 = CreateThread(NULL, 0, MyThreadSumElement, NULL, 0, &IDThread_1);
    if (hThread == NULL)
        return GetLastError();

    // 5. Освобождать выходной поток stdout после вывода на консоль каждого
    //    нового элемента массива.
    // 6. Выводить на экран поэлементно элементы массива(итогового) параллельно с
    //    работой потока MyThread_Work;
    EnterCriticalSection(&cs);
    for (int i = 0; i < g_array_size; i++)
    {
        WaitForSingleObject(hSemaphore, INFINITE);

        printf("\t=== 5 ===\t| main |\n");
        printf("\t=== 6 ===\t| main |\n");
        printf("В потоке main подготовлен ------->\t\tA[%d] = %.2lf\n\n\n", i + 1, g_array[i]);
        fflush(stdout);

        if ((i + 1) % g_array_size == 0)
        {
            printf("Обработанный массив:\n");
            printf("A[%d] = {", g_array_size);
            for (int i = 0; i < g_array_size; i++)
            {
                printf("%.2lf", g_array[i]);
                fflush(stdout);

                if (i + 1 < g_array_size)
                    printf(", ");
                if ((i + 1) % g_array_size == 0)
                    printf("}\n\n");

                fflush(stdout);
            }

            printf("\t--- 5 ---\t| MyThreadWork |\n\n");
            printf("Итоговый массив сформирован.\n\n\n");
            fflush(stdout);
        }
    }
    LeaveCriticalSection(&cs);

    // 1. Ждёт от потока MyThread_Work сигнал о начале суммирования
    printf("\t--- 1 ---\t| MyThread_SumElement |\n\n");
    printf("Поток MyThread_Work закончил свою работу.\n\n");
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hSemaphore);
    // удаляем критическую секцию
    DeleteCriticalSection(&cs);

    delete[] g_array;
    g_array = nullptr;
    g_array_size = 0;

    printf("\t| До свидания |\n\n");
    printf("=====================================================================\n");
    printf("_ПРОГРАММА ЗАВЕРШЕНА\n");
    printf("_____Hажмите-<ENTER>");
    char ch = getch();

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);
    return 0;
}

DWORD WINAPI MyThreadWork(LPVOID)
{
    int sleep;
    // 1. Запросить у пользователя временной интервал, требуемый для отдыха после
    //	 подготовки одного элемента в массиве;
    printf("\t--- 1 ---\t| MyThreadWork |\n");
    do
    {
        printf("\nВведите временной интервал (не менее 17 миллисекунд):\n -->");
        scanf_s("%d", &sleep);
        while (getchar() != '\n')
            continue;

        if (sleep >= 10000)
        {
            printf("\nЗадан максимальный интервал отдыха (1000 миллисекунд)\n\n");
            sleep = 1000;
        }

    } while (sleep < 17);
    //------------------------------------------------------------

    bool similar_element;
    bool shift;

    double temp = 0.0;
    int different_element = 0;
    int step = 0;
    int position = 0;
    int i;
    // 2. Найти в массиве повторяющиеся элементы
    // (разместить их в массиве слева, остальные соответственно справа).
    printf("\n\t--- 2 ---\t| MyThreadWork |\n\n");
    printf("Находим в массиве повторяющийся элементы.\n\n");
    for (int i = 0; i < g_array_size; i++)
    {
        similar_element = false;
        for (int j = 0; j < g_array_size; j++)
        {
            if (j == i)
                continue;
            if (g_array[j] == g_array[i])
            {
                similar_element = true;
                break;
            }
        }
        if (similar_element == false)
            different_element++;
    }

    while (step < different_element)
    {
        shift = false;
        for (i = position; i < g_array_size; i++)
        {
            similar_element = false;
            for (int j = 0; j < g_array_size; j++)
            {
                if (j == i)
                    continue;
                if (g_array[j] == g_array[i])
                {
                    similar_element = true;
                    break;
                }
            }

            if (similar_element == false)
            {
                for (int j = i; j < g_array_size - 1; j++)
                {
                    temp = g_array[j];
                    g_array[j] = g_array[j + 1];
                    g_array[j + 1] = temp;
                }
                shift = true;
            }
        }

        if (shift == true)
        {
            step++;
            i = position;
        }
    }

    for (int i = 0; i < g_array_size; i++)
    {
        for (int j = i; j < g_array_size; j++)
        {
            if (g_array[j] == g_array[i])
            {
                for (int k = j; k > i; k--)
                {
                    temp = g_array[k];
                    g_array[k] = g_array[k - 1];
                    g_array[k - 1] = temp;
                }
            }
        }

        // 3. Извещать поток main о новом элементе;
        printf("\t--- 3 ---\t| MyThreadWork |\n");
        printf("В потоке MyThreadWork обработан ------->\tА[%d] элемент.\n", i + 1);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        // 4. После каждого готового элемента
        //    отдыхать в течение заданного интервала времени;
        printf("\t--- 4 ---\t| MyThreadWork |\n");
        printf("А[%d] готов отдых в течении %d миллисекунд.\n", i + 1, sleep);
        Sleep(sleep);
    }
    return 0;
}

DWORD WINAPI MyThreadSumElement(LPVOID)
{
    EnterCriticalSection(&cs);
    double sum = 0;
    // 2. Выполнить суммирование элементов итогового массива;
    printf("\t--- 2 ---\t| MyThread_SumElement |\n\n");
    printf("Поток MyThread_SumElement суммирует элементы итогового массива.\n\n");
    for (int i = 0; i < g_array_size; i++)
    {
        sum += g_array[i];
    }
    // 3. Вывести итоговую сумму.
    printf("\t--- 3 ---\t| MyThread_SumElement |\n\n");
    printf("Сумма элементов массива = %.2lf\n\n", sum);
    LeaveCriticalSection(&cs);
    return 0;
}
