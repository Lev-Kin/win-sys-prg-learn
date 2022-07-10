#include <windows.h>
#include <iostream>

using namespace std;

int trialCheck();
int trialSet(int);

HKEY phkResult = NULL;

int main()
{
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    system("cls");
    cout << "    +++ Практическая работа №6_1 +++" << endl
         << endl;

    int trial;
    int new_trial;
    char choice;

    // Проверяем сколько осталось попыток запуска программы
    trial = trialCheck();
    cout << "---------------------------------------" << endl;
    if (trial > 0)
        trialSet(--trial); // Устанавливаем на 1 попытку меньше
    else
    {
        cout << "Пробные запуски программы израсходованы." << endl;
        // Удаляет подраздел и его значения из указанного представления реестра для конкретной платформы.
        // Если функция завершается успешно, возвращается значение ERROR_SUCCESS.
        LONG lResult = RegDeleteKeyExA(
            HKEY_CURRENT_USER, // Имя открываемого подраздела реестра.
            // Имя удаляемого ключа.
            "SOFTWARE\\TestDir", // Этот ключ должен быть подразделом ключа, указанного значением параметра hKey.
            // Маска доступа определяет вид реестра для конкретной платформы.
            KEY_WOW64_32KEY, // Удалите ключ из 32-разрядного представления реестра.
            0                // Этот параметр зарезервирован и должен быть равен нулю.
        );

        if (lResult == ERROR_SUCCESS)
            cout << "Ключ успешно удалён." << endl;
        else
            cout << "Ошибка удаления ключа." << endl;

        return 0;
    }

    cout << "Оставшихся запусков: " << trial << endl;
    cout << "Изменить значение? (Y/N) ";
    cin >> choice;
    while (cin.get() != '\n')
    {
        continue;
    }

    if (choice == 'Y' || choice == 'y')
    {
        int valueTypeI;
        do
        {
            cout << endl
                 << "Введите новое значение: ";
            cin >> valueTypeI;
            cin.clear();
            while (cin.get() != '\n')
            {
                continue;
            }
            cin.ignore(cin.rdbuf()->in_avail());
            cin.clear();

        } while (valueTypeI <= 0);
        new_trial = valueTypeI;

        trialSet(new_trial);
    }
    else
    {
        cout << endl;
        cout << "   .........................." << endl;
        cout << "   |       До свидания      |" << endl;
        cout << "   ''''''''''''''''''''''''''" << endl
             << endl;

        cout << "_ПРОГРАММА ЗАВЕРШЕНА" << endl;
        cout << "_____Hажмите-<ENTER>";
        cin.get();
        cout << endl;
    }

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);

    return 0;
}

