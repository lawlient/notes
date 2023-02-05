#ifndef SDS_HEADER__
#define SDS_HEADER__



typedef struct Sds_ {
    int capacity;
    int size;
    char data[];
} Sds;


Sds *Sds_new();
Sds *Sds_newraw(const char*);
void Sds_delete(Sds *);

static inline int Sds_free(Sds *s) { return s->capacity - s->size; }
static inline int Sds_size(Sds *s) { return s->size; }
static inline int Sds_capacity(Sds *s) { return s->capacity; }
static inline int Sds_empty(Sds *s) { return s->size == 0; }
static inline char *Sds_data(Sds *s) { return s->data; }

Sds *Sds_append(Sds *, const char *);
Sds *Sds_cat(Sds *, Sds *);
int Sds_cmp(Sds *, Sds *);
void Sds_clear(Sds *);
int Sds_resize(Sds *);
int Sds_reserve(Sds *);

Sds * Sds_assign(Sds *, const char *);
char *Sds_at(Sds *, int index);
int Sds_insert(Sds *);
int Sds_erase(Sds *);








#endif
