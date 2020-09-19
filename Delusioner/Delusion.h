#pragma once
#include "Line.h"
/**
 * .
 *
 * \param nLines ���������� ������������� ����� (����������� �� 4)
 * \param source �������� ������
 * \param sourceSize
 * \return ��������� �� ����
 */
Line* generateDelusion(unsigned int nLines, Line const* source, size_t const sourceSize);

/**
 * ���������, ��������� �� ������� ��� ���.
 * \param first ������ �������
 * \param second ������ �������
 * \return 1 ���� ������� ���������, 2 ���� ���.
 */
int isRhyme(Line first, Line second);