//g -m32 -pthread prog.c
void quickSort(int * array, int length) {
    quickSortRecurser(array, 0, length-1);
}

void quickSortRecurser(int * array, int lo, int hi) {
    if (lo < hi) {
        int pivotIndex = partition(array, lo, hi);
        quickSortRecurser(array, lo, pivotIndex - 1);
        quickSortRecurser(array, pivotIndex + 1, hi);
    }
}

int partition(int * array, int lo, int hi) {
    int mid = (hi + lo) / 2;
    int pivot = array[mid];

    while (lo < hi) {
        while (array[lo] <= pivot) {
            lo++;
        }
        while (array[hi] >= pivot) {
            hi--;
        }
        if (array[lo] < array[hi]) {
            int temp = array[lo];
            array[lo] = array[hi];
            array[hi] = temp;
        }
    }
    return hi;
}