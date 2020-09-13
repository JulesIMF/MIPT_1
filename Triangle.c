/*
Copyright (c) 2020  MIPT
Name:
	Треугольная адресация
Abstract:
	
Author:
	JulesIMF
Last Edit:
	14.09.2020 
Edit Notes:
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int progressionSum(size_t y)
{
    return y*(y+1)/2;
}

int triangleSize(size_t y)
{
    return progressionSum(y);
}

int getTriangleElement(int* array, size_t x, size_t y)
{
    int index = progressionSum(y) + x;
    return array[index];
}

void setTriangleElement(int* array, size_t x, size_t y, int value)
{
    assert(array);
    
    int index = progressionSum(y) + x;
    array[index] = value;
}

int main()
{
    size_t width = 0;
    printf("Triangle array\nEnter array width: ");
    scanf("%d", &width);
    size_t size = triangleSize(width);

    if(!size)
    {
        printf("Executed\n");
        return 0;
    }

    printf("Awaiting %d elements.\n"
           "Enter array:\n", size);

    int* triangleArray = (int*)calloc(size, sizeof(int));
    int value = 0;

    size_t counter = 0;

    for(size_t y = 0; y != width; y++)
    {
        for(size_t x = 0; x <= y; x++)
        {
            assert(++counter <= size);
            scanf("%d", &value);
            setTriangleElement(triangleArray, x, y, value);
        }
    }

    printf("Listing\n\n");
    for(size_t y = 0; y != width; y++)
    {
        for(size_t x = 0; x <= y; x++)
        {
            printf("%d\t", 
                   getTriangleElement(triangleArray, x, y));
        }
        putc('\n', stdout);
    }
    printf("\n\nExecuted\n");
    return 0;
}
