/*
Copyright (c) 2020  MIPT
Name:
	Delusion
Abstract:
	Реализует функцию бредогенерации
Author:
	JulesIMF
Last Edit:
	19.09.2020 14:59
Edit Notes:
	1) compareLastWords
*/
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "Line.h"
#include "Comparasion.h"
#include "Common.h"

#define MAX_CYCLE 1000

#define poemStep(i) ( (i&1) ? (i+=3) : (i++))


/**
 * Проверяет на совпадение последних слов.
 * 
 * \param first первая строчка
 * \param second вторая строчка
 * \param iFirst указатель на ПОСЛЕДНЮЮ БУКВУ ПОСЛЕДНЕГО СЛОВА first
 * \param iSecond указатель на ПОСЛЕДНЮЮ БУКВУ ПОСЛЕДНЕГО СЛОВА second
 * \return 1 если они совпадают, 0 иначе
 */
static int compareLastWords(char const* first, char const* second, int iFirst, int iSecond)
{
	while (iFirst >= 0 && iSecond >= 0 &&
		isAllowedChar(first[iFirst]) &&
		isAllowedChar(second[iSecond]))
	{
		if (first[iFirst] != second[iSecond])
			return 0;

		iFirst--, iSecond--;
	}

	return 1;
}

int isRhyme(Line first, Line second)
{
	//Строки с разным количеством слогов уходят
	if (first.nSyllables != second.nSyllables)
		return 0;

	int iFirst = first.size - 1;
	int iSecond = second.size - 1;
	char const* sFirst = first.string;
	char const* sSecond = second.string;


	//Переведем указатель на начало БУКВ
	do {
		iFirst--;
	} while (iFirst >= 0 && !isAllowedChar(sFirst[iFirst]));

	do {
		iSecond--;
	} while (iSecond >= 0 && !isAllowedChar(sSecond[iSecond]));

	//Если строки оканчиваются неодинаковой буквой, они уходят
	if (sFirst[iFirst] != sSecond[iSecond])
		return 0;

	//Если последние слова одинаковы, это неинтересно
	if (compareLastWords(sFirst, sSecond, iFirst, iSecond))
		return 0;

	//Строчки без букв
	if (iFirst < 0 || iSecond < 0)
		return 0;

	//Если последние буквы - гласные, то надо декрементировать итераторы
	if (isVowel(sFirst[iFirst]) && isVowel(sSecond[iSecond]))
		iFirst--, iSecond--;

	//Идем до первой гласной
	while (iFirst >= 0 && iSecond >= 0 &&
		!isVowel(sFirst[iFirst]) && !isVowel(sSecond[iSecond]))
	{
		//Несовпадение? Капец, return 0
		if (sFirst[iFirst] != sSecond[iSecond])
			return 0;

		//Пропускаем все лишнее
		do {
			iFirst--;
		} while (iFirst >=0 && !isAllowedChar(sFirst[iFirst]));
		
		do {
			iSecond--;
		} while (iSecond >= 0 && !isAllowedChar(sSecond[iSecond]));
	}

	//1 только если есть гласные и последние совпадают
	return (iFirst >= 0) && (iSecond >= 0) &&
		isVowel(sFirst[iFirst]) &&
		isVowel(sSecond[iSecond]) == isVowel(sFirst[iFirst]);
}


/**
 * .
 *
 * \param nLines количество запрашиваемых строк (округляется до 4)
 * \param source исходные строки
 * \param sourceSize
 * \return указатель на бред
 */
Line* generateDelusion(unsigned int nLines, Line const* source, size_t const sourceSize)
{
#ifdef JULESIMF_DEBUG
	assert(source);
#endif // JULESIMF_DEBUG

	if (sourceSize < 2)
		return 0;

	if (nLines & 3)
		nLines = (1 + (nLines >> 2)) << 2;

	Line* delusion = (Line*)calloc(nLines, sizeof(Line));

	if (delusion == 0)
		jerror("Calloc error in delusion generator");

	srand(time(0)); //123

	for (int i = 0; i != nLines; poemStep(i))
	{
		int cycle = 0;
		int itSource = rand() % (sourceSize - 1);
		while (!isRhyme(source[itSource], source[itSource + 1]) 
			&& cycle < MAX_CYCLE)

			itSource = rand() % (sourceSize - 1), cycle++;

		if (cycle == MAX_CYCLE)
			jerror("Programm has entered into a cycle");

		delusion[i] = source[itSource];
		delusion[i + 2] = source[itSource + 1];
	}

	return delusion;
}
