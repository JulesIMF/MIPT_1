#pragma once
#include "Line.h"
/**
 * .
 *
 * \param nLines количество запрашиваемых строк (округляется до 4)
 * \param source исходные строки
 * \param sourceSize
 * \return указатель на бред
 */
Line* generateDelusion(unsigned int nLines, Line const* source, size_t const sourceSize);

/**
 * Проверяет, рифмуются ли строчки или нет.
 * \param first первая строчка
 * \param second вторая строчка
 * \return 1 если строчки рифмуются, 2 если нет.
 */
int isRhyme(Line first, Line second);