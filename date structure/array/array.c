#include "array.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#ifdef DEBUG_MODE
#include "stdio.h"
#endif

static void autosize(Array* this);

Array* Array_ctor(int elesize, int capacity) {
    Array *array;
    array = malloc(sizeof(Array));
    if (array == NULL) {
        return NULL;
    }
    array->capacity = capacity ? capacity : 8;
    array->esize = elesize;
    array->size = 0;
    array->data = malloc(array->capacity * array->esize);
    if (array->data == NULL) {
        free(array);
        return NULL;
    }

    return array;
}

void Array_dtor(Array* a) {
    if (a == NULL) return;
    if (a->data) free(a->data);
    free(a);
}

void *Array_push_back(Array* this) {
    autosize(this);
    void *last = (char *)this->data + (this->size * this->esize);
    this->size += 1;
    return last;
}

void Array_pop_back(Array* this) {
    assert(this->size);
    this->size--;
}

void *Array_back(Array* this) {
    if (0 == this->size) return NULL;
    void *last = this->data + (this->size - 1) * this->esize;
    return last;
}

void Array_insert(Array* this, void *o, unsigned int idx) {
    autosize(this);
    char *data = (char *)this->data;
    if (idx >= this->size) {
        memcpy(data + (this->size * this->esize), o, this->esize);
        this->size++;
        return;
    }
    size_t n = ((unsigned int)this->size - idx) * this->esize;
    memmove(data + (idx + 1) * this->esize, this->data + idx * this->esize, n);
    memcpy(data + idx * this->esize, o, this->esize);
    this->size++;
}

void Array_remove(Array* this, int idx) {
    assert(idx >= 0);
    assert(idx < this->size);
    if (idx + 1 == this->size) {
        this->size--;
        return;
    }
    char* data = (char *)this->data;
    size_t n = (this->size - idx) * this->esize;
    memmove(data + idx * this->esize, data + (idx + 1) * this->esize, n);
    this->size--;
    return;
}

void Array_clear(Array* this) {
    this->capacity = 8;
    // this->esize
    this->size = 0;
    this->data = realloc(this->data, this->capacity * this->esize);
}

void *Array_at(Array* this, int idx) {
    assert(idx >= 0 && idx < this->size);
    return this->data + idx * this->esize;
}

int Array_size(Array* this) { return this->size; }

int Array_capacity(Array* this) { return this->capacity; }

int Array_empty(Array* this) { return this->size ? 0 : 1; }





void autosize(Array* this) {
    int oldsize = this->capacity;

    if (this->size * 10 > 9 * this->capacity) {
        this->capacity *= this->capacity > 1024 ? 1.125 : 2;
    } else if (this->size * 10 < this->capacity) {
        if (this->capacity > 1024) {
            this->capacity = this->capacity >> 1;
        } else if (this->capacity > 64) {
            this->capacity -= 32;
        }
    }

    if (oldsize != this->capacity) {
        this->data = reallocarray(this->data, this->capacity, this->esize);
    }
}




#ifdef DEBUG_MODE

void print_array(Array* a) {
    int size = Array_size(a);
    printf("a: [");
    for (int i = 0; i < size; i++) {
        printf("%d ", *(int *)Array_at(a, i));
    }
    printf("]\n");
}

int main() {
    Array* a = Array_ctor(sizeof(int), 0);
    if (a == NULL) {
        printf("create array fail\n");
        return 0;
    }


    int *p;
    int i = 0;
    do {
        p = Array_push_back(a);
        *p = i++;
    } while (Array_size(a) < 1000);
    print_array(a);
    
    printf("now capacity of a is %d\n\n", Array_capacity(a));

    int cnt = 99;
    Array_insert(a, &cnt, 6);
    print_array(a);

    Array_pop_back(a);
    print_array(a);

    Array_remove(a, 3);
    print_array(a);


    p = Array_back(a);
    printf("now last element:  %d\n", *p);

    char key = 0;
    do {
        printf("enter d to destroy the array:");
        scanf("%c", &key);
    } while(key != 'd');

    Array_clear(a);
    print_array(a);

    Array_dtor(a);

    printf("press any key to exit");
    getchar();
    getchar();
    return 0;
}

#endif