// Функция проверки значения реестра
int trialCheck()
{
    DWORD pcbData = sizeof(DWORD);
    DWORD quantity_key = 10;
    char choice;

    // Если функция завершается успешно, возвращается значение ERROR_SUCCESS.
    LONG lResult = RegOpenKeyExA // Открывает указанный раздел реестра (не создает ключей).
        (
            HKEY_CURRENT_USER,   // Имя открываемого подраздела реестра.
            "SOFTWARE\\TestDir", // Расположения элемента в реестре (имя подраздела вложенной ветки реестра).
            0,                   // Определяет параметр, применяемый при открытии ключа.
            KEY_ALL_ACCESS,      // Маска, определяющая желаемые права доступа к открываемому ключу.
            &phkResult           // Указатель на переменную, которая получает дескриптор открытого ключа.
        );

    if (lResult != ERROR_SUCCESS)
    {
        if (lResult == ERROR_FILE_NOT_FOUND)
        {
            cout << "Ключ не найден!" << endl;
            cout << "Создать? (Y/N) ";
            cin >> choice;
            while (cin.get() != '\n')
            {
                continue;
            }

            if (choice == 'Y' || choice == 'y')
            {
                // Создает указанный раздел реестра. Если ключ уже существует, функция открывает его.
                // Если функция завершается успешно, возвращается значение ERROR_SUCCESS.
                lResult = RegCreateKeyExA(
                    HKEY_CURRENT_USER,   // Имя открытого ключа реестра.
                    "SOFTWARE\\TestDir", // Входящий параметр, имя подраздела (вложенной ветки реестра).
                    0,                   // Этот параметр зарезервирован и должен быть 0.
                    NULL,                // Входящий параметр, определенный пользователем тип класса ключа реестра.
                    // REG_OPTION_NON_VOLATILE Все ключи, созданные функцией, непостоянны.
                    // Информация хранится в памяти и не сохраняется при выгрузке соответствующего куста реестра.
                    REG_OPTION_NON_VOLATILE, // входящий параметр, может принимать одно из зарезервированных значений
                    KEY_ALL_ACCESS,          // Маска, определяющая права доступа для создаваемого ключа.
                    // Указатель на структуру SECURITY_ATTRIBUTES, которая определяет,
                    // может ли возвращаемый дескриптор быть унаследован дочерними процессами.
                    NULL, // Если этот параметр NULL - дескриптор не наследуется.
                    // Указатель на переменную, которая получает дескриптор открытого или созданного ключа.
                    &phkResult,
                    NULL // Указатель на переменную, которая получает одно из следующих значений расположения.
                );

                cout << "---------------------------------------" << endl;
                if (lResult != ERROR_SUCCESS)
                {
                    cout << "Ошибка создания ключа." << endl;
                    return NULL;
                }
                else
                {
                    cout << "Ключ создан!" << endl;

                    // Устанавливает данные и тип указанного значения в разделе реестра.
                    // Если функция завершается успешно, возвращается значение ERROR_SUCCESS.
                    lResult = RegSetValueExA(
                        phkResult,  // Дескриптор открытого раздела реестра.
                        "TestRuns", // Имя устанавливаемого значения.
                        0,          // Этот параметр зарезервирован и должен быть равен нулю.
                        // Входящий параметр, определяющий какого типа будет создаваемое значение.
                        REG_DWORD, // 32-битное число.
                        // Входящий необязательный параметр, содержит в себе данные, которые
                        // будут записаны в значение реестра.
                        (BYTE *)&quantity_key,
                        sizeof(DWORD) // Pазмер данных (в байтах) которые будут записаны в значение реестра.
                    );
                }
            }
            else
                return NULL;
        }
        else
        {
            cout << "Ошибка открытия." << endl;
            return NULL;
        }
    }
    else
    {
        // Извлекает тип и данные для указанного значения реестра.
        // сли функция завершается успешно, возвращается значение ERROR_SUCCESS.
        lResult = RegGetValueA(
            HKEY_CURRENT_USER,   // Входящий параметр, описатель открытого ключа реестра.
            "SOFTWARE\\TestDir", // Путь к ключу реестра относительно ключа, указанного параметром hkey.
            "TestRuns",          // Имя значения реестра.
            // Флаги, ограничивающие тип данных запрашиваемого значения.
            RRF_RT_REG_DWORD, // Ограничьте тип до REG_DWORD.
            NULL,             // Указатель на переменную, которая получает код, указывающий тип данных, хранящихся в указанном значении.
            &quantity_key,    // Указатель на буфер, который получает данные значения.
            &pcbData          // Указатель на переменную, определяющую размер буфера, на который указывает параметр pvData, в байтах.
        );
        if (lResult != ERROR_SUCCESS)
        {
            lResult = RegSetValueExA(phkResult, "TestRuns", 0, REG_DWORD, (BYTE *)&quantity_key, sizeof(DWORD));
        }
    }
    return quantity_key;
}

// Функция установки значения реестра
int trialSet(int new_trial)
{
    DWORD pcbData = sizeof(DWORD);
    DWORD quantity_key = new_trial;

    // Открываем ветку реестра
    LONG lResult = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\TestDir", 0, KEY_ALL_ACCESS, &phkResult);
    if (lResult != ERROR_SUCCESS)
    {
        cout << "Ошибка открытия.";
        return NULL;
    }
    else
    {
        // Получаем значение из параметра реестра
        lResult = RegSetValueExA(phkResult, "TestRuns", 0, REG_DWORD, (BYTE *)&quantity_key, sizeof(DWORD));
        if (lResult != ERROR_SUCCESS)
        {
            cout << "Ошибка установки значения." << endl;
        }
    }
    return quantity_key;
}
