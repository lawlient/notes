#include "hash_table.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>


#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) < (y) ? (y) : (x))

static int isprime(int i);
static int factor(int hint);

static int inner_hash(K k);
static void free_node(HashTable *ht, HTNode *);
static HTNode* create_node();

static HashTableFunc kDefaultFuncSet = {
    inner_hash,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

HashTable *HashTable_new(HashTableFunc *func) {
    HashTable *ht = malloc(sizeof(HashTable));
    if (ht == NULL) return NULL;
    ht->bucketsize = factor(0);
    ht->size = 0;
    ht->buckets = malloc(sizeof(HTNode*) * ht->bucketsize);
    if (ht->buckets == NULL) {
        free(ht);
        return NULL;
    }
    memset(ht->buckets, 0, sizeof(HTNode*) * ht->bucketsize); 
    ht->functions = func ? func : &kDefaultFuncSet;
    return ht;
}

void HashTable_delete(HashTable* ht) {
    HTNode *node, *p;
    for (int i = 0; i < ht->bucketsize; i++) {
        for (node = ht->buckets[i]; node; ) {
            p = node;
            node = p->next;
            free_node(ht, p);
        }
    }

    free(ht->buckets);
    free(ht);
}

HTNode* HashTable_find(HashTable *ht, K k) {
    int hash = HTHash(ht, k);
    HTNode* n = ht->buckets[hash];
    for (; n; n = n->next) {
        if (!HTCmpKey(ht, k, n->k))
            return n;
    }
    return NULL;
}

int HashTable_insert(HashTable *ht, K k, V v) {
    int hash = HTHash(ht, k);
    HTNode **n = &ht->buckets[hash];
    HTNode *nnode = create_node();
    if (nnode == NULL) return 1;

    nnode->k = HTKeyDup(ht, k);
    nnode->v = HTValDup(ht, v);
    nnode->next = *n ? (*n)->next : NULL;
    *n = nnode;
    ht->size++;
    return 0;
}

int HashTable_remove(HashTable *ht, K k) {
    int hash = HTHash(ht, k);
    HTNode **p = &ht->buckets[hash];
    HTNode *n = (*p)->next;
    while (n) {
        if (!HTCmpKey(ht, k, n->k)) {
            (*p)->next = n->next;
            free_node(ht, n);
            ht->size--;
            return 0;
        }
        *p = &(*n);
        n = (*p)->next;
    };
    return 0;
}

int HashTable_size(HashTable *ht) { return ht->size; }
int HashTable_empty(HashTable *ht) { return 0 == ht->size; }


int isprime(int n) {
    for (int i = 2; i < n; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int factor(int hint) {
    int prime = MAX(hint, 131);
    for (; ; prime++) {
        if (isprime(prime))
            return prime;
    }
    return 0;
}

int inner_hash(K k) {
    char *key = k;
    return key[0];
}

void free_node(HashTable *ht, HTNode *n) { HTDelKey(ht, n->k); HTDelVal(ht, n->v); free(n); }
HTNode* create_node() { return malloc(sizeof(HTNode)); }
