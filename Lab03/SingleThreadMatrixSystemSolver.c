#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Ax = b
// Decompose A such that...
// A = P'LU
//      where L is the lower triangle
//      and U is the upper triangle
//      Solve, such that,
//          PA = LU
// LUx = Pb
//      Assume P is the identity matrix, s.t.
//      LUx = Ib
//      thus
//      LUx = b
// Solve Ly = b for y
//      y = L'b
// Solve Ux = y for x
//      x = U'L'b

typedef struct {
    double ** upper;
    double ** lower;
} LUDecomposition;

void printRow(double * row, uint32_t size) {
    printf("\t[");
    for (uint32_t i = 0; i < size; i++) {
        printf("%10.2lf", row[i]);
    }
    printf("]\n");
}

void printSquareMatrix(double ** matrix, uint32_t rows, uint32_t cols) {
    for (uint32_t i = 0; i < rows; i++) {
        printRow(matrix[i], cols);
    }
}

double * matrixSystemSolver(double ** A, double * b, uint32_t length) {
    return NULL;
}

LUDecomposition * luDecomp(double ** matrix, uint32_t size) {
    LUDecomposition * output = (LUDecomposition *)malloc(sizeof(LUDecomposition));

    //lower and upper matrices setup.
    double ** upper = calloc(size, sizeof(double *));
    double ** lower = calloc(size, sizeof(double *));

    for (uint32_t i = 0; i < size; i++) {
        upper[i] = calloc(size, sizeof(double));
        lower[i] = calloc(size, sizeof(double));
        //calloc() zeros out the row, so no need to manually do so.
        lower[i][i] = 1.0;
        memcpy(upper[i], matrix[i], sizeof(double) * size);
    }

    output->lower = lower;
    output->upper = upper;

    for (uint32_t i = 0; i < size - 1; i++) {
        printf("pivot: ");
        printRow(upper[i], size);

        for (uint32_t j = i + 1; j < size; j++) {
            double iVal = upper[i][i];
            if (iVal == 0) {
                printf("Failure to decompose!");
                return NULL;
            }
            double jVal = upper[j][i];
            double kVal = jVal/iVal;

            for (uint32_t k = i; k < size; k++) {
                upper[j][k] -= kVal * upper[i][k];
            }
            lower[j][i] = kVal;
            printf("upper: ");
            printRow(upper[j], size);
            printf("lower: ");
            printRow(lower[j], size);
            printf("\n");
        }
    }

    return output;
}

int main(int argc, char argv[], char envp[]) {
    uint32_t size = 4;
    double ** mat = calloc(size, sizeof(double *));
    double next = 1.0;

    for (uint32_t i = 0; i < size; i++) {
        mat[i] = calloc(3, sizeof(double));
        for (uint32_t j = 0; j < size; j++) {
            mat[i][j] = next;
            next += 3.0;
        }
        next -= 1.0;
    }
    mat[size - 2][size - 2] = 13.0;
    printSquareMatrix(mat, size, size);
    printf("\n");

    LUDecomposition * lu = luDecomp(mat, size);
    if (lu != NULL) {
        printf("Lower:\n");
        printSquareMatrix(lu->lower, size, size);
        printf("Upper:\n");
        printSquareMatrix(lu->upper, size, size);
    }
}