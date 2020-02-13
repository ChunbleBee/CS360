/** C4.3.c: matrix sum by threads with mutex lock **/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "MatrixSum.c"

#define  N   22000
int A[N][N];

long int total = 0;           // global total
pthread_mutex_t *m;           // mutex pointer

void *func(void *arg)         // working thread function
{
   int i, row, sum = 0;
   pthread_t tid = pthread_self(); // get thread ID number
   row = (int)arg;                 // get row number from arg
   for (i=0; i<N; i++)       // compute partial sum of A[row]in 
       sum += A[row][i];
   pthread_mutex_lock(m);
     total += sum;           // update global total inside a CR
   pthread_mutex_unlock(m);
}

int main (int argc, char *argv[])
{
   pthread_t thread[N];
   int i, j, r;
   void *status;

   clock_t begin, end;

   printf("Main: initialize A matrix\n");
   for (i=0; i<N; i++){
     for (j=0; j<N; j++){
         A[i][j] = i*N + j + 1;
     }
   }
   printf("Initialization complete - starting matrix sum on %lu inputs.\n\n", N);
   begin = clock();
   // create a mutex m
   m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
   pthread_mutex_init(m, NULL); // initialize mutex m 
   for(i=0; i<N; i++) {
      pthread_create(&thread[i], NULL, func, (void *)i); 
   }
   for(i=0; i<N; i++) {
      pthread_join(thread[i], &status);
   }
   pthread_mutex_destroy(m); // destroy mutex m
   end = clock();

    printf("Speed of multi-threaded matrix summation: \n");
    printf("\tStart Time: %lu\n", begin);
    printf("\tEnd Time: %lu\n", end);
    printf("\tExecution time: %lu\n\n", (end - begin));

    begin = clock();
    matrixSum(A, N, N);
    end = clock();

    printf("Speed of single-threaded matrix summation: \n");
    printf("\tStart Time: %lu\n", begin);
    printf("\tEnd Time: %lu\n", end);
    printf("\tExecution time: %lu\n", (end - begin));

    printf("\nPress enter to exit...");
    getchar();

   pthread_exit(NULL);
}

