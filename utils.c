#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void printIntArray(int *array, int length)
{
    printf("[");
    for (int i = 0; i < length; i++)
        printf("%d%s", array[i], i == length - 1 ? "\0" : ", ");
    printf("]\n");
}

void printCharArray(char *array, int length)
{
    printf("[");
    for (int i = 0; i < length; i++)
        printf("%c%s", array[i], i == length - 1 ? "\0" : ", ");
    printf("]\n");
}

int *readIntListFromArray(FILE *file, int size)
{
    int *result = (int *)malloc(sizeof(int) * size);

    if (result == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < size - 1; i++)
        fscanf(file, "%d, ", result + i);
    fscanf(file, "%d\n", result + size - 1);

    return result;
}

char *readCharListFromArray(FILE *file, int size)
{
    char *result = (char *)malloc(sizeof(char) * size);

    if (result == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < size - 1; i++)
        fscanf(file, "%c, ", result + i);
    fscanf(file, "%c\n", result + size - 1);

    return result;
}

int intArrayIncludes(int *array, int value, int length)
{
    for (int i = 0; i < length; i++)
        if (array[i] == value)
            return i;
    return -1;
}

int charArrayIncludes(char *array, char value, int length)
{
    for (int i = 0; i < length; i++)
        if (array[i] == value)
            return i;
    return -1;
}
