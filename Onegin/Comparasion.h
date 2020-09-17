/**
 * Прямой компаратор для строк.
 *
 * \param pFirst Указатель на первый элемент
 * \param pSecond Указатель на второй элемент
 * \return 0 в случае равенства строк, иначе разницу между первыми отличающимися символами
 */
int stringComparator(void const* pFirst, void const* pSecond);


/**
 * Обратный компаратор для строк.
 *
 * \param pFirst Указатель на первый элемент
 * \param pSecond Указатель на второй элемент
 * \return 0 в случае равенства строк, иначе разницу между первыми отличающимися символами
 */
int reverseStringComparator(void const* pFirst, void const* pSecond);


/**
 * \brief Отвечает, является ли символ допустимым.
 * \param c принимаемый символ
 * \return 1 если символ допустимый, 0 иначе
 */

int isAllowedChar(char c);