#pragma once
/**
 * �������� ���������� ��� �����.
 *
 * \param pFirst ��������� �� ������ �������
 * \param pSecond ��������� �� ������ �������
 * \return 0 � ������ ��������� �����, ����� ������� ����� ������� ������������� ���������
 */
int reverseStringComparator(void const* pFirst, void const* pSecond);


/**
 * \brief ��������, �������� �� ������ ����������.
 * \param c ����������� ������
 * \return 1 ���� ������ ����������, 0 �����
 */

int isAllowedChar(char c);

/**
 * ���������, �������� �� ������������� ����� �������.
 * 
 * \param c ������ �����
 * \return 1 � ������ ������������� �������, 0 �����
 */
int isVowel(char c);