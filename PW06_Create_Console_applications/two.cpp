#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

HKEY phkResult = NULL;

int CreateKey(HKEY hRoot)
{
    char choice;

    // Попытка открыть ключ реестра
    LONG lResult = RegOpenKeyExA(hRoot, "SOFTWARE\\TestDir", 0, KEY_ALL_ACCESS, &phkResult);
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
                // Попытка создания ключа реестра
                lResult = RegCreateKeyExA(hRoot, "SOFTWARE\\TestDir", 0,
                                          NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &phkResult, NULL);
                if (lResult != ERROR_SUCCESS)
                {
                    cout << "Ошибка создания ключа." << endl;
                    RegCloseKey(phkResult);
                    return -1;
                }
                else
                {
                    cout << "Ключ успешно создан." << endl;
                    return lResult;
                }
            }
            else
            {
                RegCloseKey(phkResult);
                return -1;
            }
        }
        else
        {
            cout << "Ошибка открытия." << endl;
            RegCloseKey(phkResult);
            return -1;
        }
    }
    else
    {
        cout << "Ключ успешно открыт." << endl;
        return lResult;
    }
}

// Tип данных int
int valueTypeI()
{
    int a;
    cout << "> ";
    cin >> a;
    cin.clear();
    while (cin.get() != '\n')
    {
        continue;
    }
    cin.ignore(cin.rdbuf()->in_avail());
    cin.clear();
    return a;
}

