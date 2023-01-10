#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

#include "../memory_pool/xpool.h"

static void rbtree_rotate_left(rbtree_t* tree, rbtree_node_t* node);
static void rbtree_rotate_right(rbtree_t* tree, rbtree_node_t* node);
static int rbtree_insert_red_node(rbtree_t* tree, rbtree_node_t* node);
static void rbtree_print_node_recursively(rbtree_node_t* tmp, rbtree_node_t* nil, int depth, FILE* f);


#define disengage(node) { node->parent = node->left = node->right = NULL; }

void rbtree_init(rbtree_t* tree, rbtree_node_t* nil) {
    tree->root = nil;
    tree->nil  = nil;
    set_black(tree->nil);
}


inline int rbtree_empty(rbtree_t* tree) { return tree->root == tree->nil; }

void rbtree_insert(rbtree_t* tree, rbtree_node_t* node) {
    rbtree_node_t** root = &tree->root;
    rbtree_node_t* nil   = tree->nil;

    if (rbtree_empty(tree)) {
        *root = node;
        node->left = node->right = nil;
        set_black(node);
        return;
    }

    int exist = rbtree_insert_red_node(tree, node);
    if (exist) return;

    
    while (node != *root && is_red(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            rbtree_node_t* u = node->parent->parent->right;

            if (is_red(u)) {
                set_black(u);
                set_black(node->parent);
                set_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rbtree_rotate_left(tree, node);
                }

                set_black(node->parent);
                set_red(node->parent->parent);
                rbtree_rotate_right(tree, node->parent->parent);
            }
        } else {
            rbtree_node_t* u = node->parent->parent->left;

            if (is_red(u)) {
                set_black(u);
                set_black(node->parent);
                set_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbtree_rotate_right(tree, node);
                }

                set_black(node->parent);
                set_red(node->parent->parent);
                rbtree_rotate_left(tree, node->parent->parent);
            }
        }
    }

    set_black(*root);
}


void rbtree_delete(rbtree_t* tree, rbtree_node_t* node) {
    rbtree_node_t* *root = &tree->root;
    rbtree_node_t* nil   = tree->nil;

    rbtree_node_t *sub, *tmp;

    if (node->left == nil) {
        sub = node;
        tmp = sub->right;
    } else if (node->right == nil) {
        sub = node;
        tmp = sub->left;
    } else {
        sub = rbtree_min(tree, node->right);
        tmp = sub->right;
    }

    if (sub == *root) {
        *root = tmp;
        set_black(tmp);
        disengage(node);
        return;
    }

    int is_red_sub = is_red(sub);

    if (sub == sub->parent->left) {
        sub->parent->left = tmp;
    } else {
        sub->parent->right = tmp;
    }

    if (sub == node) {
        tmp->parent = sub->parent;
    } else {
        if (sub->parent != node) {
            tmp->parent = sub->parent;
        } else {
            tmp->parent = sub;
        }

        if (node == *root) {
            *root = sub;
        } else if (node == node->parent->left) {
            node->parent->left = sub;
        } else {
            node->parent->right = sub;
        }
        sub->left = node->left;
        sub->right = node->right;
        sub->parent = node->parent;
        cpy_color(sub, node);

        if (sub->left != nil) sub->left->parent = sub;
        if (sub->right != nil) sub->right->parent = sub;
    }

    disengage(node);
    node->key = 0;

    if (is_red_sub) return;

    /* Fix up */
    while (tmp != *root && is_black(tmp)) {
        if (tmp == tmp->parent->left) {
            rbtree_node_t* b = tmp->parent->right;

            if (is_red(b)) {
                set_black(b);
                set_red(tmp->parent);
                rbtree_rotate_left(tree, tmp->parent);
                b = tmp->parent->right;
            }

            if (is_black(b->left) && is_black(b->right)) {
                set_red(b);
                tmp = tmp->parent;
            } else {
                if (is_red(b->left)) {
                    set_black(b->left);
                    set_red(b);
                    rbtree_rotate_right(tree, b);
                    b = tmp->parent->right;
                }

                cpy_color(b, tmp->parent);
                set_black(tmp->parent);
                set_black(b->right);
                rbtree_rotate_left(tree, tmp->parent);
                tmp = *root;
            }

        } else {
            rbtree_node_t* b = tmp->parent->left;

            if (is_red(b)) {
                set_black(b);
                set_red(tmp->parent);
                rbtree_rotate_right(tree, tmp->parent);
                b = tmp->parent->left;
            }

            if (is_black(b->left) && is_black(b->right)) {
                set_red(b);
                tmp = tmp->parent;
            } else {
                if (is_red(b->right)) {
                    set_black(b->right);
                    set_red(b);
                    rbtree_rotate_left(tree, b);
                    b = tmp->parent->left;
                }

                cpy_color(b, tmp->parent);
                set_black(tmp->parent);
                set_black(b->left);
                rbtree_rotate_right(tree, tmp->parent);
                tmp = *root;
            }
        }
    }

    set_black(tmp);
}


