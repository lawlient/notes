#ifndef HASH_TABLE_HEADER__
#define HASH_TABLE_HEADER__

#include <stdlib.h>

typedef struct hash_table *HashTable;
typedef void * Key;
typedef void * Value;
typedef int HashValue;

typedef struct node *Entry;

typedef HashValue (*Hash)(Key k);
typedef bool (*Equal)(Key k1, Key k2);
typedef Key (*DupKey)(Key k);
typedef Value (*DupValue)(Value k);
typedef void (*DestroyKey)(Key k);
typedef void (*DestroyValue)(Value v);

typedef void (*Insert)(HashTable, Key, Value);
typedef Value (*Get)(HashTable, Key);
typedef bool (*Remove)(HashTable, Key);
typedef void (*Clear)(HashTable);

#define dickGetKey(he) ((he)->k)
#define dictGetVal(he) ((he)->v)

#define dictSetKey(d, e, k) do { \
  if ((d)->type->dupkey) \
    (e)->k = (d)->type->dupkey((k)) \
  else \
    (e)->k = (k) \
} while(0)

#define dictSetVal(d, e, v) do { \
  if ((d)->type->dupval) \
    (e)->v = (d)->type->dupval((v)) \
  else \
    (e)->v = (v) \
} while(0)

#define dictFreeKey(d, e) \
  if ((d)->type->deskey) \
    (d)->type->deskey((e)->k)

#define dictFreeVal(d, e) \
  if ((d)->type->desval) \
    (d)->type->desval((e)->v)

#define dictCmpKeys(d, k1, k2) \
  (((d)->type->equal) ? \
    (d)->type->equal((k1), (k2)) : \
    (k1) == (k2))


typedef struct ht_type {
  Hash hash;
  Equal equal;
  DupKey dupkey;
  DupValue dupval;
  DestroyKey deskey;
  DestroyValue desval;
}*HT_TYPE;

typedef struct dict {
  HashTable ht;
  HT_TYPE type;
} *Dict;


Dict creat_hash_table(HT_TYPE ty = NULL);
void destroy_hash_table(Dict d);






#endif


