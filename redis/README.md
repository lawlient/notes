# DATA STRUCTURE & OBJECT

All key-value pair in Redis is consist of object.
Key is always a string object.
Value could be a object of string, list, hash, set, sorted set.

## SDS(simple dynamic string)

SDS is default string representation in redis. C string is used as string literal, it is read-only. For examnple print log.

### DEFINITION

An sdshdr object represent a sds value.

```
struct sdshdr {
  int len;  // length of string, number of char in buf
  int free; // number of unused byte in buf
  char buf[]; // byte array to record string
};
```

String in buf is end with '\0', and it is opaque for user, in order to use string function in C library, as strcpy, strlen, strcat, printf and so on.

### SDS vs C String

1. O(1) to get length of string

2. avoid buffer overflow

3. reduce frequency of memory reallocation 
    + memory prealloction
    ```
    if length of modified sds(sdshdr.len) < 1MB
        alloc sdshdr.len unused memory
    else
        alloc 1MB unused memory
    ```
    + inertia algorithm to free memory 
    sds add sdshdr.free replace of free memory direct while sds API need to cut string in sdshdr.buf, and usethose memory to write char in the future.The inertia strategy reduce frequency of memory reallocation. SDS supply API to free unused memory indeed.

4. binary safe
  Char in C string have to satisfy an encoding style(as ASCII), and can't be empty char except ending, so C string just record text data, can't reserve binary data like picture, audio, vedio, compressed file and so on.

5. compatible for some C string functions

C stirng           |SDS
:-----------------:|:------------:
O(N) to get length | O(1) to get length
API unsafe,overflow|API safe
have to N times alloc while changing string length N times|at most N times alloc while changing string length  N times
just reserve text data|reserve text data or binary data
use all <string.h> functions| use some <string.h> functions

### SDS API

:API:         |:FUNCTION:                |:Time Complexity:
:-------------|:-------------------------|:---------------------
sdsnew|create a sds represent a given C string|O(N), N is length of C string
sdsempty|create an empty sds |O(1)
sdsfree|free a given sds|O(N), N is length of C string
sdslen|return used bytes of sds|O(1), return sdshdr.len
sdsavail|return unused bytes of sds|O(1), return sdshdr.free
sdsdup|create a copy of given sds|O(N), N is length of C string
sdsclear|clear string in buf|O(1), inertia algorithm
sdscat|append a given C string to sds|O(N), N is length of C string
sdscatsds|append a given sds to sds|O(N), N is length of the given sds
sdscpy|copy a given C string into sds replace of origin value|O(N), N is length of the copied C string
sdsgrowzero|add to given length of sds with ' '|O(N), N is number of ' ' 
sdsrange|reserve data between ragne and clear others|O(N), N is bytes of reserve data
sdstrim|remove all char including in C string in sds from right and left|O(M * N), M is length of sds, N is length of C string
sdscmp|compare wtheter two sdss are same or not|O(N), N is minimum length of two sdss


## LIST

List can resort node efficiently, and access each node ordered, and change length flexible by add or remove node. Redis implement list by itself, and use list to implement list key, publish, subscribe, slow request, monitor and other function. Besides, Redis reserve multi-client informations and build client output buffer by list.

### Implementation of list and list node

`adlist.h/listNode` represent a list node

```
typedef struct listNode {
    struct listNode *prev;   // prev node
    struct listNode *next;   // next node
    void *value;             // value of node
} listNode;

```

`adlist.h/list` represent a list

```
typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;

```

Redis list features:
+ double-end: prev and next pointer can access adjacent node with O(1)
+ no-cycle: `list.head->prev` and `list.tail->next` are `NULL`, access double-list end with `NULL`
+ have head and tail pointer: access list head and list tail with O(1)
+ has size counter: list.len record how many nodes list have
+ polymorphic: type of list.value is `void *`, and function dup, free, match can set given type value for node, so list can reserve value of different type


### List and List Node API


## DICT