rbtree_node_t* rbtree_find(rbtree_t* tree, key_t key) {
    if (rbtree_empty(tree)) return NULL;
    rbtree_node_t* nil = tree->nil;
    rbtree_node_t** p;
    rbtree_node_t* tmp = tree->root;
    while (1) {
        if (tmp->key == key) return tmp;
        p = (key < tmp->key ? &tmp->left : &tmp->right);
        if (*p == nil) break;
        tmp = *p;
    }
    return NULL;
}


rbtree_node_t* rbtree_min(rbtree_t* tree, rbtree_node_t* node) {
    rbtree_node_t* nil = tree->nil;
    rbtree_node_t** p;
    rbtree_node_t* tmp = node;
    for (;;) {
        p = &tmp->left;
        if (*p == nil) break;
        tmp = *p;
    }
    return tmp;
}

int rbtree_insert_red_node(rbtree_t* tree, rbtree_node_t* node) {
    rbtree_node_t* nil   = tree->nil;

    rbtree_node_t* tmp = tree->root;
    rbtree_node_t** p;
    for(;;) {
        if (node->key == tmp->key) return 1;
        p = (node->key < tmp->key ? &tmp->left : &tmp->right);
        if (*p == nil) break;
        tmp = *p;
    }

    *p = node;
    node->parent = tmp;
    node->left = nil;
    node->right = nil;
    set_red(node);
    return 0;
}

void rbtree_rotate_left(rbtree_t* tree, rbtree_node_t* node) {
    rbtree_node_t** root = &tree->root;
    rbtree_node_t* nil = tree->nil;

    rbtree_node_t* pr = node->right;

    if (node == *root) {
        *root = pr;
    } else if (node == node->parent->left) {
        node->parent->left = pr;
    } else {
        node->parent->right = pr;
    }
    pr->parent = node->parent;

    node->right = pr->left;
    if (pr->left != nil) pr->left->parent = node;

    node->parent = pr;
    pr->left = node;
}

void rbtree_rotate_right(rbtree_t* tree, rbtree_node_t* node) {
    rbtree_node_t** root = &tree->root;
    rbtree_node_t* nil = tree->nil;

    rbtree_node_t* pl = node->left;

    if (node == *root) {
        *root = pl;
    } else if (node == node->parent->left) {
        node->parent->left = pl;
    } else {
        node->parent->right = pl;
    }
    pl->parent = node->parent;

    node->left = pl->right;
    if (pl->right != nil) pl->right->parent = node;

    node->parent = pl;
    pl->right = node;
}


void rbtree_print_node_recursively(rbtree_node_t* tmp, rbtree_node_t* nil, int depth, FILE* f) {
    if (tmp == nil) return;
    for (int i = 0; i < depth; i++) fputs("  ", f); 
    fprintf(f, ":%d: [fillcolor=%s]\n", tmp->key, (is_red(tmp) ? "Red" : "Black"));

    if (tmp->left == nil) {
        if (tmp->right == nil) return;

        for (int i = 0; i <= depth; i++) fputs("  ", f); 
        fputs(":\n", f);
        rbtree_print_node_recursively(tmp->right, nil, depth+1, f);
        return;
    }

    rbtree_print_node_recursively(tmp->left, nil, depth+1, f);
    rbtree_print_node_recursively(tmp->right, nil, depth+1, f);
}

void rbtree_print(rbtree_t* tree) {
    FILE* file = fopen("./tree.txt", "w");
    if (file == NULL) {
        fputs("open ./tree.txt fail\n", stdout);
        return;
    }
    fputs("= dx 22 dy 20 [style=filled fontcolor=white]\n", file);
    rbtree_print_node_recursively(tree->root, tree->nil, 0, file);
    fclose(file);

    system("mktree ./tree.txt png b");
}




#ifdef TEST_MODE

int main() {

    rbtree_t tree;
    rbtree_node_t nil;
    rbtree_init(&tree, &nil);

    memory_pool_t* pool = mp_create(1 * 1024 * 1024 * 1024);
    if (pool == NULL) {
        fputs("create memory pool fail\n", stdout);
        return 0; 
    }

    fputs("TEST INSERTION NOW\n", stdout);
    key_t input;
    while (1){
        fputs("input a number > 0 insert into tree(-1 for exit):", stdout);
        fflush(stdout);
        scanf("%d", &input);
        if (input == -1) break;

        rbtree_node_t* n = mp_alloc(pool, sizeof(rbtree_node_t));
        if (n == NULL) {
            fputs("malloc fail\n", stdout);
            exit(1);
        }
        n->key = input;
        rbtree_insert(&tree, n);
        rbtree_print(&tree);
    }

    fputs("TEST DELETION NOW\n", stdout);
    while (1){
        fputs("input a number > 0 to be deleted from tree(-1 for exit):", stdout);
        fflush(stdout);
        scanf("%d", &input);
        if (input == -1) break;

        rbtree_node_t* n = rbtree_find(&tree, input);
        if (n == NULL) {
            printf("cannot found %d in the tree\n", input);
            continue;
        }
        rbtree_delete(&tree, n);
        rbtree_print(&tree);
    }

    mp_destroy(pool);

    fputs("press any key to exit\n", stdout);
    scanf("%d", &input);
    return 0;
}


#endif
