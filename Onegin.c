/*
Copyright (c) 2020  MIPT

Name:
	Онегин
Abstract:
	Сортирует строки входного файла и выводит в другой файл
Author:
	JulesIMF
Revision History:
	
Last Edit:
	07.09.2020 19:18
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
 * Компаратор для строк.
 *
 * \param pFirst Указатель на первый элемент
 * \param pSecond Указатель на второй элемент
 * \return 0 в случае равенства строк, иначе разницу между первыми отличающимися символами
 */
int stringComparator(void const* pFirst, void const* pSecond);


/**
 * \brief Загружает файл в RAM и вставляет '\0' после него, при этом после '\0' есть еще один доступный байт.
 * 
 * \param file загружаемый файл
 * \param fileSize указатель на переменную, в которую будет записан размер файла
 * \param nStrings указатель на переменную, в которую будет записано количество строк
 * \return указатель на область памяти, в которую загружен файл, NULL в случае ошибки
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
 * \return 0 в случае успешного выполнения, 1 если переданы противоречивые аргументы, 2 если есть строка длиннее 100
 */
int separateStrings(void* source, int fileSize, char** destination, 
		    int nStrings, int const LENGTH_string);


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
	printf(".....%d parametrs:\n", argc);
	for (int i = 0; i != argc; i++)
		printf("%s\n", argv[i]);

	printf(".....PARAMETRS ENDED\n\n");

	
	/*
	argc = 3;
	
	argv[0] = "";
	argv[1] = "Tests.txt";
	argv[2] = "out.txt";
	//Так то это работает и для дебага подходит, но уже не нужно
	*/

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
	int fileSize = 0;
	int nStrings = 0;

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

	/*int nNewLineSymbols = 0;
	for (int i = 1; i != fileSize; i++)
	{
		if (translatedFile[i] == '\n' && )
			nNewLineSymbols++;
	}*/

	char** strings = (char**)calloc(nStrings, sizeof(char*)); //А здесь сам массив
	if (!strings)
	{
		printf("Error occured while calling calloc\n");
		return 0;
	}

#ifndef   JULESIMF_NO_OUTPUT
	printf("Separating strings...\n");
#endif //!JULESIMF_NO_OUTPUT
	int RETURN_readStrings = separateStrings(translatedFile, fileSize, strings, 
	                                         nStrings, LENGTH_string);

	assert(strings[0]);

	if (RETURN_readStrings == 1)
	{
		printf("Error occured while separating file into strings\n");
		return 0;
	}

	//if (RETURN_readStrings == 2)
	//{
	//	printf("A line containing at least %d symbols was found"
	//		   " - maximal length is %d\n", LENGTH_string, LENGTH_string - 1);
	//	return 0;
	//}
	

#ifdef    JULESIMF_DEBUG
	printf("\n\n.....Testing the correctness of input\n");
	assert(strings != NULL);
	for (int i = 0; i != nStrings; i++)
	{
		printf("%s\n", strings[i]);
	}
	printf(".....Test ended\n\n");
#endif //!JULESIMF_DEBUG


#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem...\n");
#endif //!JULESIMF_NO_OUTPUT

	qsort((void*)strings, nStrings, sizeof(char*), stringComparator);

#ifdef    JULESIMF_DEBUG
	printf("\n\n.....Testing the correctness of sort\n");
	assert(strings != NULL);
	for (int i = 0; i != nStrings; i++)
	{
		printf("%s\n", strings[i]);
	}
	printf("\n\n.....Test ended\n");
#endif //!JULESIMF_DEBUG


#ifndef   JULESIMF_NO_OUTPUT
	printf("Output file opened\n");
#endif //!JULESIMF_NO_OUTPUT
	//Файл с данными
	FILE* sortedPoem = fopen(argv[2], "w");
	if (!sortedPoem)
	{
		printf("Failed to open file \"%s\"\n", argv[2]);
		return 0;
	}


#ifndef   JULESIMF_NO_OUTPUT
	printf("Writing output...\n");
#endif //!JULESIMF_NO_OUTPUT
	//Поехали выводить сортед поем
	for (int i = 0; i != nStrings; i++)
		fprintf(sortedPoem, "%s\n", strings[i]);

#ifndef   JULESIMF_NO_OUTPUT
	printf("Output file closed\n");
#endif //!JULESIMF_NO_OUTPUT
	fclose(sortedPoem);
	printf("Executed correctly\n");
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
			continue;
		}
		return (int)(*first) - (int)(*second);
	}

	return (((*first) == (*second)) ? 0 : (int)(*first) - (int)(*second));
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
		return NULL;

	int RETURN_fread = fread(translatedFile, 1, fileSize, file);
	((char*)translatedFile)[RETURN_fread] = '\0'; //По новой спецификации функции (чисто поржать)
	fseek(file, 0, SEEK_SET);


	/*if (RETURN_fread != fileSize)
		return NULL;*/


#ifdef    JULESIMF_DEBUG
	printf(".....Translated file:\n");
	char* fileChars = (char*)translatedFile;
	for (int i = 0; i != RETURN_fread; i++)
	{
		if (!fileChars[i])
		{
			printf("\\0");
			continue;
		}
		if (fileChars[i] == '\n')
		{
			printf("\\n");
			continue;
		}
		putchar(fileChars[i]);
	}
	putchar('\n');
	printf(".....Translated file end\n\n");
	printf("filesize = %d\nfread = %d\ndelta = %d\n\n", 
		   fileSize, RETURN_fread, fileSize - RETURN_fread);
#endif //!JULESIMF_DEBUG
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
	char* currentString = NULL;

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
			destination[iDestination++] = currentString;
			currentString = NULL;
		}
	}

	if (iDestination != nStrings)
		return 1;

	return 0;
}
