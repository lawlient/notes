#include "sort.h"


static void parse_array(int fd, int *a[], int *n);
static void print_array(int fd, int a[], int n);
static void sort_array(int a[], int n);
static long nowms();


sort_t sort = quick_sort;
long elapse = 0;


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
    sort_array(a, n);

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
        if (*p == ' ' || *p == '\n') {
            if (p != s) return p - s;
            continue;
        }
        p++;
        if (p - s >= n) exit(1);
    } while(1);
    return 0;
}

void parse_array(int fd, int *a[], int *n) {
    char num[20];
    int size;
    size = read_i(fd, num, 20);
    if (size == 0) return;

    *n = atoi(num);
    *a = malloc(sizeof(int) * (*n));
    if (*a == NULL) return ;

    for (int i = 0; i < *n; i++) {
        size = read_i(fd, num, 20); 
        if (size == 0) break;
        (*a)[i] = atoi(num);
    }
    return;
}

void print_array(int fd, int a[], int n) {
    char split;
    int i = (n > 30 ? n - 30 : 0);
    for (; i < n; i++) {
        split = (i % 10 == 9) ? '\n' : ' ';
        dprintf(fd, "%7d%c", a[i], split);
    }
    dprintf(fd, "\ntotal %d\telapse %ldms\n", n, elapse);
}

void sort_array(int a[], int n) {
    long s, e;
    s = nowms();
    sort(a, n);
    e = nowms();
    elapse = e > s ? e - s : 0;    
}

long nowms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
