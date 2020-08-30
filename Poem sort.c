//Poem sort
//Иван Дорошенко, ИВТ
//#define JULESIMF_DEBUG

///Из-за специфики VS необходимо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <iostream>

/**
 * \brief Cчитает количество единиц в двоичном представлении n.
 * 
 * \param n Обрабатываемое число
 * \return Количество единиц в двоичном представлении n
 */
int intPopcount(int n) { int count = 0; while (n) { count += n & 1; n >>= 1; } return count; }
//В одну строчку реализация, потому что реально простая функция

/**
 * \breif Заменяет первый встетившийся '\n' на '\0', если таковой имеется.
 * 
 * \param string Обрабатываемая строка
 * \param size Размер строки
 */
void deleteNewLine(char* string, int const size);

/**
 * Добавляет новую строку в strings.
 * \warning Реальный размер массива должен быть степенью двойки, большей size.
 *          Реальный размер удваивается при превышении size этой степени.
 * \param strings Указатель на массив со строками
 * \param size Размер массивов
 * \param file Файл, из которого считывается строка
 * \param stringLength Максимальная длина строки
 * \return 0 в случае успешного добавления, -1 в случае отказа 
             realloc или malloc, -2 в случае отсутствия новой строки
 */
int addNewString(char*** strings, int size, FILE* file, int const stringLength);

/**
 * Компаратор для строк.
 * 
 * \param px Указатель на первый элемент
 * \param py Указатель на второй элемент
 * \return 0 в случае равенства строк, иначе разницу между первыми отличающимися символами
 */
int stringComparator(void const* px, void const* py);

/**
 * \brief Точка входа в программу.
 *
 * \return 0 в случае успешного выполнения программы. 
             Для других кодов смотрите документацию.
 */


int main()
{
	const int LENGTH_fileName = 258;
	const int LENGTH_string   = 100;
	//Имя входного файла
	char fileName[LENGTH_fileName];
	printf("Enter input file name: ");
	fgets(fileName, LENGTH_fileName - 2, stdin);
	
	//Не нашел, как считать строку без \n на конце, поэтому будем заниматься извращениями
	deleteNewLine(fileName, LENGTH_fileName);
	
	//Файл с данными
	FILE* poem = fopen(fileName, "r");
	if (!poem)
	{
		printf("Failed to open file \"%s\"\n", fileName);
		abort();
	}

	//А вдруг файл пустой?
	int size = 0; //Здесь хранится размер массива со строками
	char** strings = NULL; //А здесь сам массив

	int RETURN_addNewString; //Здесь хранится код возврата

	//Поехали считывать строки
	while ((RETURN_addNewString =
		addNewString(&strings, size, poem, LENGTH_string)) != -2)
	{
		if (RETURN_addNewString == -1)
		{
			printf("Error occured while calling malloc or realloc\n");
			abort();
		}

		size++;
	}

	fclose(poem);

#ifdef    JULESIMF_DEBUG
	printf("\n\n.....Testing the correctness of input\n");
	assert(strings != NULL);
	for (int i = 0; i != size; i++)
	{
		printf("%s\n", strings[i]);
	}
	printf(".....Test ended\n\n");
#endif //!JULESIMF_DEBUG

	qsort((void*)strings, size, sizeof(char*), stringComparator);

#ifdef    JULESIMF_DEBUG
	printf("\n\n.....Testing the correctness of sort\n");
	assert(strings != NULL);
	for (int i = 0; i != size; i++)
	{
		printf("%s\n", strings[i]);
	}
	printf("\n\n.....Test ended\n");
#endif //!JULESIMF_DEBUG


	printf("Enter output file name: ");
	fgets(fileName, LENGTH_fileName - 2, stdin);

	deleteNewLine(fileName, LENGTH_fileName);

	//Файл с данными
	FILE* sortedPoem = fopen(fileName, "w");
	if (!sortedPoem)
	{
		printf("Failed to open file \"%s\"\n", fileName);
		abort();
	}
	
	//Поехали выводить сортед поем
	for (int i = 0; i != size; i++)
	{
		fprintf(sortedPoem, "%s\n", strings[i]);
	}

	fclose(sortedPoem);
	return 0;	
}

void deleteNewLine(char* string, int const size)
{
	for (int i = 0; i != size; i++)
	{
		if (string[i] == '\n')
		{
			string[i] = '\0';
			break;
		}
	}
}

int addNewString(char*** strings, int size, FILE* file, int const stringLength)
{
	if (feof(file))
		return -2;

	//Итератор на место, где будет храниться новая строка
	int i = size;

	//Если size - степень двойки, надо обновить реальный размер
	if (intPopcount(size) <= 1)
	{
		if (size == 0)
			size = 1;
		else
			size <<= 1;
		char** newStrings = (char**)
		       realloc((void*)(*strings), sizeof(char**) * size);

		if (newStrings)
			*strings = newStrings;
		else
			return -1;
	}
	
	(*strings)[i] = (char*)malloc(sizeof(char) * stringLength);

	if (!(*strings)[i])
		return -1;

	fgets((*strings)[i], stringLength, file);
	deleteNewLine((*strings)[i], stringLength);
	return 0;
}

int stringComparator(void const* px, void const* py)
{
	//px и py указывают не на строки, а на указатель на строки

	//А это уже строки
	char* pcx = (*(char**)px);
	char* pcy = (*(char**)py);

	while ((*pcx) && (*pcy))
	{
		if ((*pcx) == (*pcy))
		{
			pcx++;
			pcy++;
			continue;
		}
		return (int)(*pcx) - (int)(*pcy);
	}

	return (((*pcx) == (*pcy)) ? 0 : (int)(*pcx) - (int)(*pcy));
}
