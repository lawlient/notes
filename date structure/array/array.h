#ifndef ARRAY_HEADER__
#define ARRAY_HEADER__


typedef struct Array_ {
    int capacity;   // 当前容量
    int size;       // 当前元素个数
    int esize;      // 单个对象字节数
    void *data;
} Array;





Array* Array_ctor(int elesize, int capacity);
void Array_dtor(Array* a);
void *Array_push_back(Array* this);
void Array_pop_back(Array* this);
void *Array_back(Array* this);
void Array_insert(Array* this, void *o, unsigned int idx);
void Array_remove(Array* this, int idx);
void Array_clear(Array* this);
void *Array_at(Array* this, int idx);


int Array_size(Array* this);
int Array_capacity(Array* this);
int Array_empty(Array* this);



#endif