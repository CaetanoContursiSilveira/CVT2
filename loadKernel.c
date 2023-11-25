#include <stdio.h>
#include <stdlib.h>
#include "loadKernel.h"

int** loadKernel(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Falha ao ler arquivo: %s\n", filename);
        return NULL;
    }
    *rows = 0;
    *cols = 0;
    int tempCols = 0;
    int tempChar;
    while ((tempChar = fgetc(file)) != EOF) {
        if (tempChar >= '0' && tempChar <= '9') {
            tempCols++;
        }
        if (tempChar == '\n') {
            (*rows)++;
            if (tempCols > *cols) {
                *cols = tempCols;
            }
            tempCols = 0;
        }
    }

    fseek(file, 0, SEEK_SET);

    int** matrix = (int**)malloc((*rows) * sizeof(int*));
    for (int i = 0; i < *rows; i++) {
        matrix[i] = (int*)malloc((*cols) * sizeof(int));
    }
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (fscanf(file, "%d", &matrix[i][j]) == EOF) {
                fprintf(stderr, "Falha ao ler dados\n");
                fclose(file);
                freeMatrix(matrix, *rows);
                return NULL;
            }
        }
    }

    fclose(file);
    return matrix;
}

void freeMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
