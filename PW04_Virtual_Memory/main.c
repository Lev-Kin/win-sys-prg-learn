// Ввести одномерный целочисленный массив размерностью N в память ПЭВМ.
// Память под массив выделить используя динамическое создание кучи при
// помощи системного вызова HeapCreate()и распределения памяти из кучи
// при помощи функция HеарАllос().
// Вывести массив на экран по пять элементов в строке. Записать массив в файл.
// Вывести массив из файла на экран по пять элементов в строке.
// Определить все локальные минимумы и максимумы в заданном массиве.
// Элемент называется локальным минимумом (максимумом), если у него нет соседа,
// меньшего (большего), чем он сам.
// Результат вывести на экран.
// Работу программы построить в виде меню.

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h> // INT_MAX

int main()
{
	int i1 = GetConsoleCP();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	FILE *in = NULL;
	FILE *out = NULL;

	HANDLE hHeap = NULL;

	int *array = NULL;
	int array_size = 0;
	int heap_size = 0;
	int element = 0;

	int menu;
	char any_key;
	int install = 0;
	int goback = 1;
	do
	{
		system("cls");
		printf("    +++ Практическая работа №4 +++\n\n");
		printf("   ________| ГЛАВНОЕ МЕНЮ |________\n");
		printf("   Выберите ПУНКТ меню для работы:\n");
		printf("1) Инициализация массива.\n");
		printf("2) Вывести массив на экран.\n");
		printf("3) Cохранить массив в  файл.\n");
		printf("4) Загрузить массив из файла.\n");
		printf("5) Обработка массива согласно условию.\n");
		printf("6) ----------|  ВыхоД  |-----------\n\n");
		printf(">");
		menu = 0;
		scanf_s("%d", &menu);
		while (getchar() != '\n')
			continue;

		switch (menu)
		{
		case 1:
		{
			printf("\n=== Выбран 1 пункт меню ===\n\n");
			if (install != 0)
				printf("Массив инсталлирован.\n");
			else
			{
				do
				{
					while (1)
					{
						char ch = 0;
						printf("\nВведите размер массива (5 - 20):\n --> ");

						if (!scanf_s("%d", &array_size) || getchar() != '\n')
						{
							printf("\nНекорректный ввод (только целые числа)\n");
							printf("Пожалуйста повторите ввод\n");
							while ((ch = getchar()) != '\n' && ch != EOF)
								;
						}
						else
							break;
					}

				} while (array_size < 5 || array_size > 20);
				heap_size = array_size * sizeof(int); // размер кучи

				printf("\n");

				// Динамическое создания кучи
				hHeap = HeapCreate(
					HEAP_NO_SERIALIZE, // атрибуты распределения кучи
					heap_size,		   // начальный размер кучи
					heap_size		   // максимальный размер кучи
				);

				if (hHeap != NULL)
					printf("Куча успешно выделилась.\n");
				else
				{
					printf("Куча не выделилась.\n");
					printf("Программа будет закрыта.\n");
					printf("________Hажмите-<ENTER>");
					any_key = getch();
					exit(0);
				}

				// Распределения памяти из кучи
				array = (int *)HeapAlloc(
					hHeap,					 // дескриптор кучи
					HEAP_ZERO_MEMORY,		 // управляющие флаги
					array_size * sizeof(int) // размер распределяемой памяти
				);

				if (array != NULL)
				{
					printf("Массив A[%d] создан.\n\n", array_size);
					printf("Заполните созданный массив числами:\n");
					for (int i = 0; i < array_size; i++)
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

					printf("Массив успешно сформирован.\n");
					install = 1;
				}
				else
				{
					printf("Память не выделена.\n");
					printf("Программа будет закрыта.\n");
					printf("________Hажмите-<ENTER>");
					any_key = getch();
					exit(0);
				}
			}
			printf("---------------------------------------\n");
			printf("Вернутся в главное меню нажмите-<ENTER>");
			any_key = getch();
		}
		break;

		case 2:
		{
			printf("\n=== Выбран 2 пункт меню ===\n\n");
			if (install != 0)
			{
				printf("Массив в куче:\n");
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
			}
			else
			{
				printf("Инсталлируйте массив пункт меню 1\n");
				printf("Или загрузите массив пункт меню 4\n");
			}

			printf("---------------------------------------\n");
			printf("Вернутся в главное меню нажмите-<ENTER>");
			any_key = getch();
		}
		break;

		case 3:
		{
			printf("\n=== Выбран 3 пункт меню ===\n\n");
			if (install != 0)
			{
				int ask;
				printf("При имеющемся файле (Array.txt), файл будет перезаписан.\n");
				printf(">>> COХРАНИТЬ <<< ?\n");
				printf("       ДА  - нажмите 1.\n");
				printf("       НЕТ - нажмите 2.");
				do
				{
					while (1)
					{
						char ch = 0;

						printf("\n------->");

						if (!scanf_s("%d", &ask) || getchar() != '\n')
						{
							printf("\nНекорректный ввод (Ответ = 2 -нет).");
							printf("\nПожалуйста правильно ответьте на вопрос.");
							while ((ch = getchar()) != '\n' && ch != EOF)
								;
						}
						else
							break;
					}

				} while (ask <= 0 || ask > 2);

				printf("\n");

				if (ask == 1)
				{
					if (out = fopen("Array.txt", "w"))
					{
						for (int i = 0; i < array_size; i++)
						{
							fprintf(out, "%d ", array[i]);
						}
						printf("Данные массива успешно записаны в файл.\n");
						fclose(out);
					}
					else
						printf("Не удается открыть файл для записи.\n");
				}
				if (ask == 2)
				{
					break;
				}
			}
			else
			{
				printf("Инсталлируйте массив пункт меню 1\n");
				printf("Или загрузите массив пункт меню 4\n");
			}

			printf("---------------------------------------\n");
			printf("Вернутся в главное меню нажмите-<ENTER>");
			any_key = getch();
		}
		break;

		case 4:
		{
			printf("\n=== Выбран 4 пункт меню ===\n\n");
			if (install != 0)
			{
				int ask;
				printf("Имеющийся данные будут потеряны. (если не были сохранены)\n");
				printf(">>> ЗАГРУЗИТЬ <<< ? (будут загружены данные из файла)\n");
				printf("       ДА  - нажмите 1.\n");
				printf("       НЕТ - нажмите 2.");
				do
				{
					while (1)
					{
						char ch = 0;

						printf("\n------->");

						if (!scanf_s("%d", &ask) || getchar() != '\n')
						{
							printf("\nНекорректный ввод (Ответ = 2 -нет).");
							printf("\nПожалуйста правильно ответьте на вопрос.");
							while ((ch = getchar()) != '\n' && ch != EOF)
								;
						}
						else
							break;
					}

				} while (ask <= 0 || ask > 2);

				printf("\n");

				if (ask == 1)
				{
					array_size = 0;
					heap_size = 0;

					if (HeapFree(hHeap, NULL, array))
						printf("Выделенная память в куче освобождена.\n");
					else
					{
						printf("Ошибка освобождения памяти.\n");
						printf("Программа будет закрыта.\n");
						printf("________Hажмите-<ENTER>");
						any_key = getch();
						exit(0);
					}

					if (HeapDestroy(hHeap))
						printf("Динамическая выделенная память кучи удалена.\n");
					else
					{
						printf("Ошибка удаления памяти.\n");
						printf("Программа будет закрыта.\n");
						printf("________Hажмите-<ENTER>");
						any_key = getch();
						exit(0);
					}

					if (in = fopen("Array.txt", "r"))
					{
						while ((fscanf(in, "%d ", &element)) != EOF)
						{
							if (in)
								array_size++;
							else
								break;
						}
						heap_size = array_size * sizeof(int); // размер кучи
						printf("Файл считан успешно.\n");
						fseek(in, 0L, SEEK_SET);

						// Динамическое создания кучи
						hHeap = HeapCreate(HEAP_NO_SERIALIZE, heap_size, heap_size);
						if (hHeap != NULL)
							printf("Куча успешно выделилась.\n");
						else
						{
							printf("Куча не выделилась.\n");
							install = 0;
						}

						// Распределения памяти из кучи
						array = (int *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, array_size * sizeof(int));
						if (array != NULL)
						{
							for (int i = 0; i < array_size; i++)
								fscanf_s(in, "%d ", &array[i]);

							printf("Массив A[%d] создан.\n", array_size);
							printf("Массив успешно сформирован.\n");
							install = 1;
						}
						else
						{
							printf("Память не выделена.\n");
							install = 0;
						}

						fclose(in);
					}
					else
					{
						printf("Не удается открыть файл для чтения.\n");
						install = 0;
					}
				}
				if (ask == 2)
				{
					break;
				}
			}
			else // first load
			{
				int ask;
				printf(">>> ЗАГРУЗИТЬ <<< ? (будут загружены данные из файла)\n");
				printf("       ДА  - нажмите 1.\n");
				printf("       НЕТ - нажмите 2.");
				do
				{
					while (1)
					{
						char ch = 0;

						printf("\n------->");

						if (!scanf_s("%d", &ask) || getchar() != '\n')
						{
							printf("\nНекорректный ввод (Ответ = 2 -нет).");
							printf("\nПожалуйста правильно ответьте на вопрос.");
							while ((ch = getchar()) != '\n' && ch != EOF)
								;
						}
						else
							break;
					}

				} while (ask <= 0 || ask > 2);

				printf("\n");

				if (ask == 1)
				{
					if (in = fopen("Array.txt", "r"))
					{
						while ((fscanf(in, "%d ", &element)) != EOF)
						{
							if (in)
								array_size++;
							else
								break;
						}
						heap_size = array_size * sizeof(int); // размер кучи
						printf("Файл считан успешно.\n");
						fseek(in, 0L, SEEK_SET);

						// Динамическое создания кучи
						hHeap = HeapCreate(HEAP_NO_SERIALIZE, heap_size, heap_size);
						if (hHeap != NULL)
							printf("Куча успешно выделилась.\n");
						else
						{
							printf("Куча не выделилась.\n");
							install = 0;
						}

						// Распределения памяти из кучи
						array = (int *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, array_size * sizeof(int));
						if (array != NULL)
						{
							for (int i = 0; i < array_size; i++)
								fscanf_s(in, "%d ", &array[i]);

							printf("Массив A[%d] создан.\n", array_size);
							printf("Массив успешно сформирован.\n");
							install = 1;
						}
						else
						{
							printf("\nПамять не выделена.\n");
							install = 0;
						}

						fclose(in);
					}
					else
					{
						printf("\nНе удается открыть файл для чтения.\n");
						install = 0;
					}
				}
				if (ask == 2)
				{
					break;
				}
			}

			printf("---------------------------------------\n");
			printf("Вернутся в главное меню нажмите-<ENTER>");
			any_key = getch();
		}
		break;

		case 5:
		{
			printf("\n=== Выбран 5 пункт меню ===\n\n");
			if (install != 0)
			{
				printf("Определить все локальные минимумы и максимумы в заданном массиве.\n");
				printf("Элемент называется локальным минимумом (максимумом),\n");
				printf("если у него нет соседа, меньшего (большего), чем он сам.\n\n");

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

				int max = INT_MIN;
				int min = INT_MAX;

				printf("\nВсе локальные минимумы  в массиве:\n");
				for (int i = 1; i < array_size - 1; i++)
					if (array[i] < array[i + 1] && array[i] < array[i - 1])
					{
						printf(" %d |", array[i]);

						if (array[i] < min)
							min = array[i];
					}

				printf("\nВсе локальные максимумы в массиве:\n");
				for (int i = 1; i < array_size - 1; i++)
					if (array[i] > array[i + 1] && array[i] > array[i - 1])
					{
						printf(" %d |", array[i]);

						if (array[i] > max)
							max = array[i];
					}

				printf("\n\n");
				printf("Минимальный  локальный минимум  в массиве = %d\n", min);
				printf("Максимальный локальный максимум в массиве = %d\n", max);
			}
			else
			{
				printf("Инсталлируйте массив пункт меню 1\n");
				printf("Или загрузите массив пункт меню 4\n");
			}

			printf("---------------------------------------\n");
			printf("Вернутся в главное меню нажмите-<ENTER>");
			any_key = getch();
		}
		break;

		case 6:
		{
			if (install != 0)
			{
				if (HeapFree(hHeap, NULL, array) != FALSE) // HEAP_NO_SERIALIZE
					printf("\nВыделенная память в куче освобождена.");
				else
					printf("\nОшибка освобождения памяти в куче.");

				if (HeapDestroy(hHeap) != FALSE)
					printf("\nДинамическая выделенная память кучи удалена.");
				else
					printf("\nОшибка удаления динамической памяти кучи.");
			}
			goback = 0;
		}
		break;

		default:
			printf("\n\n");
			printf(">>>Неверный пункт меню.\n");
			printf(">>>Будьте внимательны!!!\n");
			printf(">>>Для продолжения-<ENTER>");
			any_key = getch();
			break;
		}

	} while (goback != 0);

	printf("\n\n");
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
