#ifndef MEMORY_POOL_HEADER__
#define MEMORY_POOL_HEADER__

#include <stdlib.h>

#define PAGESIZE 1024
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef struct memory_pool_s memory_pool_t;
typedef struct memory_pool_head_s memory_pool_head_t;
typedef struct memory_large_block_s memory_large_block_t;


struct memory_pool_head_s {
    char *start;
    char *end;
    memory_pool_t *next;
};

struct memory_large_block_s {
    memory_large_block_t *next;
    void* data;
};

struct memory_pool_s {
    memory_pool_head_t head;
    memory_pool_t *current;
    memory_large_block_t *large;
    int max;
};


memory_pool_t *mp_create(size_t size);
void mp_destroy(memory_pool_t *pool);

void *mp_alloc(memory_pool_t *pool, size_t size);






















#endif