Dict (symbol table, associative array, map) is a data structure to reserve key-value pair. Dict is widely used in Redis. Redis implement database with dict, and database insert, delete, select, update is built on dict ooperation. On the other hand, dict is used to implement hash key while there are too many key-value pairs in a hash key or many values are long string.


### Implementation of Dict
Redis implement dict with hash table, a hash table compare of many hash node, and a hash node reserve one key-value pair.

+ hash table
`dict.h/dictht` define hash table in Redis.

```
typedef struct dictht {
    dictEntry **table;        // hash table array
    unsigned long size;       // size of hash table
    unsigned long sizemask;   // to calc index, = size - 1
    unsigned long used;       // number of node in hash table
} dictht;
```

+ hash node 
`dict.h/dictEntry` define hash node to reserve one key-value pair.

```
typedef struct dictEntry {
    void *key;
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct dictEntry *next;
} dictEntry;
```

`dictEntry.next` point to another node, it connect multi node with same key to resolve collision.

+ dict 
`dict.h/dict` represent dict in Redis.

```
typedef struct dict {
    dictType *type;
    void *privdata;
    dictht ht[2];
    long rehashidx; /* rehashing not in progress if rehashidx == -1 */
    unsigned long iterators; /* number of iterators currently running */
} dict;
```

`type` and `privdata` is designed to support different type key-value pairs for polymorphic.

`type` is dictType pointer, and dictType reserve a series of function to operate given type key-value pair.
`privdata` reserve optional parameters for given type functions.

```
typedef struct dictType {
    uint64_t (*hashFunction)(const void *key);
    void *(*keyDup)(void *privdata, const void *key);
    void *(*valDup)(void *privdata, const void *obj);
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    void (*keyDestructor)(void *privdata, void *key);
    void (*valDestructor)(void *privdata, void *obj);
} dictType;
```

`dict.ht` is an array including two values, each value is a dictht, and `ht[0]` is used most time, `ht[1]` is used while rehashing. And `dict.rehashidx` represent schedule of rehashing, `dict.rehashidx = -1` if rehash is not going on.


### Hash algorithm

While inserting a new key-value pair into dict, first, redis calculate hash and index according to key, then insert new hash node in dict with index.

`hash = dict->type->hashFunction(key);`
`index = hash & dict->ht[x].sizemask;`

MurmurHash algorithm is designed by Austin Appleby in 2008, its advantage is supplying a good random distribution even if the input key is regular and it is efficient. You can access ![MurmurHash](http://code.google.com/p/smhasher/) to get more information.


### Solve collision

Collision: two or more keys are distributed to the same index.

Redis solve collision with separate chaining. Each hash node has a next pointer, so multi nodes could consist singly linked list by next.


### Rehash

Key in hash table will increase or decrease along with command executing, progress will expand or shrink hash table by rehash in order to control load factor in a right range. 

Rehash Steps:
1. Allocate memory for ht[1]. Size depends on what operation to be executed and number of keys in ht[0] ( = ht[0].used)
    + expand : size of ht[1] = (first 2^n >= 2 * ht[0].used)
    + shrink : size of ht[1] = (first 2^n >= ht[0].userd)
2. Recalculate hash and index for key in ht[0] migrate to ht[1], and then insert node in ht[0] into ht[1]
3. When all keys in ht[0] migrate to ht[1], progress free ht[0] and set ht[1] = ht[0] and create a empty dictht in ht[1] for next rehash

Load Factor

load_factor = ht[0].used / ht[0].size

Any condition as follows is satisfied, progress will start rehash to expand hash table.
1. Server is not execute BGSAVE or BGREWRITEAOF, and load factor of hash table >= 1
2. Server is executing BGSAVE or BGREWRITEAOF, and load factor of hash table >= 5
Progress will start rehash to shrink hash table if load factor <= 0.1

### Gradual rehash


### Dict API








## SKIPLIST



## INTSET


## ZIPLIST


## OBJECT




# IMPLEMENTATION of SINGLE DATABASE



# IMPLEMENTATION of MULTI DATABASE



# IMPLEMENTATION of INDEPENDENT FUNCTION
