/*
Copyright (c) 2020  MIPT
Name:
	Delusion
Abstract:
	��������� ������� ��������������
Author:
	JulesIMF
Last Edit:
	19.09.2020 0:27
Edit Notes:
	1) ������� 
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
 * ��������� �� ���������� ��������� ����.
 * 
 * \param first ������ �������
 * \param second ������ �������
 * \param iFirst ��������� �� ��������� ����� ���������� ����� first
 * \param iSecond ��������� �� ��������� ����� ���������� ����� second
 * \return 1 ���� ��� ���������, 0 �����
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
	//������ � ������ ����������� ������ ������
	if (first.nSyllables != second.nSyllables)
		return 0;

	int iFirst = first.size - 1;
	int iSecond = second.size - 1;
	char const* sFirst = first.string;
	char const* sSecond = second.string;


	//��������� ��������� �� ������ ����
	do {
		iFirst--;
	} while (iFirst >= 0 && !isAllowedChar(sFirst[iFirst]));

	do {
		iSecond--;
	} while (iSecond >= 0 && !isAllowedChar(sSecond[iSecond]));

	//���� ������ ������������ ������������ ������, ��� ������
	if (sFirst[iFirst] != sSecond[iSecond])
		return 0;

	//���� ��������� ����� ���������, ��� �����������
	if (compareLastWords(sFirst, sSecond, iFirst, iSecond))
		return 0;

	//������� ��� ����
	if (iFirst < 0 || iSecond < 0)
		return 0;

	//���� ��������� ����� - �������, �� ���� ���������������� ���������
	if (isVowel(sFirst[iFirst]) && isVowel(sSecond[iSecond]))
		iFirst--, iSecond--;

	//���� �� ������ �������
	while (iFirst >= 0 && iSecond >= 0 &&
		!isVowel(sFirst[iFirst]) && !isVowel(sSecond[iSecond]))
	{
		//������������? �����, return 0
		if (sFirst[iFirst] != sSecond[iSecond])
			return 0;

		//���������� ��� ������
		do {
			iFirst--;
		} while (iFirst >=0 && !isAllowedChar(sFirst[iFirst]));
		
		do {
			iSecond--;
		} while (iSecond >= 0 && !isAllowedChar(sSecond[iSecond]));
	}

	//1 ������ ���� ���� ������� � ��������� ���������
	return (iFirst >= 0) && (iSecond >= 0) &&
		isVowel(sFirst[iFirst]) &&
		isVowel(sSecond[iSecond]) == isVowel(sFirst[iFirst]);
}


/**
 * .
 *
 * \param nLines ���������� ������������� ����� (����������� �� 4)
 * \param source �������� ������
 * \param sourceSize
 * \return ��������� �� ����
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