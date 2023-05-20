#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printIntArray(int *array, int length);
void printCharArray(char *array, int length);

int *readIntListFromArray(FILE *file, int size);
char *readCharListFromArray(FILE *file, int size);

int intArrayIncludes(int *array, int value, int length);
int charArrayIncludes(char *array, char value, int length);

#endif