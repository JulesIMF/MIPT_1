#ifndef JULESIMF_STRING_VIEW_INCLUDED
#define JULESIMF_STRING_VIEW_INCLUDED
/**
 * Структура, хранящая указатель на строку и ее предподсчитанный размер.
 */
typedef struct string_view_
{
	char* string;
	int size;
} string_view;
#endif // !JULESIMF_STRING_VIEW_INCLUDED

