#include "hash_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int intHash(void* n) { return *(int *)n; }
static void *intDup(void *n) {
    int *nn = malloc(sizeof(int));
    memcpy(nn, n, sizeof(*nn));
    return nn;
}
static int intCmp(void *k1, void *k2) {
    return *(int *)k1 == *(int *)k2 ? 0 : 1;
}
static void intFree(void *n) { free(n); }

static HashTableFunc int_int_hashtable = {
    intHash,
    intDup,
    intDup,
    intCmp,
    intFree,
    intFree,
};




static void *PrintPair(void *k, void *v) {
    printf("[%d] = %d\n", *(int *)k, *(int *)v);
    return NULL;
}


int main() {
    HashTable *ht = HashTable_new(&int_int_hashtable);
    if (ht == NULL) {
        printf("create hash table fail\n");
        return 1;
    }

    for (int i = 0; i < 1000; i++) {
        int v = i + 1;
        HashTable_insert(ht, (int *)&i, &v);
    }

    HTApplyFn(ht, &PrintPair);
    return 0;
}
