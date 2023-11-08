#ifndef LOAD_KERNEL_H
#define LOAD_KERNEL_H

#include <stdio.h>

// Function to allocate a matrix from a file and return a pointer to it
int** loadKernel(const char* filename, int* rows, int* cols);
void freeMatrix(int** matrix, int rows);
#endif
