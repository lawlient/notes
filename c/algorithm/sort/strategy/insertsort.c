#include "sort.h"

static int inner_sort(int a[], int i);


int insert_sort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        inner_sort(a, i);
    }
    return 0;
}


int inner_sort(int a[], int i) {
    int n = a[i];
    int j;
    for (j = i; j; j--) {
        if (a[j-1] <= n) break;
        a[j] = a[j-1];
    }
    a[j] = n;
    return 0;
}
