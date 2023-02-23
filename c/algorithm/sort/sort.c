#include "sort.h"


static void parse_array(int fd, int *a[], int *n);
static void print_array(int fd, int a[], int n);


sort_t sort = quick_sort;


int main(int argc, char *argv[]) {
    int *a = NULL;
    int n;
    parse_array(STDIN, &a, &n);


    if (argc >= 2) {
        switch (argv[1][0]) {
            case 'q': sort = quick_sort; break;
            case 'i': sort = insert_sort; break;
            case 'm': sort = merge_sort; break;
            case 'h': sort = heap_sort; break;
            case 's': sort = select_sort; break;
            case 'b': sort = bubble_sort; break;
            default: sort = insert_sort;
        }
    }
    if (sort) sort(a, n);

    print_array(STDOUT, a, n);
    if (a) free(a);
    return 0; 
}

int read_i(int fd, char s[], int n) {
    memset(s, 0, n);
    int rc = 0;
    char *p = s;
    do {
        rc = read(fd, p, 1);
        if (rc == 0) return p - s;
        if (*p == ' ') {
            if (p != s) return p - s;
            continue;
        }
        p++;
        if (p - s >= n) exit(1);
    } while(1);
    return 0;
}

void parse_array(int fd, int *a[], int *n) {
    *n = 0;
    *a = malloc(sizeof(int) * 100);
    if (*a == NULL) return ;

    char num[20];
    int size = 0;
    do {
        size = read_i(fd, num, 20); 
        if (size == 0) break;
        (*a)[*n] = atoi(num);
        (*n)++;
    } while(1);
    return;
}

void print_array(int fd, int a[], int n) {
    char split;
    for (int i = 0; i < n; i++) {
        split = (i % 10 == 9) ? '\n' : ' ';
        dprintf(fd, "%5d%c", a[i], split);
    }
    dprintf(fd, "\ntotal %d\n", n);
}


