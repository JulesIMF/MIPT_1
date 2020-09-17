/**
 * \brief ��������� ���� � RAM � ��������� 0 ����� ����, ��� ���� ����� 0 ���� ��� ���� ��������� ����.
 *
 * \param fileName ����������� ����
 * \param fileSize ��������� �� ����������, � ������� ����� ������� ������ �����
 * \param nStrings ��������� �� ����������, � ������� ����� �������� ���������� �����
 * \return ��������� �� ������� ������, � ������� �������� ����, 0 � ������ ������
 */
void* translateFileIntoRam(char const* fileName, int* pFileSize, int* nStrings);


/**
 * \brief ������� ������ ����� � ����.
 * \param fileName ������ � ������ �����
 * \param strings ������ �����
 * \param nStrings ��������� ����� � �������
 * \param mode ����� �������� �����
 * \return 0 � ������ ��������� ����������, 1 � ������ ������ �������� �����
 */
int write(char const* fileName, string_view* strings, int nStrings, char const* mode);


/**
 * \brief ���������� ������ �����
 * \param file ����������� ����
 * \return ������ ����� � ������.
 */
int getFileSize(FILE* file);


/**
 * \brief ��������� source �� ������ � destination.
 * \param source ��������� �� ��������������� ����
 * \param fileSize ������ ���������������� �����
 * \param destination ��������� �� ������ �� ��������
 * \param nStrings ���������� �����
 * \warning source �� ��������������!!
 * \warning ������ *stringsLengths ��������������!!
 * \return 0 � ������ ��������� ����������, 1 ���� �������� �������������� ���������, 2 ���� ���� ������ ������� 100
 */
int separateStrings(void* source, int fileSize, string_view* destination, int nStrings);