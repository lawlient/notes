#include "hashtable.h"
#include <stdint.h>
#include <malloc.h>
#include <string.h>

static uint32_t BSIZE[2] = { 13, 29};
static uint8_t BSIZE_INDEX = 0;

#define OK 0
#define ERR 1

// ----------------- private --------------------------
static int __initDict(Dict d, HT_TYPE ty);
static void __dictReset(Dict d);




static inline void *__malloc(uint32_t s) { return malloc(s); }

static HashValue __hash(Key k);
static bool __equal(Key k1, Key k2, size_t s);
static void __insert(HashTable ht, Key k, Value v);
static bool __remove(HashTable ht, Key k);
static Value __get(HashTable ht, Key k);
static void __clear(HashTable ht);

ht_type default_ht_type = {
  __hash,
  __equal,
  NULL,
  NULL,
  NULL,
  NULL,
};

typedef struct node {
  Key k;
  Value v;
  struct node *next;
} Node;

struct hash_table {
  Entry* bucket;
  uint32_t size;
  uint32_t used;
};

static int __initDict(Dict d, HT_TYPE ty) {
  __dictReset(d);
  d->type = ty;
  return OK;
}

static void __dictReset(Dict d) {
  d->ht->bucket = NULL;
  d->ht->size = d->ht->used = 0;
}

Dict creat_hash_table(HT_TYPE ty) {
  Dict d = (Dict)__malloc(sizeof(dict));
  __initDict(d, ty);
  return d;
}

void destroy_hash_table(Dict d) {
  if (!ht) return;
  if (ht->bucket) free(ht->bucket);
  free(ht);
}

HashValue __hash(Key k) {
  HashValue hv = 0;
  return hv;
}

bool __equal(Key k1, Key k2, size_t s) {
  return memcmp(k1, k2, s);
}

void __insert(HashTable ht, Key k, Value v) {
  // TODO: resize
  HashValue hv = ht->type->hash(k);
  if (ht->bucket[hv].k == NULL) {
    ht->bucket[hv].k = k;
    ht->bucket[hv].v = v;
    return;
  }

  Node *p = &ht->bucket[hv];
  while (p) {
    if (ht->type->equal(k, p->k)) {

      return;
    }
  }
}


