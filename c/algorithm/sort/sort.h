#ifndef SORT_HEADER__
#define SORT_HEADER__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/time.h>

#define STDIN  0
#define STDOUT 1

#define SWAP(x, y) do { int z = (x); (x) = (y); (y) = z; } while(0);


typedef int (*sort_t)(int a[], int n);


extern int quick_sort(int a[], int n);
extern int insert_sort(int a[], int n);
extern int merge_sort(int a[], int n);
extern int heap_sort(int a[], int n);
extern int select_sort(int a[], int n);
extern int bubble_sort(int a[], int n);








#endif
