#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>

#define N 4
double A[N][N+1];
double solution[N+1];
pthread_barrier_t barrier;

int printMatrix() {
    printf("Matrix:\n");
    for (int i = 0; i < N; i++) {
        printf("\t| ");
        for (int j = 0; j < N + 1; j++) {
            printf("%6.2f", A[i][j]);
        }
        printf(" |\n");
    }
}

void * gaussianElimination(void * arg) {
    uint32_t pivot_row = 0;
    uint32_t myID = (uint32_t) arg;

    for (uint32_t i = 0; i < N-1; i++) {
        if (i == myID) {
            printf("Thread #%u Partial Pivoting Row #%u\n", myID, i);
            pivot_row = i;
            double localMaxima = 0.0;

            //Getting pivot row
            for (uint32_t j = i; j <= N; j++) {
                if (fabs(A[j][i]) > localMaxima) {
                    localMaxima = fabs(A[j][i]); //FLOATING ABSOLUTE
                    pivot_row = j;
                }
            }

            printf("Pivot Row: %u - Pivot Value: %0.2f\n", pivot_row, A[pivot_row][i]);
            if (pivot_row != i) {
                for (uint32_t j = i + 1; j < N + 1; j++) {
                    double temp = A[i][j];
                    A[i][j] = A[pivot_row][j];
                    A[pivot_row][j] = temp;
                }
            }
            printf("Pivoted local maxima to local top of the matrix, row: %d\n", i);

            // Create barrier, find the correct row to factorize;
            pthread_barrier_wait(&barrier);
            for (int j = i + 1; j < N; j++) {
                if (j == myID) {
                    double factor = A[j][i]/A[i][i];
                    printf("Thread %u to factorize row %u by factor: %lf.\n", myID, j, factor);
                    for (uint32_t k = i; k <= N; k++) {
                        A[j][k] -= factor * A[i][k];
                    }
                }
            }
        }
        printf("BARRIER\n");
        pthread_barrier_wait(&barrier);
        if (i == myID) {
            printMatrix();
        }
    }
}

int main(int argc, char * argv[]) {
    pthread_t threads[N];

    printf("Initialization of matrix...\n");
    for (uint32_t i=0; i<N; i++) {
        for (uint32_t j=0; j<N; j++) {
            A[i][j] = 1.0;
        }
    }
    for (uint32_t i=0; i<N; i++) {
        A[i][N-i-1] = 1.0*N;
    }
    for (uint32_t i=0; i<N; i++) {
        A[i][N] = 2 * N - 1;
    }
    printMatrix();

    pthread_barrier_init(&barrier, NULL, N);
    printf("Main, creating %u extrea threads...", N);
    for (uint32_t i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, gaussianElimination, (void *) i);
    }
    printf("Main, waiting for all working threads to join...\n");

    for (uint32_t i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main, Back substitution/propogation...\n");

    for (uint32_t i = N - 1; i >= 0; i--) {
        double sum = 0.0;

        for (uint32_t j = i + 1; j < N; j++) {
            sum += A[i][j] * A[j][N];
            solution[i] = (A[i][N] - sum)/ A[i][i];
        }

    }

    printf("Solution:\n\t| ");
    for (int i = 0; i < N; i++) {
        printf("%6.2lf", solution[i]);
    }
    printf(" |");
}