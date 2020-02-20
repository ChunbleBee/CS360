#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#define N 8
#define THREADS 4

bool print_messages = true;

double mat[N][N];
double low[N][N];
double up[N][N];

long p[N];
double b[N];
double y[N];
double x[N];

pthread_barrier_t barrier;

void printMatrix(double matrix[N][N], char ch) {
    printf("Matrix %c:\n", ch);

    for (long i = 0; i < N; i++) {
        printf("\t| ");
        for (long j = 0; j < N; j++) {
            printf("%6.2f  ", matrix[i][j]);
        }
        printf(" |\n");
    }
}

void printDoubleVector(double vector[N], char ch) {
    printf("Vector %c:\n\t| ", ch);

    for (long i = 0; i < N; i++) {
        printf("%6.2f  ", vector[i]);
    }
    printf(" |\n");
}

void printIntVector(long vector[N], char ch) {
    printf("Vector %c:\n\t| ", ch);

    for (long i = 0; i < N; i++) {
        printf("%6lu  ", vector[i]);
    }
    printf(" |\n");
}

void init() {
    for (long i = 0; i < N; i++) {
        for (long j = 0; j < N; j++) {
            mat[i][j] = 1.0;
            up[i][j] = 0.0;
            low[i][j] = (i == j) ? 1.0 : 0.0;
        }

        mat[i][N-1-i] = 1.0*N;
        p[i] = i;
        b[i] = (N)*(N+1)/2 + (N-i)*(N-1);
    }

    pthread_barrier_init(&barrier, NULL, THREADS);
    if (print_messages)
        printf("Initialization complete!\n");
}

void * ludecomp(void * arg) {
    long id = (int) arg;

    if (print_messages)
        printf("in ludecomp on thread: %u\n", id);

    for (long i = 0; i < N; i++) {
        if (i % THREADS == id) {
            double local_maxima = 0.0;
            long pivot_row = i;

            for (long j = i; j < N; j++) {
                if (fabs(mat[j][i]) > local_maxima) {
                    local_maxima = fabs(mat[j][i]);
                    pivot_row = j;
                }
            }

            if (local_maxima == 0.0) {
                printf("Error in LU Decomp - Singular matrix\n");
                return NULL;
            }

            if (pivot_row != i) {
                long swap_index = p[pivot_row];
                p[pivot_row] = p[i];
                p[i] = swap_index;

                for (long j = 0; j < N; j++) {
                    // Swaping row values in init. matrix
                    double temp_val = mat[pivot_row][j];
                    mat[pivot_row][j] = mat[i][j];
                    mat[i][j] = temp_val;

                    //Swapping rows in lower tri. matrix
                    temp_val = low[i][j];
                    low[i][j] = low[pivot_row][j];
                    low[pivot_row][j] = temp_val;
                }
            }
        }
        // Wait for partial pivoting to complete...
        pthread_barrier_wait(&barrier);

        //Calculate LU entries
        up[i][i] = mat[i][i];
        for (long j = i + 1; j < N; j++) {
            if (j%THREADS == id) {
                low[j][i] = mat[j][i]/up[i][i];
                up[i][j] = mat[i][j];
            }
        }

        //Row reductions
        pthread_barrier_wait(&barrier);
        for (long j = i + 1; j < N; j++) {
            if (j%THREADS == id) {
                for (long k = 0; k < N; k++) {
                    mat[j][k] -= low[j][i]*up[i][k];
                }
            }
        }

        pthread_barrier_wait(&barrier);

        if (i%THREADS == id &&
            print_messages == true) {
            printMatrix(mat, 'A');
            printMatrix(low, 'L');
            printMatrix(up, 'U');
        }
    }
}

long main(long argc, char * argv[], char * envp[]) {
    pthread_t threads[THREADS];

    init();

    for (long i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, ludecomp, (void *) i);
    }

    printf("Waiting for threads to join...\n");
    for (long i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printIntVector(p, 'P');

    printf("Joined!\n b vector...\nBefore reorder:\n");
    printDoubleVector(b, 'b');
    // Reorganizing the b vector.
    for (long i = 0; i < N; i++) {
        double temp_val = b[i];
        b[i] = b[p[i]];
        b[p[i]] = temp_val;

        long temp_index = p[p[i]];
        p[p[i]] = p[i];
        p[i] = temp_index;
    }
    printf("Post: \n");
    printDoubleVector(b, 'b');

    printf("Solving for the y vector...\n");
    for (long i = 0; i < N; i++) {
        y[i] = b[i];

        for (long j = 0; j < i; j++) {
            y[i] -= low[i][j]*y[j];
        }
    }
    printDoubleVector(y, 'Y');

    printf("Solving for the x vector...\n");
    for (long i = N-1; i >= 0; i--) {
        double sub = 0.0;

        for (long j = i + 1; j < N; j++) {
            sub += (up[i][j] * x[j]);
        }
        x[i] = (y[i] - sub) / up[i][i];
    }

    printf("Answer:\n");
    printDoubleVector(x, 'X');
}
