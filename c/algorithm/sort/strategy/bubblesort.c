#include "sort.h"


int bubble_sort(int a[], int n) {

    for (int j = n; j; j--) {
        for (int i = 0; i < j; i++) {
            if (a[i] > a[i+1]) {
                SWAP(a[i], a[i+1]);
            }
        }
    }
    return 0;
}
