/*
Copyright (c) 2020  MIPT
Name:
	Сортировка Онегина
Abstract:
	Сортирует строки входного файла и выводит в другой файл
Author:
	JulesIMF
Last Edit:
	12.09.2020 22:02
Edit Notes:
	1) Структура string_view добавлена
	2) translateFileIntoRam теперь честно считает количество строк
	3) Функция getFileSize
*/

//#define JULESIMF_DEBUG
//#define JULESIMF_NO_OUTPUT

///Из-за специфики VS необходимо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "string_view.h"
#include "Comparasion.h"
#include "Files.h"


//*************************************************************************************************	
//*************************************************************************************************	
//******************************************MAIN
//*************************************************************************************************
//*************************************************************************************************	

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

	//Получим размер файла и транслируем его в память
	int fileSize = 0; //Размер файла запишется сюда
	int nStrings = 0; //Количество строк запишется сюда

//*************************************************************************************************	
//******************************************ТРАНСЛЯЦИЯ ДАННЫХ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Translating file into RAM...\n");
#endif //!JULESIMF_NO_OUTPUT

	void* translatedFile = translateFileIntoRam(argv[1], &fileSize, &nStrings);


	if (!translatedFile)
	{
		printf("Error occured while translating file into RAM\n");
		return 0;
	}

	string_view* strings = (string_view*)calloc(nStrings, sizeof(string_view)); //А здесь сам массив
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
		nStrings);

	if (RETURN_readStrings == 1)
	{
		printf("Error occured while separating file into strings\n");
		return 0;
	}

	string_view* originalStrings = (string_view*)calloc(nStrings, sizeof(string_view)); //А здесь сам массив


#ifndef   JULESIMF_DEBUG
	assert(originalStrings);
#endif //!JULESIMF_DEBUG

	memcpy(originalStrings, strings, nStrings * sizeof(string_view));



//*************************************************************************************************	
//******************************************СОРТИРОВКА СТРОК В ПРЯМОМ ПОРЯДКЕ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem in direct...\n");
#endif //!JULESIMF_NO_OUTPUT

	qsort((void*)strings, nStrings, sizeof(string_view), stringComparator);
	

	if (write(argv[2], strings, nStrings, "w"))
	{
		printf("Failed to open file \"%s\"", argv[2]);
		return 0;
	}

//*************************************************************************************************	
//******************************************СОРТИРОВКА СТРОК В ОБРАТНОМ ПОРЯДКЕ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem in reverse...\n");
#endif //!JULESIMF_NO_OUTPUT

#ifdef    JULESIMF_DEBUG
	FILE* file = fopen(argv[2], "a");
	fprintf(file, "\n\n\n");
	fclose(file);
#endif //!JULESIMF_DEBUG


	qsort((void*)strings, nStrings, sizeof(string_view), reverseStringComparator);

	if (write(argv[2], strings, nStrings, "a"))
	{
		printf("Failed to open file \"%s\"", argv[2]);
		return 0;
	}

//*************************************************************************************************	
//******************************************ВЫВОД В ОРИГИНАЛЬНОМ ПОРЯДКЕ
//*************************************************************************************************
#ifdef    JULESIMF_DEBUG
	FILE* file_ = fopen(argv[2], "a");
	fprintf(file_, "\n\n\n");
	fclose(file_);
#endif //!JULESIMF_DEBUG

	if (write(argv[2], originalStrings, nStrings, "a"))
	{
		printf("Failed to open file \"%s\"", argv[2]);
		return 0;
	}


	free(strings);
	free(originalStrings);
	free(translatedFile);
#ifndef JULESIMF_NO_OUTPUT
	printf("\n\n");
#endif // !JULESIMF_NO_OUTPUT

	printf("Executed correctly\n");
	return 0;
}