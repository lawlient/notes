#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>



static char *subs(char *src);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s string\n", argv[0]);
        return 0;
    }

    int n = strlen(argv[1]);
    char *src = malloc(n + 1);
    assert(src);
    strncpy(src, argv[1], n);
    src[n] = 0;

    src = subs(src);
    
    if (src) {
        printf("%s\n", src);
        free(src);
    }
    return 0;
}
 
char *subs(char *src) {
    int nspace = 0;
    char *p;

    for (p = src; *p; p++) {
        if (*p == ' ') {
            nspace++;
        }
    }

    int more_space = 2 * nspace;
    int ol = strlen(src);
    int nl = ol + more_space;
    char *tmp = realloc(src, nl + 1);
    if (tmp == NULL) return src;
    src = tmp;

    for (int i = ol, j = nl; i >= 0 && j >= 0; ) {
        if (src[i] == ' ') {
            i--;
            src[j--] = '0';
            src[j--] = '2';
            src[j--] = '%';
        } else {
            src[j--] = src[i--];
        }
    }

    return src;
}
