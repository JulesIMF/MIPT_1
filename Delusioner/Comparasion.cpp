/*
Copyright (c) 2020  MIPT
Name:
	Comparasion
Abstract:
	��������� ������� ��������� �����
Author:
	JulesIMF
Last Edit:
	19.09.2020 0:28
Edit Notes:
	1) ����� ������ ����������
	2) ������� isVovel
	3) �������� ���������� ��������� ���������� ������
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Line.h"
#include "Comparasion.h"
#include "Files.h"

//#define JULESIMF_DEBUG
#define JULESIMF_NO_OUTPUT


int reverseStringComparator(void const* pFirst, void const* pSecond)
{
#ifdef    JULESIMF_DEBUG
	assert(pFirst);
	assert(pSecond);
#endif //!JULESIMF_DEBUG

	Line* stringFirst  = (Line*)pFirst;
	Line* stringSecond = (Line*)pSecond;

	//������ ������ ���� ������������� � ������ ������� �� ���������� ������
	{
		int syllablesDelta =
			stringFirst->nSyllables - stringSecond->nSyllables;
		if (syllablesDelta)
			return syllablesDelta;
	}

	//� ��� ��� ������
	char* first  = stringFirst ->string;
	char* second = stringSecond->string;
	int L_first  = stringFirst ->size;
	int L_second = stringSecond->size;


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

		cFirst = *(first + L_first - 1 - iFirst);
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


int isAllowedChar(char c)
{
	
	if ('�' <= c && c <= '�')
		return 1;
	if ('�' <= c && c <= '�')
		return 1;
	if (c < 0)
		return 0;
	if (isalpha(c))
		return 1;

	return 0;
}

int isVowel(char c)
{
	static char vowels[] =
	{
		'�', '�', '�', '�', '�',
		'�', '�', '�', '�', '�',
		'�', '�', '�', '�', '�',
		'�', '�', '�', '�', '�'
	};

	for (int i = 0; i != sizeof(vowels); i++)
		if (c == vowels[i])
			return i%5;

	return 0;
}