/* 输入二叉树的前序、中序遍历结果，请重建出该二叉树。
 * 假设输入的前序、中序遍历结果不含重复数字
 *
 * */
#include <stdio.h>
#include <stdlib.h>

#define TAB_SPACE "    "        // 终端打印时的缩进: 4spaces


typedef struct node_s node_t;
typedef struct node_s tree_t;

struct node_s {
    int val;
    node_t *l;
    node_t *r;
};



static tree_t *tree_rebuild(int preorder[], int inorder[], int n);
static void tree_print(const tree_t *t, int depth);
static void tree_fprint(const tree_t *t);
static void tree_print_recursively(const tree_t *t, FILE* f, int depth);
static void tree_destroy(tree_t *t);
static node_t *node_creat(int v);


int main(int argc, char *argv[]) {
    int preorder[8] = {1, 2, 4, 7, 3, 5, 6, 8};
    int inorder[8] =  {4, 7, 2, 1, 5, 3, 8, 6};
    tree_t *tree = tree_rebuild(preorder, inorder, 8); 
    if (tree == NULL) {
        printf("rebuild tree fail\n");
        return 1;
    }

    if (system("which mktree > /dev/null") == 0) {
        /* 依赖mktree 安装 */
        tree_fprint(tree);
    } else {
        tree_print(tree, 0);
    }
    tree_destroy(tree);
    return 0;
}


tree_t *tree_rebuild(int preorder[], int inorder[], int n) {
    if (n < 1) return NULL;
    tree_t *tree = node_creat(preorder[0]);

    int ln = 0;
    for (; ln < n; ln++) {
        if (inorder[ln] == preorder[0])
            break;
    }
    tree->l = tree_rebuild(preorder + 1     , inorder         , ln);
    tree->r = tree_rebuild(preorder + ln + 1, inorder + ln + 1, n - ln - 1);
    return tree;
}

/* 向终端输出数
 * Rules:
 *   1. 每行表示一个节点
 *   2. 相同缩进为同一层级节点
 *   3. * 表示空左孩子，仅对无左子树有右子树情况
 */
void tree_print(const tree_t *t, int depth) {
    if (t == NULL) return;

    for (int i = 0; i < depth; i++) fprintf(stdout, TAB_SPACE);
    fprintf(stdout, "%d\n", t->val);

    if (t->l == NULL) {
        if (t->r == NULL) return;
        for (int i = 0; i <= depth; i++) fprintf(stdout, TAB_SPACE);
        fprintf(stdout, "*\n");
        tree_print(t->r, depth+1);
        return;
    }
    tree_print(t->l, depth+1);
    tree_print(t->r, depth+1);
    return;
}

/* 当前路径下生成辅助文件tree.txt 记录树节点(依赖mktree安装)
 * 并生成tree.png图片展示二叉树数结构
 * Rules:
 *   1. 第一行为属性声明，具体格式参见mktree
 *   2. 每行表示一个节点
 *   3. 相同缩进为同一层级节点
 *   4. :表示空左孩子，仅对无左子树有右子树情况
 *
 */
void tree_fprint(const tree_t *t) {
    FILE* file = fopen("./tree.txt", "w");
    if (file == NULL) {
        fputs("open ./tree.txt fail\n", stdout);
        return;
    }
    fputs("= dx 22 dy 20 [style=filled fontcolor=black]\n", file);
    tree_print_recursively(t, file, 0);
    fclose(file);

    system("mktree ./tree.txt png b && rm ./tree.txt");
}

void tree_print_recursively(const tree_t *t, FILE* f, int depth) {
    if (t == NULL) return;

    for (int i = 0; i < depth; i++) fputs("  ", f); 
    fprintf(f, ":%d: [fillcolor=white]\n", t->val);

    if (t->l == NULL) {
        if (t->r == NULL) return;

        for (int i = 0; i <= depth; i++) fputs("  ", f); 
        fputs(":\n", f);
        tree_print_recursively(t->r, f, depth+1);
        return;
    }

    tree_print_recursively(t->l, f, depth+1);
    tree_print_recursively(t->r, f, depth+1);
}

/* 释放以t为根的树空间 */
void tree_destroy(tree_t *t) {
    if (t  == NULL) return;
    if (t->l) tree_destroy(t->l);
    if (t->r) tree_destroy(t->r);
    free(t);
}



/* 创建一个树节点，值为参数v */
node_t *node_creat(int v) {
    node_t *n = malloc(sizeof(node_t));
    if (n == NULL) return NULL;

    n->val = v;
    n->l = n->r = NULL;
    return n;
}
