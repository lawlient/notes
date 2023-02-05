#include "sds.h"

#include <string.h>
#include <stdlib.h>


#define SdsAddNil(s) s->data[s->size] = '\0'

#define MIN(x, y) ((x) < (y) ? (x) : (y))


Sds *Sds_new() {
    Sds *s = malloc(sizeof(Sds));
    if (s == NULL) return NULL;

    s->capacity = 0;
    s->size = 0;
    return s;
}

Sds *Sds_newraw(const char* o) {
    int size = strlen(o);
    int capa = size + 1;
    Sds *s = malloc(sizeof(Sds) + capa);
    if (s == NULL) return NULL;

    s->capacity = capa;
    s->size = size;
    memcpy(s->data, o, size);
    SdsAddNil(s);
    return s;
}

void Sds_delete(Sds *s) { free (s); }

Sds *Sds_append(Sds *s, const char *o) {
    int moresize = strlen(o);
    if (Sds_free(s) < moresize) {
        int newsize = Sds_size(s) + moresize;
        Sds *ns = realloc(s, newsize);
        if (ns == NULL) return NULL;
        s = ns;
        s->capacity = newsize;
    }

    memcpy(s->data + s->size, o, moresize);
    s->size += moresize;
    SdsAddNil(s);
    return s;
}

Sds *Sds_cat(Sds *s, Sds *o) { return Sds_append(s, o->data); }

int Sds_cmp(Sds *l, Sds *r) {
    int l1 = Sds_size(l);
    int l2 = Sds_size(r);
    int cmp = memcmp(l->data, r->data, MIN(l1, l2));
    if (cmp) return cmp;
    return (l1 > l2 ? 1 : -1);
}

void Sds_clear(Sds *s) { 
    s->size = 0;
    SdsAddNil(s);
}



Sds * Sds_assign(Sds *s, const char *o) {
    Sds_clear(s); 
    int newsize = strlen(o);
    if (Sds_free(s) < newsize) {
        Sds *ns = realloc(s, newsize);
        if (ns == NULL) return NULL;
        s = ns;
        s->capacity = newsize;
    }

    memcpy(s->data, o, newsize);
    s->size = newsize;
    SdsAddNil(s);
    return s;
}

char *Sds_at(Sds *s, int index) {
    if (index >= s->size) return NULL;
    return &(s->data[index]);
}
