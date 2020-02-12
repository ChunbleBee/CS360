
/****** C4.2.c: quicksort by threads *****/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "QuickSortSingleThread.c"

typedef struct{
  int upperbound;
  int lowerbound;
} PARM;  

#define N 10        

int a[N] = {5,1,6,4,7,2,9,8,0,3};  // unsorted data
int b[N] = {5,1,6,4,7,2,9,8,0,3};

int print()   // print current a[] contents
{
  int i;
  printf("[ ");
  for (i=0; i<N; i++)
    printf("%d ", a[i]);
  printf("]\n");
}

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


    while (left < right){         // partition loop
       do { left++;} while (a[left] < pivot);

       do { right--;} while (a[right] > pivot);

       if (left < right ){          
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
    PARM arg;
    int i, *array;
    pthread_t me, thread;

    struct timeval t1, t2, t3, t4;
    gettimeofday(&t1, NULL);

    me = pthread_self();
    //printf("main %lu: unsorted array = ", me);
    //print();

    arg.upperbound = N-1;
    arg.lowerbound = 0;

    //printf("main %lu create a thread to do QS\n", me);
    pthread_create(&thread, NULL, Qsort, (void *)&arg);

    // wait for QS thread to finish
    pthread_join(thread, NULL);
    //printf("main %lu sorted array = ", me);
    //print();
    
    gettimeofday(&t2, NULL);

    printf("Speed of multi-threaded quicksort: \n");
    printf("End Time: sec=%ld usec=%ld\n", t2.tv_sec, t2.tv_usec);
    printf("Start Time: sec=%ld usec=%ld\n", t1.tv_sec, t1.tv_usec);
    printf("Micro-second execution time: %ld\n", t2.tv_usec - t1.tv_usec);

    gettimeofday(&t3, NULL);
    quickSort(b, N);
    gettimeofday(&t4, NULL);

    printf("Speed of single-threaded quick sort: \n");
    printf("End Time: sec=%ld usec=%ld\n", t4.tv_sec, t4.tv_usec);
    printf("Start Time: sec=%ld usec=%ld\n", t3.tv_sec, t3.tv_usec);
    printf("Micro-second execution time: %ld\n", t4.tv_usec - t3.tv_usec);
}

