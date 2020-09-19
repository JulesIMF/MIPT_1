/*
Copyright (c) 2020  MIPT
Name:
	Бредогенератор
Abstract:
	Сортирует строки входного файла и выводит в другой файл
Author:
	JulesIMF
Last Edit:
	19.09.2020 15:01
Edit Notes:
	1) Теперь нет строк с одинаковыми словами в конце
	2) Исправлен баг с переводом строки в именах файлов
*/

//#define JULESIMF_DEBUG
#define JULESIMF_NO_OUTPUT

///Из-за специфики VS необходимо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <Windows.h>
#include "Line.h"
#include "Comparasion.h"
#include "Files.h"
#include "Common.h"
#include "Delusion.h"


//*************************************************************************************************	
//*************************************************************************************************	
//******************************************MAIN
//*************************************************************************************************
//*************************************************************************************************	

int main(int argc, char const** argv)
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


//*************************************************************************************************	
//******************************************ИМЕНА ФАЙЛОВ
//*************************************************************************************************
	char const* inputFileName = 0;
	char const* outputFileName = 0;
	int toFreeIONames = 0;
#ifdef    JULESIMF_DEBUG
	//Для извращенского дебага все можно
	argc = 3;
#endif //!JULESIMF_DEBUG

	if (argc && !argv)
		jerror("Error occured while passing the parameters");

	switch (argc)
	{
	case 3:
		inputFileName  = argv[1];
		outputFileName = argv[2];
		break;

	case 1:
	{
		char* _input = (char*)calloc(256, sizeof(char));
		char* _output = (char*)calloc(256, sizeof(char));

		if (_input == 0 || _output == 0)
			jerror("Unknown calloc error");

		printf("Enter input file name: ");
		fgets(_input, 256, stdin);
		fflush(stdin);

		printf("Enter output file name: ");
		fgets(_output, 256, stdin);
		fflush(stdin);

		replaceNewLine(_input);
		replaceNewLine(_output);
		inputFileName = _input;
		outputFileName = _output;

		toFreeIONames = 1;
	}
		break;

	default:
		jerror("Invalid parameters");
	}

#ifdef    JULESIMF_DEBUG
	//Для извращенского дебага все можно
	inputFileName = "Tests.txt";
	outputFileName = "out.txt";
#endif //!JULESIMF_DEBUG
	

	//Получим размер файла и транслируем его в память
	int fileSize = 0; //Размер файла запишется сюда
	int nStrings = 0; //Количество строк запишется сюда

//*************************************************************************************************	
//******************************************ТРАНСЛЯЦИЯ ДАННЫХ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Translating file into RAM...\n");
#endif //!JULESIMF_NO_OUTPUT

	void* translatedFile = translateFileIntoRam(inputFileName, &fileSize, &nStrings);


	if (!translatedFile)
		jerror("Error occured while translating file into RAM");

	Line* strings = (Line*)calloc(nStrings, sizeof(Line)); //А здесь сам массив
	if (!strings)
		jerror("Error occured while calling calloc");

//*************************************************************************************************	
//******************************************РАЗДЕЛЕНИЕ НА СТРОКИ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Separating strings...\n");
#endif //!JULESIMF_NO_OUTPUT


	int RETURN_readStrings = separateStrings(translatedFile, fileSize, strings,
		nStrings);

	if (RETURN_readStrings == 1)
		jerror("Error occured while separating file into strings");
	
	
	/*char i = 0;
	for (int c = 0; c != 256; c++)
	{
		printf("%d: %c\n", (int)c, (c + i));
	}*/
	

//*************************************************************************************************	
//******************************************СОРТИРОВКА СТРОК В ОБРАТНОМ ПОРЯДКЕ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem in reverse...\n");
#endif //!JULESIMF_NO_OUTPUT



	qsort((void*)strings, nStrings, sizeof(Line), reverseStringComparator);

	int nLinesInPoem = 0;
	printf("Enter number of lines in a delusion: ");
	while (!scanf("%d", &nLinesInPoem))
	{
		fflush(stdin);
		printf("Wrong format, enter once again: ");
	}

	Line* delusion = generateDelusion(nLinesInPoem, strings, nStrings);

	if (delusion == 0)
		jerror("Wrong parameters passed into generateDelusion()");

	if (write(outputFileName, delusion, nLinesInPoem, "w"))
	{
		printf("Failed to open file \"%s\"", outputFileName);
		return 0;
	}


	free(strings);
	free(translatedFile);
	free(delusion);

	if (toFreeIONames)
	{
		free((void*)inputFileName);
		free((void*)outputFileName);
	}
#ifndef JULESIMF_NO_OUTPUT
	printf("\n\n"); //только если до этого все подряд выводилось
#endif // !JULESIMF_NO_OUTPUT

	printf("Executed correctly\n");
	return 0;
}