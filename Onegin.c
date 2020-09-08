/*
Copyright (c) 2020  MIPT

Name:
	Онегин
Abstract:
	Сортирует строки входного файла и выводит в другой файл
Author:
	JulesIMF	
Last Edit:
	08.09.2020 19:59
Edit Notes:
	1) Убраны ненужные отладочные модули и устаревшие строки кода
	2) Добавлены reverseStringComparator, write	
*/

//#define JULESIMF_DEBUG
//#define JULESIMF_NO_OUTPUT

///Из-за специфики VS необходимо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <iostream>


/**
 * Прямой компаратор для строк.
 *
 * \param pFirst Указатель на первый элемент
 * \param pSecond Указатель на второй элемент
 * \return 0 в случае равенства строк, иначе разницу между первыми отличающимися символами
 */
int stringComparator(void const* pFirst, void const* pSecond);


/**
 * Прямой компаратор для строк.
 *
 * \param pFirst Указатель на первый элемент
 * \param pSecond Указатель на второй элемент
 * \return 0 в случае равенства строк, иначе разницу между первыми отличающимися символами
 */
int reverseStringComparator(void const* pFirst, void const* pSecond);


/**
 * \brief Загружает файл в RAM и вставляет '\0' после него, при этом после '\0' есть еще один доступный байт.
 * 
 * \param file загружаемый файл
 * \param fileSize указатель на переменную, в которую будет записан размер файла
 * \param nStrings указатель на переменную, в которую будет записано количество строк
 * \return указатель на область памяти, в которую загружен файл, 0 в случае ошибки
 */
void* translateFileIntoRam(FILE* file, int* pFileSize, int* nStrings);


/**
 * \brief Разбивает source на строки в destination.
 * \param source указатель на транслированный файл
 * \param fileSize размер транслированного файла
 * \param destination указатель на массив со строками
 * \param nStrings количество строк
 * \param LENGTH_string максимальная длина строки
 * \warning source не перекопируется!!
 * \warning массив *stringsLengths перевыделяется!!
 * \return 0 в случае успешного выполнения, 1 если переданы противоречивые аргументы, 2 если есть строка длиннее 100
 */
int separateStrings(void* source, int fileSize, char** destination, 
                    int nStrings, int const LENGTH_string);

/**
 * \brief Выводит массив строк в файл.
 * \param fileName строка с именем файла
 * \param strings массив строк
 * \param nStrings кличество строк в массиве
 * \param mode режим открытия файла
 */
int write(char const* fileName, char** strings, int nStrings, char const* mode);



//*************************************************************************************************	
//*************************************************************************************************	
//******************************************MAIN
//*************************************************************************************************
//*************************************************************************************************	


/**
 * \brief Точка входа в программу.
 * \param argc количество переданных аргументов
 * \param argv массив параметров (строк)
 *
 * \return 0 в случае корректного выполнения программы.
			 Для других кодов смотрите документацию.
 */
int main(int argc, char const** argv)
{
#ifdef    JULESIMF_DEBUG
	//Для извращенского дебага все можно
	argc = 3;
	argv[1] = "Tests.txt";
	argv[2] = "out.txt";

#endif //!JULESIMF_DEBUG
	if (argc && !argv)
	{
		printf("Error occured while passing parameters!\n");
		return 0;
	}

	if (argc != 3)
	{
		printf("Invalid parameters!\n");
		return 0;
	}
	const int LENGTH_string = 100;

//*************************************************************************************************	
//******************************************ЧТЕНИЕ ДАННЫХ
//*************************************************************************************************

	//Файл с данными
	FILE* poem = fopen(argv[1], "r");
	if (!poem)
	{
		printf("Failed to open file \"%s\"\n", argv[1]);
		return 0;
	}
	else
	{
#ifndef   JULESIMF_NO_OUTPUT
		printf("Input file opened\n");
#endif //!JULESIMF_NO_OUTPUT
	}

	//Получим размер файла и транслируем его в память
	int fileSize = 0; //Размер файла запишется сюда
	int nStrings = 0; //Количество строк запишется сюда

//*************************************************************************************************	
//******************************************ТРАНСЛЯЦИЯ ДАННЫХ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Translating file into RAM...\n");
#endif //!JULESIMF_NO_OUTPUT

	void* translatedFile = translateFileIntoRam(poem, &fileSize, &nStrings);

#ifndef   JULESIMF_NO_OUTPUT
	printf("Input file closed\n");
#endif //!JULESIMF_NO_OUTPUT

	fclose(poem);

	if (!translatedFile)
	{
		printf("Error occured while translating file into RAM\n");
		return 0;
	}

	char** strings = (char**)calloc(nStrings, sizeof(char*)); //А здесь сам массив
	if (!strings)
	{
		printf("Error occured while calling calloc\n");
		return 0;
	}

//*************************************************************************************************	
//******************************************РАЗДЕЛЕНИЕ НА СТРОКИ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Separating strings...\n");
#endif //!JULESIMF_NO_OUTPUT
	

	int RETURN_readStrings = separateStrings(translatedFile, fileSize, strings, 
	                                         nStrings, LENGTH_string);


	if (RETURN_readStrings == 1)
	{
		printf("Error occured while separating file into strings\n");
		return 0;
	}
	
//*************************************************************************************************	
//******************************************СОРТИРОВКА СТРОК В ПРЯМОМ ПОРЯДКЕ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem in direct...\n");
#endif //!JULESIMF_NO_OUTPUT

	qsort((void*)strings, nStrings, sizeof(char*), stringComparator);

	if (write(argv[2], strings, nStrings, "w"))
	{
		printf("Failed to open file \"%s\"", argv[2]);
		return 0;
	}
		
//*************************************************************************************************	
//******************************************СОРТИРОВКА СТРОК В ОБРАТНОМ ПОРЯДКЕ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem in direct...\n");
#endif //!JULESIMF_NO_OUTPUT

#ifdef    JULESIMF_DEBUG
	FILE* file = fopen(argv[2], "a");
	fprintf(file, "\n\n\n");
	fclose(file);
#endif //!JULESIMF_DEBUG

	
	qsort((void*)strings, nStrings, sizeof(char*), reverseStringComparator);

	if(write(argv[2], strings, nStrings, "a"))
	{
		printf("Failed to open file \"%s\"", argv[2]);
		return 0;
	}
	
	
	printf("\n\nExecuted correctly\n");
	return 0;
}






