# DATA STRUCTURE & OBJECT

All key-value pair in Redis is consist of object.
Key is always a string object.
Value could be a object of string, list, hash, set, sorted set.

## SDS(simple dynamic string)

SDS is default string representation in redis. C string is used as string literal, it is read-only. For examnple print log.

```
struct sdshdr {
  int len;
  int free;
  char buf[];
};
```


## LIST



## DICT


## SKIPLIST



## INTSET


## ZIPLIST


## OBJECT




# IMPLEMENTATION of SINGLE DATABASE



# IMPLEMENTATION of MULTI DATABASE



# IMPLEMENTATION of INDEPENDENT FUNCTION
