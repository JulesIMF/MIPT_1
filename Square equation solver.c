//Square equation solver
//Иван Дорошенко, ИВТ

//Из-за специфики VS надо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <assert.h>

#define MIN_DIFFERENCE          1e-4
#define EXIT_UNKNOWN_EXCEPTION  1

enum SolutionType {
    TWO_ROOTS,
    ONE_ROOT,
    NO_REAL_ROOTS,
    LINEAR_NO_SOLUTION,
    LINEAR_INFINITE_SOLUTION,
    LINEAR_ONE_ROOT,
    UNDEFINED_SOLUTION
};

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

    if (fabs(*x1 - *x2) < MIN_DIFFERENCE)
        return ONE_ROOT;
    else
        return TWO_ROOTS;
}

//Точка входа в программу
int main()
{
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
}
