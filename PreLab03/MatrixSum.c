#include <stdint.h>
long matrixSum(int ** matrix, uint32_t columns, uint32_t rows) {
    long sum = 0;
    for (uint32_t i = 0; i < columns; i++) {
        for (uint32_t j = 0; j < rows; j++) {
            sum += matrix[i][j];
        }
    }

    return sum;
}