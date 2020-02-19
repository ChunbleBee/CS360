#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

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

    //Setup
    double ** upper = calloc(size, sizeof(double *));
    double ** lower = calloc(size, sizeof(double *));
    double * y      = calloc(size, sizeof(double));
    uint32_t * perm = calloc(size, sizeof(uint32_t));

    for (uint32_t i = 0; i < size; i++) {
        upper[i] = calloc(size, sizeof(double));
        lower[i] = calloc(size, sizeof(double));
        //calloc() zeros out the row, so no need to manually do so.
        lower[i][i] = 1.0;
        perm[i] = i;
    }

    output->lower = lower;
    output->upper = upper;

    for (uint32_t i = 0; i < size - 1; i++) {
        printf("pivot: ");
        printRow(upper[i], size);
        double iVal = upper[i][i];

        for (uint32_t j = i; j < size; j++) {
            if (upper[j][i] > iVal) {
                iVal = upper[j][i];

                double * temp = upper[i];
                upper[i] = upper[j];
                upper[j] = temp;

                temp = lower[i];
                lower[i] = lower[j];
                lower[j] = temp;
            }
        }

        for (uint32_t j = i + 1; j < size; j++) {
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

    for (uint32_t i = 0; i < size; i++) {
        if (perm[i] != i) {
            uint32_t j = perm[i];
            perm[j] = perm[i];
            perm[i] = j;

            double * temp = upper[i];
            upper[i] = upper[j];
            upper[j] = temp;

            temp = lower[i];
            lower[i] = lower[j];
            lower[j] = temp;
            i--;
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