//*************************************************************************************************	
//*************************************************************************************************	
//******************************************ОПРЕДЕЛЕНИЕ ФУНКЦИЙ
//*************************************************************************************************
//*************************************************************************************************	


int write(char const* fileName, char** strings, int nStrings, char const* mode)
{
#ifdef    JULESIMF_DEBUG
	assert(fileName);
	assert(strings);
	assert(mode); //Что??
#endif //!JULESIMF_DEBUG
	
#ifndef   JULESIMF_NO_OUTPUT
	printf("Output file opened\n");
#endif //!JULESIMF_NO_OUTPUT
	//Файл с данными
	FILE* file = fopen(fileName, mode);
	if (!file)
		return 1;

#ifndef   JULESIMF_NO_OUTPUT
	printf("Writing output...\n");
#endif //!JULESIMF_NO_OUTPUT

	//Поехали выводить
	for (int i = 0; i != nStrings; i++)
	{
#ifdef    JULESIMF_DEBUG
		assert(strings[i]);
#endif //!JULESIMF_DEBUG
		fprintf(file, "%s\n", strings[i]);
	}

#ifndef   JULESIMF_NO_OUTPUT
	printf("Output file closed\n");
#endif //!JULESIMF_NO_OUTPUT

	fclose(file);
	return 0;
}


int stringComparator(void const* pFirst, void const* pSecond)
{
#ifdef    JULESIMF_DEBUG
	assert(pFirst);
	assert(pSecond);
#endif //!JULESIMF_DEBUG


	//pFirst и pSecond указывают не на строки, а на указатель на строки

	//А это уже строки
	char* first = (*(char**)pFirst);
	char* second = (*(char**)pSecond);

	while ((*first) && (*second))
	{
		if ((*first) == (*second))
		{
			first++;
			second++;
#ifdef    JULESIMF_DEBUG
			assert(first);
			assert(second);
#endif //!JULESIMF_DEBUG
			continue;
		}
		return (int)(*first) - (int)(*second);
	}

	return (((*first) == (*second)) ? 0 : (int)(*first) - (int)(*second));
}

int reverseStringComparator(void const* pFirst, void const* pSecond)
{
#ifdef    JULESIMF_DEBUG
	assert(pFirst);
	assert(pSecond);
#endif //!JULESIMF_DEBUG

	int L_first = 0;
	int L_second = 0;


	//pFirst и pSecond указывают не на строки, а на указатель на строки

	//А это уже строки
	char* first = (*(char**)pFirst);
	char* second = (*(char**)pSecond);

	{
		char* tmpFirst  = first;
		char* tmpSecond = second;
		while (*(tmpFirst++))
			L_first++;

		while (*(tmpSecond)++)
			L_second++;
	}


	char cFirst = '\0', cSecond = '\0';
	
	int i = 0;
	for(; i < L_first && i < L_second; i++)
	{
		cFirst  = *(first  + L_first  - 1 - i);
		cSecond = *(second + L_second - 1 - i);
		if (cFirst == cSecond)
			continue;

		return cFirst - cSecond;
	}

	return L_first - L_second;
}


void* translateFileIntoRam(FILE* file, int* pFileSize, int* nStrings)
{
#ifdef    JULESIMF_DEBUG
	assert(file);
	assert(pFileSize);
	assert(nStrings);
#endif //!JULESIMF_DEBUG
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	void* translatedFile = malloc(fileSize + 2); //на всякий случай
	if (!translatedFile)
		return 0;

	int RETURN_fread = fread(translatedFile, 1, fileSize, file);
	((char*)translatedFile)[RETURN_fread] = '\0'; //По новой спецификации функции (чисто поржать)
	fseek(file, 0, SEEK_SET);


	*nStrings  = 1 + fileSize - RETURN_fread;
	*pFileSize = RETURN_fread;
	return translatedFile;
}

int separateStrings(void* source, int fileSize, char** destination, 
                    int nStrings, int const LENGTH_string)
{
#ifdef    JULESIMF_DEBUG
	assert(source);
	assert(destination);
#endif //!JULESIMF_DEBUG

	char* text          = (char*)source;
	int   iDestination  = 0; //Итератор для destination
	char* currentString = 0;

	for (int iText = 0; iText != fileSize + 1; iText++)
	{
		//Если iDestinаtion слишком большой, то JOPA
		if (iDestination >= nStrings)
			return 1;

		//Это если мы в прошлый раз запихнули строку
		if (!currentString)
			currentString = text + iText;
		
		if (text[iText] == '\n' || !text[iText])
		{
			text[iText] = '\0';
			destination      [iDestination] = currentString;
			iDestination++;
			currentString = 0;
		}
	}

	if (iDestination != nStrings)
		return 1;

	return 0;
}
