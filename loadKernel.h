#ifndef LOAD_KERNEL_H
#define LOAD_KERNEL_H

#include <stdio.h>
int** loadKernel(const char* filename, int* rows, int* cols);
void freeMatrix(int** matrix, int rows);
#endif
