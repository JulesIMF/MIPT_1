//Square equation solver
//Иван Дорошенко, ИВТ

//Из-за специфики VS надо объявить следующий макрос
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>

#define MIN_DIFFERENCE 1e-4

//Cтруктура, хранящая уравнение
//Формат уравнения a*x² + b*x + c
struct Equation
{
	double a, b, c;
};


//Структура, хранящая решение уравнения
struct Solution
{
	double x1, x2;          //Корни уравнения
	int isLinear;           //Флаг равен 1, если а == 0, иначе равен 0
	int isInfiniteSolution; //Флаг равен 1, если подходит любое число, иначе равен 0
	int isEmptySolution;    //Флаг равен 1, если не подходит ни одно число, иначе равен 0
	int isComplex;          //Флаг равен 1, если решения не действительные, иначе равен 0
	int isZeroDiscriminant; //Флаг равен 1, если дискриминант равен 0, иначе равен 0
};


//Эта функция инициализирует структуру Equation
void initializeEquation(Equation* equation)
{
	equation->a = 0.0;
	equation->b = 0.0;
	equation->c = 0.0;
}


//Эта функция инициализирует структуру Solution
void initializeSolution(Solution* solution)
{
	solution->x1 = solution->x2	 = 0.0;
	solution->isLinear		 = 0;
	solution->isComplex		 = 0;
	solution->isEmptySolution	 = 0;
	solution->isInfiniteSolution     = 0;
	solution->isZeroDiscriminant     = 0;
}


//Эта функция считывает уравнение и записывает его в equation
void scanEquation(Equation* equation)
{
	scanf("%lf %lf %lf",
		&(equation->a),
		&(equation->b),
		&(equation->c));
}


double getDiscriminant(Equation* equation)
{
	return (equation->b) * (equation->b)
	        - 4 * (equation->a) * (equation->c);
}


//Эта функция решает уравнение equation и записывает решение в solution
void calculateSolution(Equation* equation, Solution* solution)
{
	initializeSolution(solution);
	//Если a == 0, уравнение линейно
	if (equation->a == 0)
	{
		solution->isLinear = 1;
		
		//Если средний коэффициент равен 0, то либо нет решений, либо решений бесконечно много
		if (equation->b == 0)
		{
			if (equation->c == 0)
				solution->isInfiniteSolution = 1;  //Бесконечно много решений
			else
				solution->isEmptySolution    = 1;  //Нет решений

			return;
		}

		//Иначе корень есть и будет записан в x1
		solution->x1 = (-equation->c) / (equation->b);
		return;
	}

	//Если а != 0, это квадратное уравнение	
	double discriminant = getDiscriminant(equation);

	//Если дискриминант отрицательный, решений нет
	if (discriminant < 0.0)
	{
		solution->isComplex = 1;
		return;
	}

	double discriminantRoot = sqrt(discriminant);

	solution->x1 = (-equation->b - discriminantRoot) / (2.0 * equation->a);
	solution->x2 = (-equation->b + discriminantRoot) / (2.0 * equation->a);

	if (fabs(solution->x1 - solution->x2) < MIN_DIFFERENCE)
		solution->isZeroDiscriminant = 0; //Равные корни
}


//Вывод приветствия
void infoOut()
{
	printf("Square equation solver\n"
		   "JulesIMF\n"
		   "Equation format is a*(x^2) + b*x + c\n");
}


//Обработка решения
void proceedSolution(Solution* solution)
{
	//Линейное уравнение
	if (solution->isLinear)
	{
		//Пустое решение
		if (solution->isEmptySolution)
		{
			printf("Solution is empty\n");
			return;
		}

		//Бесконечное количество решений
		if (solution->isInfiniteSolution)
		{
			printf("Infinite solution\n");
			return;
		}


		//Иначе решение есть и единственно
		printf("x = %.5lf\n", solution->x1);
		return;
	}


	//Нет действительных решений
	if (solution->isComplex)
	{
		printf("No real solution\n");
		return;
	}

	//Решение одно
	if (solution->isZeroDiscriminant)
	{
		printf("x1 = x2 = %.5lf\n", solution->x1);
	}

	//Решений два
	else
	{
		printf("x1 = %.5lf\nx2 = %.5lf", solution->x1, solution->x2);
	}
}


//Точка входа в приложение
int main()
{
	infoOut();

	//Считываем уравнение
	Equation equation;
	printf("Enter a, b, c: ");
	scanEquation(&equation);

	//Решаем уравнение
	Solution solution;
	calculateSolution(&equation, &solution);

	//Обрабатываем решение
	proceedSolution(&solution);	
}
