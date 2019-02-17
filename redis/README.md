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
  1. memory prealloction
    ```
    if length of modified sds(sdshdr.len) < 1MB then
        alloc sdshdr.len unused memory
    else
        alloc 1MB unused memory
    ```
  2. inertia algrithm to free memory
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


## LIST



## DICT


## SKIPLIST



## INTSET


## ZIPLIST


## OBJECT




# IMPLEMENTATION of SINGLE DATABASE



# IMPLEMENTATION of MULTI DATABASE



# IMPLEMENTATION of INDEPENDENT FUNCTION
