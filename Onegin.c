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
 * \brief Загружает файл в RAM и вставляет 0 после него, при этом после 0 есть еще один доступный байт.
 *
 * \param fileName загружаемый файл
 * \param fileSize указатель на переменную, в которую будет записан размер файла
 * \param nStrings указатель на переменную, в которую будет записано количество строк
 * \return указатель на область памяти, в которую загружен файл, 0 в случае ошибки
 */
void* translateFileIntoRam(char const* fileName, int* pFileSize, int* nStrings);


/**
 * \brief Разбивает source на строки в destination.
 * \param source указатель на транслированный файл
 * \param fileSize размер транслированного файла
 * \param destination указатель на массив со строками
 * \param nStrings количество строк
 * \warning source не перекопируется!!
 * \warning массив *stringsLengths перевыделяется!!
 * \return 0 в случае успешного выполнения, 1 если переданы противоречивые аргументы, 2 если есть строка длиннее 100
 */
int separateStrings(void* source, int fileSize, struct string_view* destination, int nStrings);

/**
 * \brief Выводит массив строк в файл.
 * \param fileName строка с именем файла
 * \param strings массив строк
 * \param nStrings кличество строк в массиве
 * \param mode режим открытия файла
 * \return 0 в случае успешного исполнения, 1 в случае ошибки открытия файла
 */
int write(char const* fileName, struct string_view* strings, int nStrings, char const* mode);

/**
 * \brief Отвечает, является ли символ допустимым.
 * \param c принимаемый символ
 * \return 1 если символ допустимый, 0 иначе
 */

int isAllowedChar(char c)
{
	if (isalpha(c))
		return 1;


	return 0;
}

/**
 * \brief Возвращает размер файла
 * \param file принимаемый файл
 * \return размер файла в байтах.
 */
int getFileSize(FILE* file);

/**
 * Структура, хранящая указатель на строку и ее предподсчитанный размер.
 */
struct string_view
{
	char* string;
	int size;
};




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

	struct string_view* strings = (struct string_view*)calloc(nStrings, sizeof(struct string_view)); //А здесь сам массив
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

	struct string_view* originalStrings = (struct string_view*)calloc(nStrings, sizeof(struct string_view)); //А здесь сам массив

	//SEPARATE
	for (int i = 0; i != nStrings; i++)
	{
		printf("%d: (%p)\t(%d)\t(%s)\n", i, &strings[i], strings[i].size, strings[i].string);
	}


#ifndef   JULESIMF_DEBUG
	assert(originalStrings);
#endif //!JULESIMF_DEBUG

	memcpy(originalStrings, strings, nStrings * sizeof(struct string_view));



//*************************************************************************************************	
//******************************************СОРТИРОВКА СТРОК В ПРЯМОМ ПОРЯДКЕ
//*************************************************************************************************

#ifndef   JULESIMF_NO_OUTPUT
	printf("Sorting the poem in direct...\n");
#endif //!JULESIMF_NO_OUTPUT

	qsort((void*)strings, nStrings, sizeof(struct string_view), stringComparator);
	

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


	qsort((void*)strings, nStrings, sizeof(struct string_view), reverseStringComparator);

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

	printf("\n\nExecuted correctly\n");
	return 0;
}






//*************************************************************************************************	
//*************************************************************************************************	
//******************************************ОПРЕДЕЛЕНИЕ ФУНКЦИЙ
//*************************************************************************************************
//*************************************************************************************************	


int write(char const* fileName, struct string_view* strings, int nStrings, char const* mode)
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
		assert(strings[i].string);
#endif //!JULESIMF_DEBUG
		fprintf(file, "%s\n", strings[i].string);
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
	char* first  = ((struct string_view*)pFirst)-> string;
	char* second = ((struct string_view*)pSecond)->string;

	while ((*first) && (*second))
	{
		while (!isAllowedChar(*first) && (*first))
			first++;
		while (!isAllowedChar(*second) && (*second))
			second++;

		if ((*first) == (*second))
		{
			first++;
			second++;
#ifdef    JULESIMF_DEBUG
			assert(first);
			assert(second);
#endif //!JULESIMF_DEBUG
		}
		else
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

	

	//А это уже строки
	char* first  = ((struct string_view*)pFirst)-> string;
	char* second = ((struct string_view*)pSecond)->string;
	int L_first  = ((struct string_view*)pFirst)-> size;
	int L_second = ((struct string_view*)pSecond)->size;


	char cFirst = '\0', cSecond = '\0';

	int iFirst = 0, iSecond = 0;
	while (iFirst < L_first || iSecond < L_second)
	{
		while (iFirst < L_first &&
			!isAllowedChar(*(first + L_first - 1 - iFirst)))

			iFirst++;

		while (iSecond < L_second &&
			!isAllowedChar(*(second + L_second - 1 - iSecond)))

			iSecond++;

		cFirst  = *(first  + L_first  - 1 - iFirst);
		cSecond = *(second + L_second - 1 - iSecond);

		if (cFirst == cSecond)
		{
			iFirst++;
			iSecond++;
			continue;
		}

		return cFirst - cSecond;
	}

	return L_first - L_second;
}

int getFileSize(FILE* file)
{
#ifdef    JULESIMF_DEBUG
	assert(file);
#endif //!JULESIMF_DEBUG

	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	return fileSize;
}

void* translateFileIntoRam(char const* fileName, int* pFileSize, int* nStrings)
{
	//Файл с данными
	FILE* file = fopen(fileName, "r");
	if (!file)
	{
		printf("Failed to open file \"%s\"\n", fileName);
		return 0;
	}
	else
	{
#ifndef   JULESIMF_NO_OUTPUT
		printf("Input file opened\n");
#endif //!JULESIMF_NO_OUTPUT
	}

#ifdef    JULESIMF_DEBUG
	assert(file);
	assert(pFileSize);
	assert(nStrings);
#endif //!JULESIMF_DEBUG
	int fileSize = getFileSize(file);
	void* translatedFile = malloc(fileSize + 2); //на всякий случай
	if (!translatedFile)
		return 0;

	int RETURN_fread = fread(translatedFile, 1, fileSize, file);
	((char*)translatedFile)[RETURN_fread] = '\0'; //По новой спецификации функции (чисто поржать)
	fseek(file, 0, SEEK_SET);

	int nNewLines = 0;
	for (int i = 0; i != RETURN_fread; i++)
	{
		if (((char*)translatedFile)[i] == '\n')
			nNewLines++;
	}
	*nStrings  = nNewLines + 1;
	*pFileSize = RETURN_fread;
#ifndef   JULESIMF_NO_OUTPUT
	printf("Input file closed\n");
#endif //!JULESIMF_NO_OUTPUT

	fclose(file);
	return translatedFile;
}

int separateStrings(void* source, int fileSize, struct string_view* destination, int nStrings)
{
#ifdef    JULESIMF_DEBUG
	assert(source);
	assert(destination);
#endif //!JULESIMF_DEBUG

	char* text = (char*)source;
	int   iDestination = 0; //Итератор для destination
	char* currentString = 0;
	int   currentStringLength = 0;

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
			struct string_view newString = 
				 { currentString, currentStringLength };

			destination[iDestination] = newString;
			iDestination++;
			currentString = 0;
			currentStringLength = 0;
		}
		else
			currentStringLength++;
	}

	if (iDestination != nStrings)
		return 1;

	return 0;
}
