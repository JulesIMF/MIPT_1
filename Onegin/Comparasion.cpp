/*
Copyright (c) 2020  MIPT
Name:
	Comparasion
Abstract:
	Реализует функции сравнения строк
Author:
	JulesIMF
Last Edit:
	12.09.2020 22:02
Edit Notes:
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "string_view.h"
#include "Comparasion.h"
#include "Files.h"

//#define JULESIMF_DEBUG
#define JULESIMF_NO_OUTPUT

int stringComparator(void const* pFirst, void const* pSecond)
{
#ifdef    JULESIMF_DEBUG
	assert(pFirst);
	assert(pSecond);
#endif //!JULESIMF_DEBUG


	//pFirst и pSecond указывают не на строки, а на указатель на строки

	//А это уже строки
	char* first = ((string_view*)pFirst)->string;
	char* second = ((string_view*)pSecond)->string;

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
	char* first = ((string_view*)pFirst)->string;
	char* second = ((string_view*)pSecond)->string;
	int L_first = ((string_view*)pFirst)->size;
	int L_second = ((string_view*)pSecond)->size;


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
	if (isalpha(c))
		return 1;


	return 0;
}