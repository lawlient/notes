#include "sort.h"


#define L(p) (2 * (p) + 1)
#define R(p) (2 * (p) + 2)

static int aux(int a[], int n, int h);
static int buildheap(int a[], int n);


int heap_sort(int a[], int n) {
    buildheap(a, n);

    while (--n) {
        SWAP(a[n], a[0]);
        aux(a, n, 0);
    }

    return 0;
}

/* 假设a满足大顶堆，h为需要调整的节点
 * 迭代向下转移hole，直至叶子节点
 */
int aux(int a[], int n, int h) {
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

/* 从最后一个非叶子节点向下调整，以此调整至第一个节点（堆顶）
 * 即完成堆的构建
 */
int buildheap(int a[], int n) {
    int h;
    for (h = n / 2; h >= 0; h--) {
        aux(a, n, h);
    }
    return 0;
}
