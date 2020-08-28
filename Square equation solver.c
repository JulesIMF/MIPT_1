//Square equation solver
//Иван Дорошенко, ИВТ
#define JULESIMF_DEBUG
///Из-за специфики VS надо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <assert.h>

///Минимальная разница между корнями, при которой они считаются различными
#define MIN_DIFFERENCE          1e-4  
///Код выхода из программы при неизвестной ошибке
#define EXIT_UNKNOWN_EXCEPTION  1     

/**
 * Хранит тип решения
 */
enum SolutionType {
    TWO_ROOTS,                ///<[0] Два корня квадратного уравнения
    ONE_ROOT,                 ///<[1] Один корень квадратного уравнения
    NO_REAL_ROOTS,            ///<[2] Для квадратного уравнения не существуют действительные решения
    LINEAR_NO_SOLUTION,       ///<[3] Уравнение вырождено в линейное и не имеет решений
    LINEAR_INFINITE_SOLUTION, ///<[4] Уравнение вырождено в линейное и имеет бесконечное число решений
    LINEAR_ONE_ROOT,          ///<[5] Уравнение вырождено в линейное и имеет один корень
    UNDEFINED_SOLUTION        ///<[6] Неизвестная ошибка
};

int isZero(double d)
{
    return (fabs(d) < MIN_DIFFERENCE) ? 1 : 0;
}

/**
 *  \brief Функция для решения квадратного уравнения
 *
 *  \param a старший коэффициент
 *  \param b средний коэффициент
 *  \param c свободный коэффициент
 *  \param x1 указатель на первый корень
 *  \param x2 указатель на второй корень
 *  \return тип решения для данного уравнения
 */
enum SolutionType getSolution(double a, double b, double c,
    double* x1, double* x2);

/**
 * \brief Проверяет void getSolution(void)
 * 
 * Формат тестов:
 * [количество тестов]
 * [type] [a] [b] [c] [x1] [x2] //Соответсвует количеству тестов
 * 
 * \param testsFile имя файла с тестами
 * 
 */
void CHECKER_getSolution(char const*);

/**
 * \brief Точка входа в программу.
 * 
 * \return 0 в случае корректного завершения работы, 1 в случае неизвестной ошибки
 */
int main()
{
#ifdef    JULESIMF_DEBUG
    printf("..............DEBUG MODE..............\n\n\n");
    CHECKER_getSolution("Tests.txt");
#endif //!JULESIMF_DEBUG
    printf("Square equation solver\n"
        "JulesIMF\n"
        "Equation format is a*(x^2) + b*x + c\n\n");

    double a = 0.0, b = 0.0, c = 0.0; //Коэффициенты уравнения

    double x1 = 0.0, x2 = 0.0; //Корни уравнения


    //Считываем коэффициенты, пока они не будут введены корректно
    printf("Enter a, b, c: ");
    while (scanf("%lf %lf %lf", &a, &b, &c) != 3)
    {
        printf("Incorrect input\n\nEnter a, b, c: ");
        //На плюсах вроде fflush не работает, а вот на Си работает
        fflush(stdin);
    }

    //Получаем решение
    enum SolutionType type = UNDEFINED_SOLUTION;
    type = getSolution(a, b, c, &x1, &x2);


#ifdef    JULESIMF_DEBUG
    assert(type != UNDEFINED_SOLUTION);
#endif //!JULESIMF_DEBUG

    //Вывод решения
    switch (type)
    {
    case TWO_ROOTS:
        printf("x1 = %.5lf\n"
            "x2 = %.5lf\n",
            x1, x2);
        break;

    case ONE_ROOT:
        printf("x1 = x2 = %.5lf\n", x1);
        break;

    case NO_REAL_ROOTS:
        printf("No real roots\n");
        break;

    case LINEAR_NO_SOLUTION:
        printf("No solution\n");
        break;

    case LINEAR_INFINITE_SOLUTION:
        printf("Infinite solution\n");
        break;

    case LINEAR_ONE_ROOT:
        printf("x = %.5lf\n", x1);
        break;

    default:
        printf("Unknown exception\n");
        return EXIT_UNKNOWN_EXCEPTION;
    }

    return 0; //Пишем чисто по приколу
}

enum SolutionType getSolution(double a, double b, double c,
    double* x1, double* x2)
{
#ifdef    JULESIMF_DEBUG
    assert(!isnan(a));
    assert(!isnan(b));
    assert(!isnan(c));
    assert(x1 != x2);
    assert(x1 != NULL);
    assert(x2 != NULL);
#endif //!JULESIMF_DEBUG


    //Случай линейного уравнения
    if (a == 0.0)
    {
        if (b == 0.0)
        {
            *x1 = *x2 = NAN;
            if (c == 0.0)
                return LINEAR_INFINITE_SOLUTION;
            else
                return LINEAR_NO_SOLUTION;
        }

        *x1 = *x2 = -c / b;
        return LINEAR_ONE_ROOT;
    }

    //Случай квадратного уравнения
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0.0)
    {
        *x1 = *x2 = NAN;
        return NO_REAL_ROOTS;
    }

    double discriminantRoot = sqrt(discriminant);
    *x1 = (-b - discriminantRoot) / (2.0 * a);
    *x2 = (-b + discriminantRoot) / (2.0 * a);

    if (isZero(*x1 - *x2))
        return ONE_ROOT;
    else
        return TWO_ROOTS;
}

void CHECKER_getSolution(char const* testsFile)
{
    printf("....CHECKER_getSolution loaded\n");
    double _x1 = 0, _x2 = 0;    //Проверяемые корни
    SolutionType _type; //Проверяемое возвращаемое значение
    int nReaded;        //Переменная для проверки корректности вводе
    
    FILE* tests = fopen(testsFile, "r");
    assert(tests != NULL);
    printf(".......Test file \"%s\" loaded\n", testsFile);

    int nTests; //Количество тестов
    nReaded = fscanf(tests, "%d", &nTests);
    assert(nReaded == 1);

    double a = 0,  b = 0,  c = 0;
    double x1 = 0, x2 = 0;
    enum SolutionType type = UNDEFINED_SOLUTION;

    printf(".......%d test(s)\n\n", nTests); //Выводим кол-во переменных

    for (int i = 0; i != nTests; i++)
    {
        nReaded = fscanf(tests, "%d %lf %lf %lf %lf %lf", &type, &a, &b, &c, &x1, &x2);
        assert(nReaded == 6);
        _type = getSolution(a, b, c, &_x1, &_x2);

        printf(".......Test #%d : %d %lf %lf %lf %lf %lf\n", i, type, a, b, c, x1, x2); //Выводим тест и номер теста

        //Тип всегда должен совпадать
        assert(type == _type);
        if (type != TWO_ROOTS &&
            type != ONE_ROOT &&
            type != LINEAR_ONE_ROOT) continue;

        //Проверять надо только если первый корень есть
        assert(isZero(x1 - _x1));

        if (type != TWO_ROOTS)
            continue;

        //Проверять надо только если второй корень есть
        assert(isZero(x2 - _x2));
    }
    printf("....CHECKER_getSolution exited\n\n\n\n");
}
