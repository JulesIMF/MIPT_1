/*
Copyright (c) 2020  MIPT
Name:
	printf
Abstract:
	Имплементация printf
Author:
	JulesIMF
Last Edit:
	14.09.2020
Edit Notes:
	1) Выводит:
        *Процент
        *Целые со знаком
        *Символы
        *Строки
        *Количество выведенных символов
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef _MSC_VER
#define __crt_va_start va_start
#define __crt_va_end va_end
#define __crt_va_arg va_arg
#endif // !_MSC_VER


typedef enum
{
	VAL_NULL,
	VAL_PROCENT,
	VAL_DECIMAL,
	VAL_CHAR,
	VAL_STRING,
	VAL_NPRINTED,
	VAL_IGNORE
} jValueType_;


static jValueType_ jGetValueType_(char const* format, int* i)
{
	switch (*format)
	{
	case '\0':
		(*i)++;
		return VAL_NULL;
	case '%':
		(*i)++;
		return VAL_PROCENT;
	case 'd':
	case 'i':
		(*i)++;
		return VAL_DECIMAL;
	case 'c':
		(*i)++;
		return VAL_CHAR;
	case 's':
		(*i)++;
		return VAL_STRING;
	case 'n':
		(*i)++;
		return VAL_NPRINTED;
	default:
		(*i)++;
		return VAL_IGNORE;
	}
}


static void jPutInt_(int value, int* nPrinted)
{
	char digits[10];
	if (value < 0)
	{
		putc('-', stdout);
		(*nPrinted)++;
		value *= -1;
	}
	int nDigits = 0;
	do
	{
		assert(nDigits < 10);
		digits[nDigits++] = '0' + value % 10;
		value /= 10;
	} while (value);

	while (nDigits)
	{
		assert(nDigits >= 1);
		putc(digits[--nDigits], stdout);
		(*nPrinted)++;
	}
	return;
}


int jprintf(char const* const format, ...)
{
	int i = 0;
	int nPrinted = 0;
	if (format == 0)
		return -1;

	va_list values;
	__crt_va_start(values, format);

	while (format[i])
	{
		if (format[i] != '%')
		{
			putc(format[i], stdout);
			nPrinted++;
			i++;
		}

		else
		{
			i++;
			switch (jGetValueType_(format + i, &i))
			{
			case VAL_NULL:
				return nPrinted;
			case VAL_PROCENT:
				putc('%', stdout);
				nPrinted++;
				break;
			case VAL_DECIMAL:
				jPutInt_(__crt_va_arg(values, int), &nPrinted);
				break;
			case VAL_CHAR:
				putc(__crt_va_arg(values, char), stdout);
				nPrinted++;
				break;
			case VAL_STRING:
			{
				char const* string = __crt_va_arg(values, char const*);
				while (*string)
				{
					putc(*(string++), stdout);
					nPrinted++;
				}
				break;
			}
			case VAL_NPRINTED:
				jPutInt_(nPrinted, &nPrinted);
				break;
			case VAL_IGNORE:
				break;
			default:
				return -1;				
			}
		}
	}

	__crt_va_end(values);
	return nPrinted;
}
