#include "sort.h"


#define L(p) (2 * (p) + 1)
#define R(p) (2 * (p) + 2)

static int down(int a[], int n, int h);


int heap_sort(int a[], int n) {
    for (int h = n / 2; h >= 0; h--) {
        down(a, n, h);
    }

    while (--n) {
        SWAP(a[n], a[0]);
        down(a, n, 0);
    }

    return 0;
}

/* 假设a满足大顶堆，h为需要调整的节点
 * 迭代向下转移hole，直至叶子节点
 */
int down(int a[], int n, int h) {
    int l = L(h), r = R(h);
    while (l < n && r < n) {
        int m = a[l] < a[r] ? r : l;
        if (a[h] >= a[m]) break;

        SWAP(a[h], a[m]);
        h = m;
        l = L(h);
        r = R(h);
    }
    if (l < n && a[l] > a[h]) 
        SWAP(a[l], a[h]);

    return 0;
}

