#include "xpool.h"

#include <malloc.h>

static void *alloc_small(memory_pool_t *pool, int size);
static void *alloc_large(memory_pool_t *pool, int size);
static void *alloc_segment(memory_pool_t *pool, int size);



memory_pool_t *mp_create(size_t size) {
    memory_pool_t *p;

    p = (memory_pool_t *)memalign(16, size);
    if (p == NULL) return NULL;

    p->head.start = p + sizeof(memory_pool_t);
    p->head.end = p + size + sizeof(memory_pool_t);
    p->head.next = NULL;
    p->current = p;
    p->large = NULL;
    size = size - sizeof(memory_pool_t);
    p->max = MIN(size, PAGESIZE);
    
    return p;
}


void mp_destroy(memory_pool_t *pool) {
    if (pool == NULL) return;
    memory_pool_t *p, *pn;
    memory_large_block_t *l, *n;
   
    for (l = pool->large; l; l = l->next) {
        if (l->data) free(l->data);
    }

    for (p = pool, pn = p->head.next; /*void*/; p = pn, pn = p->head.next) {
        free(p);
        if (!pn) break;
    }
}


void *mp_alloc(memory_pool_t *pool, size_t size) {
    if (size < pool->max) {
        return alloc_small(pool, size);
    } else {
        return alloc_large(pool, size);
    }
}

void *alloc_small(memory_pool_t *pool, int size) {
    memory_pool_t* p = pool;
    void *d;

    do {
        if (size <= p->head.end - p->head.start) {
            d = p->head.start;
            p->head.start += size;
            return d;
        }
        p = p->head.next;
    } while (p);
    return alloc_segment(pool, size);
}


void *alloc_large(memory_pool_t *pool, int size) {
    void *m = malloc(size);
    if (m == NULL) return NULL;

    memory_large_block_t *l;
    l = mp_alloc(pool, sizeof(memory_large_block_t));
    l->data = m;
    l->next = pool->large->next;
    pool->large->next = l;
    return m;
}

void *alloc_segment(memory_pool_t *pool, int size) {
    memory_pool_t *np;
    int psize = pool->head.end - (char *)pool;
    np = memalign(16, psize);
    if (np == NULL) return NULL;

    void *d = np + sizeof(memory_pool_t);
    np->head.start = np + sizeof(memory_pool_t) + size;
    np->head.end  = np + psize;
    np->head.next = NULL;
    pool->current->head.next = np;
    pool->current = np;
    return d;
}