int main()
{
    int i1 = GetConsoleCP();
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HKEY hRoot = NULL;

    int menu;
    int select;
    int ask;

    bool key_classes_r;
    bool key_current_u;
    bool key_local_m;
    bool key_u;

    bool root_on = false;
    bool key_on = false;
    bool parameter_on = false;
    bool goback = true;
    do
    {
        system("cls");
        cout << "    +++ Практическая работа №6_2 +++" << endl
             << endl;
        cout << "   ________| ГЛАВНОЕ МЕНЮ |_________" << endl;
        cout << "1) Выбор корневого раздела реестра." << endl;
        cout << "2) Создать ключ." << endl;
        cout << "3) Удалить ключ." << endl;
        cout << "4) Создать параметр." << endl;
        cout << "5) Удалить параметр." << endl;
        cout << "6) Удалить все тестовые ключи из реестра." << endl;
        cout << "7) -----------|  ВыхоД  |-----------" << endl
             << endl;
        menu = valueTypeI();

        cout << endl;

        switch (menu)
        {
        case 1:
        {
            if (root_on == false)
            {
                cout << "=== Выбран 1 пункт меню ===" << endl
                     << endl;
                cout << "   Выберите корневой раздел:" << endl;
                cout << "1) HKEY_CLASSES_ROOT." << endl;
                cout << "2) HKEY_CURRENT_USER." << endl;
                cout << "3) HKEY_LOCAL_MACHINE." << endl;
                cout << "4) HKEY_USERS." << endl;
                cout << "5) ОТМЕНА выбора.";
                do
                {
                    cout << endl
                         << endl;
                    select = 0;
                    select = valueTypeI();

                    if (select != 1 && select != 2 && select != 3 && select != 4 && select != 5)
                    {
                        cout << endl
                             << "=> Некорректный ввод (Ответ = 5 -отмена).";
                        cout << endl
                             << "=> Пожалуйста правильно выберите раздел.";
                    }

                } while (select != 1 && select != 2 && select != 3 && select != 4 && select != 5);

                cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                if (select == 5)
                    break;
                if (select == 1)
                {
                    hRoot = HKEY_CLASSES_ROOT;
                    cout << "Выбран корневой раздел реестра HKEY_CLASSES_ROOT" << endl;
                    key_classes_r = true;
                    key_current_u = false;
                    key_local_m = false;
                    key_u = false;

                    key_on = false;
                }
                if (select == 2)
                {
                    hRoot = HKEY_CURRENT_USER;
                    cout << "Выбран корневой раздел реестра HKEY_CURRENT_USER" << endl;
                    key_classes_r = false;
                    key_current_u = true;
                    key_local_m = false;
                    key_u = false;

                    key_on = false;
                }
                if (select == 3)
                {
                    hRoot = HKEY_LOCAL_MACHINE;
                    cout << "Выбран корневой раздел реестра HKEY_LOCAL_MACHINE" << endl;
                    key_classes_r = false;
                    key_current_u = false;
                    key_local_m = true;
                    key_u = false;

                    key_on = false;
                }
                if (select == 4)
                {
                    hRoot = HKEY_USERS;
                    cout << "Выбран корневой раздел реестра HKEY_USERS" << endl;
                    key_classes_r = false;
                    key_current_u = false;
                    key_local_m = false;
                    key_u = true;

                    key_on = false;
                }

                root_on = true; // корневой каталог реестра выбран
            }
            else
            {
                cout << "=== Выбран 1 пункт меню ===";
                do
                {
                    cout << endl
                         << endl;
                    if (key_classes_r)
                        cout << "Был выбран корневой раздел реестра HKEY_CLASSES_ROOT" << endl;
                    if (key_current_u)
                        cout << "Был выбран корневой раздел реестра HKEY_CURRENT_USER" << endl;
                    if (key_local_m)
                        cout << "Был выбран корневой раздел реестра HKEY_LOCAL_MACHINE" << endl;
                    if (key_u)
                        cout << "Был выбран корневой раздел реестра HKEY_USERS" << endl;

                    cout << "Хотите выбрать другой раздел?" << endl;
                    cout << "1. ДА." << endl;
                    cout << "2. НЕТ." << endl
                         << endl;
                    ask = 0;
                    ask = valueTypeI();

                    if (ask != 1 && ask != 2)
                    {
                        cout << endl
                             << "=> Некорректный ввод (Ответ = 2 -нет).";
                        cout << endl
                             << "=> Пожалуйста правильно ответьте на вопрос.";
                    }

                } while (ask != 1 && ask != 2);

                cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                if (ask == 2)
                    break;
                if (ask == 1)
                {
                    cout << "   Выберите корневой раздел:" << endl;
                    cout << "1) HKEY_CLASSES_ROOT." << endl;
                    cout << "2) HKEY_CURRENT_USER." << endl;
                    cout << "3) HKEY_LOCAL_MACHINE." << endl;
                    cout << "4) HKEY_USERS." << endl;
                    cout << "5) ОТМЕНА выбора.";
                    do
                    {
                        cout << endl
                             << endl;
                        select = 0;
                        select = valueTypeI();

                        if (select != 1 && select != 2 && select != 3 && select != 4 && select != 5)
                        {
                            cout << endl
                                 << "=> Некорректный ввод (Ответ = 5 -отмена).";
                            cout << endl
                                 << "=> Пожалуйста правильно выберите раздел.";
                        }

                    } while (select != 1 && select != 2 && select != 3 && select != 4 && select != 5);

                    cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                    if (select == 5)
                        break;
                    if (select == 1)
                    {
                        if (phkResult != NULL)
                            RegCloseKey(phkResult);

                        hRoot = HKEY_CLASSES_ROOT;
                        cout << "Выбран корневой раздел реестра HKEY_CLASSES_ROOT" << endl;
                        key_classes_r = true;
                        key_current_u = false;
                        key_local_m = false;
                        key_u = false;

                        key_on = false;
                    }
                    if (select == 2)
                    {
                        if (phkResult != NULL)
                            RegCloseKey(phkResult);

                        hRoot = HKEY_CURRENT_USER;
                        cout << "Выбран корневой раздел реестра HKEY_CURRENT_USER" << endl;
                        key_classes_r = false;
                        key_current_u = true;
                        key_local_m = false;
                        key_u = false;

                        key_on = false;
                    }
                    if (select == 3)
                    {
                        if (phkResult != NULL)
                            RegCloseKey(phkResult);

                        hRoot = HKEY_LOCAL_MACHINE;
                        cout << "Выбран корневой раздел реестра HKEY_LOCAL_MACHINE" << endl;
                        key_classes_r = false;
                        key_current_u = false;
                        key_local_m = true;
                        key_u = false;

                        key_on = false;
                    }
                    if (select == 4)
                    {
                        if (phkResult != NULL)
                            RegCloseKey(phkResult);

                        hRoot = HKEY_USERS;
                        cout << "Выбран корневой раздел реестра HKEY_USERS" << endl;
                        key_classes_r = false;
                        key_current_u = false;
                        key_local_m = false;
                        key_u = true;

                        key_on = false;
                    }
                }
            }

            cout << "-------------------------------------------------" << endl;
            cout << "Вернутся в главное меню нажмите-<ENTER>";
            cin.get();
        }
        break;

        case 2:
        {
            cout << "=== Выбран 2 пункт меню ===";
            if (root_on)
            {
                do
                {
                    ask = 0;
                    cout << endl
                         << endl;
                    if (key_classes_r)
                        cout << "Выбран корневой раздел реестра HKEY_CLASSES_ROOT" << endl;
                    if (key_current_u)
                        cout << "Выбран корневой раздел реестра HKEY_CURRENT_USER" << endl;
                    if (key_local_m)
                        cout << "Выбран коневой раздел реестра HKEY_LOCAL_MACHINE" << endl;
                    if (key_u)
                        cout << "Выбран корневой раздел реестра HKEY_USERS" << endl;

                    cout << "Хотите СОЗДАТЬ ключ в выбраном разделе реестра?" << endl;
                    cout << "1. ДА." << endl;
                    cout << "2. НЕТ." << endl
                         << endl;
                    ask = valueTypeI();

                    if (ask != 1 && ask != 2)
                    {
                        cout << endl
                             << "=> Некорректный ввод (Ответ = 2 -нет).";
                        cout << endl
                             << "=> Пожалуйста правильно ответьте на вопрос.";
                    }

                } while (ask != 1 && ask != 2);

                cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                if (ask == 2)
                    break;
                if (ask == 1)
                {
                    if (key_on == false)
                    {
                        if (CreateKey(hRoot) != -1)
                        {
                            key_on = true;
                        }
                    }
                    else
                    {
                        cout << "Ключ уже создан: ";
                        if (key_classes_r)
                            cout << "в корневом разделе реестра HKEY_CLASSES_ROOT" << endl;
                        if (key_current_u)
                            cout << "в корневом разделе реестра HKEY_CURRENT_USER" << endl;
                        if (key_local_m)
                            cout << "в корневом разделе реестра HKEY_LOCAL_MACHINE" << endl;
                        if (key_u)
                            cout << "в корневом разделе реестра HKEY_USERS" << endl;

                        cout << "Для создания другого ключа выберите другой корневой каталог реестра." << endl;
                        cout << "Смена корневого каталога реестра - 1 пункт меню." << endl;
                    }
                }
            }
            else
            {
                cout << endl
                     << endl;
                cout << "Корневой каталог реестра не выбран." << endl;
                cout << "Выбор корневого каталога реестра - 1 пункт меню." << endl;
            }

            cout << "-------------------------------------------------" << endl;
            cout << "Вернутся в главное меню нажмите-<ENTER>";
            cin.get();
        }
        break;

        case 3:
        {
            cout << "=== Выбран 3 пункт меню ===";
            if (root_on)
            {
                if (key_on)
                {
                    do
                    {
                        ask = 0;
                        cout << endl
                             << endl;
                        if (key_classes_r)
                            cout << "В корневом разделе реестра HKEY_CLASSES_ROOT есть ключ." << endl;
                        if (key_current_u)
                            cout << "В корневом разделе реестра реестра HKEY_CURRENT_USER есть ключ." << endl;
                        if (key_local_m)
                            cout << "В корневом разделе реестра реестра HKEY_LOCAL_MACHINE есть ключ." << endl;
                        if (key_u)
                            cout << "В корневом разделе реестра раздел реестра HKEY_USERS есть ключ." << endl;

                        cout << "Хотите УДАЛИТЬ ключ в выбраном разделе реестра?" << endl;
                        cout << "1. ДА." << endl;
                        cout << "2. НЕТ." << endl
                             << endl;
                        ask = valueTypeI();

                        if (ask != 1 && ask != 2)
                        {
                            cout << endl
                                 << "=> Некорректный ввод (Ответ = 2 -нет).";
                            cout << endl
                                 << "=> Пожалуйста правильно ответьте на вопрос.";
                        }

                    } while (ask != 1 && ask != 2);

                    cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                    if (ask == 2)
                        break;
                    if (ask == 1)
                    {
                        LONG lResult = RegDeleteKeyExA(hRoot, "SOFTWARE\\TestDir", KEY_WOW64_32KEY, 0);
                        if (lResult == ERROR_SUCCESS)
                        {
                            cout << "Ключ успешно удалён." << endl;
                            key_on = false;
                        }
                        else
                            cout << "Ошибка удаления ключа." << endl;
                    }
                }
                else
                {
                    cout << endl
                         << endl;
                    if (key_classes_r)
                        cout << "В корневом разделе реестра HKEY_CLASSES_ROOT" << endl;
                    if (key_current_u)
                        cout << "В корневом разделе реестра HKEY_CURRENT_USER" << endl;
                    if (key_local_m)
                        cout << "В корневом разделе реестра HKEY_LOCAL_MACHINE" << endl;
                    if (key_u)
                        cout << "В корневом разделе реестра HKEY_USERS" << endl;

                    cout << "Ключ не создан. Удаление невозможно." << endl;
                }
            }
            else
            {
                cout << endl
                     << endl;
                cout << "Корневой каталог реестра не выбран." << endl;
                cout << "Выбор корневого каталога реестра - 1 пункт меню." << endl;
            }

            cout << "-------------------------------------------------" << endl;
            cout << "Вернутся в главное меню нажмите-<ENTER>";
            cin.get();
        }
        break;

        case 4:
        {
            cout << "=== Выбран 4 пункт меню ===";
            if (root_on)
            {
                if (key_on)
                {
                    do
                    {
                        ask = 0;
                        cout << endl
                             << endl;
                        if (key_classes_r)
                            cout << "Выбран раздел реестра HKEY_CLASSES_ROOT в котором есть ключ." << endl;
                        if (key_current_u)
                            cout << "Выбран раздел реестра HKEY_CURRENT_USER в котором есть ключ." << endl;
                        if (key_local_m)
                            cout << "Выбран раздел реестра HKEY_LOCAL_MACHINE в котором есть ключ." << endl;
                        if (key_u)
                            cout << "Выбран раздел реестра HKEY_USERS в котором есть ключ." << endl;

                        cout << "Хотите СОЗДАТЬ ПАРАМЕТР в ключе в выбраном разделе реестра?" << endl;
                        cout << "1. ДА." << endl;
                        cout << "2. НЕТ." << endl
                             << endl;
                        ask = valueTypeI();

                        if (ask != 1 && ask != 2)
                        {
                            cout << endl
                                 << "=> Некорректный ввод (Ответ = 2 -нет).";
                            cout << endl
                                 << "=> Пожалуйста правильно ответьте на вопрос.";
                        }

                    } while (ask != 1 && ask != 2);

                    cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                    if (ask == 2)
                        break;
                    if (ask == 1)
                    {
                        DWORD value; // задаем значения ключа
                        do
                        {
                            value = 0;
                            cout << endl
                                 << "Введите новое значение реестра:" << endl;
                            value = valueTypeI();

                        } while (value == 0);

                        cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                        LONG lResult = RegSetValueExA(phkResult, "TestRuns", 0, REG_DWORD, (BYTE *)&value, sizeof(DWORD));
                        if (lResult == ERROR_SUCCESS)
                        {
                            cout << "Параметр успешно создан." << endl;
                            parameter_on = true;
                        }
                        else
                        {
                            cout << "Ошибка создания параметра." << endl;
                            parameter_on = false;
                        }
                    }
                }
                else
                {
                    cout << endl
                         << endl;
                    if (key_classes_r)
                        cout << "В корневом разделе реестра HKEY_CLASSES_ROOT" << endl;
                    if (key_current_u)
                        cout << "В корневом разделе реестра HKEY_CURRENT_USER" << endl;
                    if (key_local_m)
                        cout << "В корневом разделе реестра HKEY_LOCAL_MACHINE" << endl;
                    if (key_u)
                        cout << "В корневом разделе реестра HKEY_USERS" << endl;

                    cout << "Ключь не определен !!!" << endl;
                    cout << "Создать параметр ключа невозможно." << endl;
                    cout << "Создать или открыть ключ - 2 пункт меню." << endl;
                }
            }
            else
            {
                cout << endl
                     << endl;
                cout << "Корневой каталог реестра не выбран." << endl;
                cout << "Выбор корневого каталога реестра - 1 пункт меню." << endl;
            }

            cout << "-------------------------------------------------" << endl;
            cout << "Вернутся в главное меню нажмите-<ENTER>";
            cin.get();
        }
        break;

        case 5:
        {
            cout << "=== Выбран 5 пункт меню ===";
            if (root_on)
            {
                if (key_on)
                {
                    if (parameter_on)
                    {
                        do
                        {
                            ask = 0;
                            cout << endl
                                 << endl;
                            if (key_classes_r)
                                cout << "Выбран раздел реестра HKEY_CLASSES_ROOT в котором есть ключ с параметром." << endl;
                            if (key_current_u)
                                cout << "Выбран раздел реестра HKEY_CURRENT_USER в котором есть ключ с параметром." << endl;
                            if (key_local_m)
                                cout << "Выбран раздел реестра HKEY_LOCAL_MACHINE в котором есть ключ с параметром." << endl;
                            if (key_u)
                                cout << "Выбран раздел реестра HKEY_USERS в котором есть ключ с параметром." << endl;

                            cout << "Хотите УДАЛИТЬ ПАРАМЕТР в ключе в выбраном разделе реестра?" << endl;
                            cout << "1. ДА." << endl;
                            cout << "2. НЕТ." << endl
                                 << endl;
                            ask = valueTypeI();

                            if (ask != 1 && ask != 2)
                            {
                                cout << endl
                                     << "=> Некорректный ввод (Ответ = 2 -нет).";
                                cout << endl
                                     << "=> Пожалуйста правильно ответьте на вопрос.";
                            }

                        } while (ask != 1 && ask != 2);

                        cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

                        if (ask == 2)
                            break;
                        if (ask == 1)
                        {
                            //Удаляем значение реестра
                            LONG lResult = RegDeleteValueA(phkResult, "TestRuns");
                            if (lResult == ERROR_SUCCESS)
                            {
                                cout << "Параметр успешно удален." << endl;
                                parameter_on = false;
                            }
                            else
                            {
                                cout << "Ошибка удаления параметра." << endl;
                                parameter_on = true;
                            }
                        }
                    }
                    else
                    {
                        cout << endl
                             << endl;
                        cout << "В созданном или открытом ключе отсутствует параметр." << endl;
                        cout << "Создать параметр ключа - 4 пункт меню." << endl;
                    }
                }
                else
                {
                    cout << endl
                         << endl;
                    if (key_classes_r)
                        cout << "В корневом разделе реестра HKEY_CLASSES_ROOT" << endl;
                    if (key_current_u)
                        cout << "В корневом разделе реестра HKEY_CURRENT_USER" << endl;
                    if (key_local_m)
                        cout << "В корневом разделе реестра HKEY_LOCAL_MACHINE" << endl;
                    if (key_u)
                        cout << "В корневом разделе реестра HKEY_USERS" << endl;

                    cout << "Ключ не создан. Удалить параметр ключа невозможно." << endl;
                }
            }
            else
            {
                cout << endl
                     << endl;
                cout << "Корневой каталог реестра не выбран." << endl;
                cout << "Выбор корневого каталога реестра - 1 пункт меню." << endl;
            }

            cout << "-------------------------------------------------" << endl;
            cout << "Вернутся в главное меню нажмите-<ENTER>";
            cin.get();
        }
        break;

        case 6:
        {
            cout << "=== Выбран 6 пункт меню ===";
            do
            {
                cout << endl
                     << endl;
                cout << "Хотите УДАЛИТЬ ВСЕ СОЗДАННЫЕ ТЕСТОВЫЕ КЛЮЧИ ИЗ РЕЕСТРА? (при их наличии)" << endl;
                cout << "1. ДА." << endl;
                cout << "2. НЕТ." << endl
                     << endl;
                ask = 0;
                ask = valueTypeI();

                if (ask != 1 && ask != 2)
                {
                    cout << endl
                         << "=> Некорректный ввод (Ответ = 2 -нет).";
                    cout << endl
                         << "=> Пожалуйста правильно ответьте на вопрос.";
                }

            } while (ask != 1 && ask != 2);

            cout << ".    .    .    .    .    .    .    .    .    .    " << endl;

            if (ask == 2)
                break;
            if (ask == 1)
            {
                LONG lResult = NULL;

                lResult = RegDeleteKeyExA(HKEY_CLASSES_ROOT, "SOFTWARE\\TestDir", KEY_WOW64_32KEY, 0);
                if (lResult == ERROR_SUCCESS)
                    cout << "Тестовый ключ успешно удалён c HKEY_CLASSES_ROOT." << endl;
                else
                    cout << "Тестовый ключ в HKEY_CLASSES_ROOT не создан." << endl;

                cout << "_________________________________________________" << endl;
                lResult = NULL;
                lResult = RegDeleteKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\TestDir", KEY_WOW64_32KEY, 0);
                if (lResult == ERROR_SUCCESS)
                    cout << "Тестовый ключ успешно удалён c HKEY_CURRENT_USER." << endl;
                else
                    cout << "Тестовый ключ в HKEY_CURRENT_USER не создан." << endl;

                cout << "_________________________________________________" << endl;
                lResult = NULL;
                lResult = RegDeleteKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\TestDir", KEY_WOW64_32KEY, 0);
                if (lResult == ERROR_SUCCESS)
                    cout << "Тестовый ключ успешно удалён c HKEY_LOCAL_MACHINE." << endl;
                else
                    cout << "Тестовый ключ в HKEY_LOCAL_MACHINE не создан." << endl;

                cout << "_________________________________________________" << endl;
                lResult = NULL;
                lResult = RegDeleteKeyExA(HKEY_USERS, "SOFTWARE\\TestDir", KEY_WOW64_32KEY, 0);
                if (lResult == ERROR_SUCCESS)
                    cout << "Тестовый ключ успешно удалён c HKEY_USERS." << endl;
                else
                    cout << "Тестовый ключ в HKEY_USERS не создан." << endl;
            }

            key_on = false;

            cout << "-------------------------------------------------" << endl;
            cout << "Вернутся в главное меню нажмите-<ENTER>";
            cin.get();
        }
        break;

        case 7:
        {
            if (root_on)
            {
                //Если был открыт ключ реестра закрываем его
                if (phkResult != NULL)
                    RegCloseKey(phkResult);
            }
            goback = false;
        }
        break;

        default:
            cout << ">>> Неверный пункт меню." << endl;
            cout << ">>> Будьте внимательны!!!" << endl;
            cout << ">>> Для продолжения-<ENTER>";
            cin.get();
            break;
        }

    } while (goback);

    cout << endl;
    cout << "   .........................." << endl;
    cout << "   |       До свидания      |" << endl;
    cout << "   ''''''''''''''''''''''''''" << endl
         << endl;

    cout << "_ПРОГРАММА ЗАВЕРШЕНА" << endl;
    cout << "_____Hажмите-<ENTER>";
    cin.get();
    cout << endl;

    SetConsoleCP(i1);
    SetConsoleOutputCP(i1);

    return 0;
}

