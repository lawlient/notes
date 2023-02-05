#ifndef HASHTABLE_HEADER__
#define HASHTABLE_HEADER__

typedef void* K;
typedef void* V;

typedef int (*Hash)(K);
typedef K (*KeyDup)(K);
typedef V (*ValDup)(V);
typedef int (*KeyCmp)(K, K);
typedef void (*KeyDel)(K);
typedef void (*ValDel)(V);

typedef struct HashTableFunc_ {
    Hash hash;
    KeyDup kdup;
    ValDup vdup;
    KeyCmp kcmp;
    KeyDel kdel;
    ValDel vdel;
} HashTableFunc;


#define HTHash(ht, key) ht->functions->hash(key) % ht->bucketsize;
#define HTKeyDup(ht, key) (ht->functions->kdup ? ht->functions->kdup(key) : key)
#define HTValDup(ht, val) (ht->functions->vdup ? ht->functions->vdup(val) : val)
#define HTDelKey(ht, key) if (ht->functions->kdel) ht->functions->kdel(key);
#define HTDelVal(ht, val) if (ht->functions->vdel) ht->functions->vdel(val);
#define HTCmpKey(ht, k1, k2) (ht->functions->kcmp ? ht->functions->kcmp(k1, k2) : (k1 == k2))


typedef struct HTNode_ {
    K k;
    V v;
    struct HTNode_ *next;
} HTNode;


typedef struct HashTable_ {
    HashTableFunc *functions;
    HTNode **buckets;
    int bucketsize;
    int size;
} HashTable;



HashTable *HashTable_new(HashTableFunc *func);
void HashTable_delete(HashTable* ht);


HTNode* HashTable_find(HashTable *ht, K key);
int HashTable_insert(HashTable *ht, K k, V v);
int HashTable_remove(HashTable *ht, K k);
int HashTable_size(HashTable *ht);
int HashTable_empty(HashTable *ht);


#define HTApplyFn(ht, fn)                               \
    for (int i =  0; i < ht->bucketsize; i++) {         \
        HTNode *n = ht->buckets[i];                     \
        while (n) {                                     \
            (*fn)(n->k, n->v); n = n->next;             \
        }                                               \
    }




#endif
