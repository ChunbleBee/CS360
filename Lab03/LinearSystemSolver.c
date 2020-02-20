#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>

#define N 8
#define THREADS 4
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
    long myID = (long) arg;

    printf("in GE, thread: %u\n", myID);

    for (long i = 0; i < N-1; i++) {
        printf("GE, i = %u, thread = %u\n", i, myID);
        if (i%THREADS == myID) {
            printf("Thread #%u Partial Pivoting Row #%u\n", myID, i);
            long pivot_row = i;
            double localMaxima = 0.0;

            //Getting pivot row
            for (long j = i; j <= N; j++) {
                if (fabs(A[j][i]) > localMaxima) {
                    localMaxima = fabs(A[j][i]); //FLOATING ABSOLUTE
                    pivot_row = j;
                }
            }

            printf("Pivot Row: %u, Pivot Value: %0.2f\n", pivot_row, A[pivot_row][i]);
            if (pivot_row != i) {
                for (long j = i; j < N + 1; j++) {
                    double temp = A[i][j];
                    A[i][j] = A[pivot_row][j];
                    A[pivot_row][j] = temp;
                }
            }
            printf("Pivoted local maxima to local top of the matrix, row: %d\n", i);

            // Waiting for all partial pivoting to be done.
            pthread_barrier_wait(&barrier);

            // Factorize the rows
            for (long j = i + 1; j < N; j++) {
                if (j%THREADS == myID) {
                    double factor = A[j][i]/A[i][i];
                    printf("Thread %u to factorize row %u by factor: %lf.\n", myID, j, factor);
                    for (long k = i; k <= N; k++) {
                        A[j][k] -= factor * A[i][k];
                    }
                    A[j][i] = 0.0;
                }
            }
        }

        printf("BARRIER, THREAD #%u\n", myID);
        pthread_barrier_wait(&barrier);
        if (i%THREADS == myID) {
            printMatrix();
        }
        printf("PAST BARRIER, THREAD #%u\n", myID);
    }
}

int main(int argc, char * argv[]) {
    pthread_t threads[THREADS];

    printf("Initialization of matrix...\n");
    for (long i=0; i<N; i++) {
        for (long j=0; j<N; j++) {
            A[i][j] = 1.0;
        }
    }
    for (long i=0; i<N; i++) {
        A[i][N-i-1] = 1.0*N;
    }
    for (long i=0; i<N; i++) {
        A[i][N] = 2 * N - 1;
    }
    printMatrix();

    pthread_barrier_init(&barrier, NULL, THREADS);

    printf("Main, creating %u extra threads...\n", THREADS);
    for (long i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, gaussianElimination, (void *) i);
    }
    printf("Main, waiting for all working threads to join...\n");

    for (long i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main, Back substitution/propogation...\n");

    for (long i = N - 1; i >= 0; i--) {
        double sum = 0.0;

        for (long j = i + 1; j < N; j++) {
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