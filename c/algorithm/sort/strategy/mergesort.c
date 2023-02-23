#include "sort.h"

int inner_sort(int a[], int s, int e) {
    if (e - s <= 1) return 0;
    if (e - s == 2) {
        if (a[s] > a[e-1]) {
            int tmp = a[s];
            a[s] = a[e-1];
            a[e-1] = tmp;
        }
        return 0;
    }
    int m = (e - s) / 2 + s;
    inner_sort(a, s, m);
    inner_sort(a, m, e);

    int *tmp = malloc(sizeof(int) * (e - s));
    assert(tmp);
    int i = s, j = m, k = 0;
    for (; i < m && j < e; ) {
        tmp[k++] = (a[i] < a[j] ? a[i++] : a[j++]);
    }
    for (; i < m;) tmp[k++] = a[i++];
    for (; j < e;) tmp[k++] = a[j++];
    assert(k == (e-s));
    memcpy(a + s, tmp, k * sizeof(int));
    free(tmp);
    return 0;
}

int merge_sort(int a[], int n) {
    return inner_sort(a, 0, n);
}
