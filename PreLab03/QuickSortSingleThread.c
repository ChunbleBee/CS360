//g -m32 -pthread prog.c
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

void quickSort(int * array, unsigned int length);
void quickSortRecurser(int * array, long lo, long hi);
long partition(int * array, long lo, long hi);
int medianOf3(int lo, int mid, int hi);

void quickSort(int * array, unsigned int length) {
    quickSortRecurser(array, 0, length - 1);
}

void quickSortRecurser(int * array, long lo, long hi) {
    if (lo < hi) {
        long pivotIndex = partition(array, lo, hi);
        if (pivotIndex < 0) pivotIndex = 0;
        quickSortRecurser(array, lo, pivotIndex - 1);
        quickSortRecurser(array, pivotIndex + 1, hi);
    }
}

long partition(int * array, long lo, long hi) {
    int pivot = medianOf3(array[lo], array[(lo + hi) / ((long) 2)], array[hi]);
    lo--;
    hi++;
    while (true) {
        //printf("pivot: %d, lo: %ld, hi: %ld\n", pivot,  lo, hi);
        do { lo++; } while (array[lo] < pivot);
        do { hi--; } while (array[hi] > pivot);
        if (lo < hi) {
            int temp = array[lo];
            array[lo] = array[hi];
            array[hi] = temp;
        } else {
            return hi;
        }
    }
}

int medianOf3(int lo, int mid, int hi) {
    if (lo > hi) {
        int temp = lo;
        lo = hi;
        hi = temp;
    }
    if (lo > mid) {
        mid = lo;
    }
    if (mid > hi) {
        mid = hi;
    }
    return mid;
}