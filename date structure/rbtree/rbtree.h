#ifndef RBTREE_HEADER__
#define RBTREE_HEADER__


typedef struct rbtree_s rbtree_t;
typedef struct rbtree_node_s rbtree_node_t;

typedef int key_t;
typedef void* value_t;


#define Black 0
#define Red   1

struct rbtree_node_s {
    rbtree_node_t* parent;
    rbtree_node_t* left;
    rbtree_node_t* right;
    int color;
    key_t key;
    value_t data;
};

struct rbtree_s {
    rbtree_node_t* root;
    rbtree_node_t* nil;
};


#define set_black(node) (node)->color = 0
#define set_red(node) (node)->color = 1

#define is_black(node) (node)->color == 0
#define is_red(node) (node)->color == 1

#define cpy_color(node1, node2) (node1)->color = (node2)->color


void rbtree_init(rbtree_t* tree, rbtree_node_t* nil);
int rbtree_empty(rbtree_t* tree);

void rbtree_insert(rbtree_t* tree, rbtree_node_t* node);
void rbtree_delete(rbtree_t* tree, rbtree_node_t* node);
rbtree_node_t* rbtree_find(rbtree_t* tree, key_t key);
rbtree_node_t* rbtree_min(rbtree_t* tree, rbtree_node_t* node);

void rbtree_print(rbtree_t* tree);



#endif
