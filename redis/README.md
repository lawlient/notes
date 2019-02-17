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


## SKIPLIST



## INTSET


## ZIPLIST


## OBJECT




# IMPLEMENTATION of SINGLE DATABASE



# IMPLEMENTATION of MULTI DATABASE



# IMPLEMENTATION of INDEPENDENT FUNCTION
