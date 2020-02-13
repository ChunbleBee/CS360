
/****** C4.2.c: quicksort by threads *****/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "QuickSortSingleThread.c"

typedef struct {
  int upperbound;
  int lowerbound;
} PARM;

#define N 5100000

int a[N];  // unsorted data
int b[N];

void *Qsort(void *aptr)
{
    PARM *ap, aleft, aright;
    int pivot, pivotIndex, left, right, temp;
    int upperbound, lowerbound;

    pthread_t me, leftThread, rightThread;
    me = pthread_self();
    ap = (PARM *)aptr;

    upperbound = ap->upperbound;
    lowerbound = ap->lowerbound;

    pivot = a[upperbound];         // pick low pivot value

    left = lowerbound - 1;         // scan index from left side
    right = upperbound;            // scan index from right side

    if (lowerbound >= upperbound)
       pthread_exit(NULL);


    while (left < right) {         // partition loop
       do { left++;} while (a[left] < pivot);

       do { right--;} while (a[right] > pivot);

       if (left < right ) {
          temp = a[left];
          a[left] = a[right];
          a[right] = temp;
       }
    }
    //print();

    pivotIndex = left;           // put pivot back
    temp = a[pivotIndex];
    a[pivotIndex] = pivot;
    a[upperbound] = temp;

    // start the "recursive threads"
    aleft.upperbound = pivotIndex - 1;
    aleft.lowerbound = lowerbound;
    aright.upperbound = upperbound;
    aright.lowerbound = pivotIndex + 1;

    //printf("%lu: create left and right threads\n", me);
    pthread_create(&leftThread,  NULL, Qsort, (void *)&aleft);
    pthread_create(&rightThread, NULL, Qsort, (void *)&aright); 

    // wait for left and right threads to finish
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);
    //printf("%lu: joined with left & right threads\n", me);
}

int main(int argc, char *argv[])
{
    printf("Initializing...\n");
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
      int temp = rand() % 1000;
      a[i] = temp;
      b[i] = temp;
    }
    PARM arg;
    int i, *array;
    pthread_t me, thread;

    clock_t begin, end;
    printf("Init. Complete - Running Sorts on %d inputs.\n\n", N);

    // start threaded quicksort
    begin = clock();
    me = pthread_self();
    arg.upperbound = N-1;
    arg.lowerbound = 0;
    pthread_create(&thread, NULL, Qsort, (void *)&arg);
    pthread_join(thread, NULL);
    end = clock();

    printf("Speed of multi-threaded quicksort: \n");
    printf("\tStart Time: %lu\n", begin);
    printf("\tEnd Time: %lu\n", end);
    printf("\tExecution time: %lu\n\n", (end - begin));

    begin = clock();
    quickSort(b, N);
    end = clock();

    printf("Speed of single-threaded quicksort: \n");
    printf("\tStart Time: %lu\n", begin);
    printf("\tEnd Time: %lu\n", end);
    printf("\tExecution time: %lu\n", (end - begin));

    printf("\nPress enter to exit...");
    getchar();
}