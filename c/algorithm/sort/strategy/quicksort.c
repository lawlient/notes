#include "sort.h"


static int partition(int a[], int n, int p);
static int inner_sort(int a[], int s, int e);


int quick_sort(int a[], int n) { return inner_sort(a, 0, n-1); }

int inner_sort(int a[], int s, int e) {
    if (s >= e) return 0;

    int pi = partition(a, s, e); 
    inner_sort(a, s, pi - 1);
    inner_sort(a, pi + 1, e);
    return 0;
}

int partition(int a[], int s, int e) {
    if (s >= e) return s;
    int i = s, j = e;

    int p = a[s];

    while (i < j) {
        while (i < j && a[j] >= p) j--;
        a[i] = a[j];
        while (i < j && a[i] <= p) i++;
        a[j] = a[i];
    }
    a[i] = p;
    return i;
}
