#include "sort.h"



int select_sort(int a[], int n) {
    int m, i, j;
    for (j = n; j; j--) {
        m = 0;
        for (i = 1; i < j; i++) {
            if (a[i] > a[m]) {
                m = i;
            }
        }
        int tmp = a[j-1];
        a[j-1] = a[m];
        a[m] = tmp;
    }
    return 0;
}
