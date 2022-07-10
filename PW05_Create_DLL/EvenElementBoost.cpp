#include <windows.h>
#include <stdio.h>

// главная функция projectDLL
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH: // загрузка DLL
        break;
    case DLL_THREAD_ATTACH: // создание потока
        break;
    case DLL_PROCESS_DETACH: // завершение потока
        break;
    case DLL_THREAD_DETACH: // отключение DLL
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) int array_size = 0;

extern "C" __declspec(dllexport) int Add(int size)
{
    array_size = size;
    return array_size;
}

extern "C" __declspec(dllexport) int printArray(int *array)
{
    printf("A[%2d] = {", array_size);
    for (int i = 0; i < array_size; i++)
    {
        printf("%4d", array[i]);
        if (i + 1 < array_size)
            printf(",");

        if ((i + 1) % array_size == 0)
            printf("}\n\n");

        if ((i + 1) % 5 == 0)
            printf("\n\t ");
    }
    printf("\n");

    return 0;
}

extern "C" __declspec(dllexport) int evenElementBoost(int *&array)
{
    for (int i = 0; i < array_size; i++)
        if (array[i] % 2 == 0)
            array[i] = array[i] * 2;

    return 0;
}
