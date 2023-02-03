#include "stack.h"
#include "expression.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


extern int yylex(void);

int expidx;
Exp expr[128];


static int calculate(int *ret);



int main() {
    expidx = 0;
    memset(expr, 0, 128*sizeof(Exp));

    yylex();

    /// for (int i = 0; i < expidx; i++) {
    ///     if (expr[i].type == NUMBER) {
    ///         printf("%d\n", expr[i].exp.val);
    ///     } else if (expr[i].type == OP) {
    ///         printf("%c\n", expr[i].exp.op);
    ///     } else {
    ///         printf("type %d\n", expr[i].type);
    ///     }
    /// }
    /// return 0;

    int result = 0;
    int err = calculate(&result);

    if (!err) {
        printf("= %d\n", result);
    } else {
        printf("check your expresion, err: %d\n", err);
    }
    
    return err;
}

enum Op { ADD = 0, SUB, MUL, DIV, MOD, OPNUM };
static const int kPrio[OPNUM][OPNUM] = {
/*    +   -   *   /   %  */
    { 0,  0, -1, -1, -1},    /* + */ 
    { 0,  0, -1, -1, -1},    /* - */ 
    { 1,  1,  0,  0, -1},    /* * */ 
    { 1,  1,  0,  0, -1},    /* / */ 
    { 1,  1,  1,  1,  0},    /* % */ 
};

static inline int OpCode(char op) {
    switch (op) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '%': return MOD;
    }
    return OPNUM;
}

int cmp_op(char op1, char op2) { 
    if (op1 == op2) return 0;
    if (op1 == '(') return 1;
    if (op2 == '(') return -1;
    return kPrio[OpCode(op1)][OpCode(op2)];
}

int cal(int l, char op, int r) {
    switch (op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/': return l / r;
        case '%': return l % r;
    }
    return 0;
}

int calculate(int *result) {
    Stack *nst = Stack_new(sizeof(int), 0); assert(nst);
    Stack *opst = Stack_new(sizeof(char), 0); assert(opst);
    int err = 0;

    for (int i = 0; i < expidx; i++) {
        Exp e = expr[i];
        switch (e.type) {
            case NUMBER: {
                int *n = Stack_push(nst);
                *n = e.exp.val;
                break;
            }
            case OP: {
                if (e.exp.op == '(') {
                    char *c = Stack_push(opst);
                    *c = e.exp.op;
                    break;
                } 
                
                if (e.exp.op == ')') {
                    while (*(char *)Stack_top(opst) != '(') {
                        int r = *(int *)Stack_pop(nst);
                        int l = *(int *)Stack_pop(nst);
                        char op = *(char *)Stack_pop(opst);
                        int *ret = Stack_push(nst);
                        *ret = cal(l, op, r);
                    }
                    if (*(char *)Stack_top(opst) != '(') 
                        return 3;
                    Stack_pop(opst);
                    break;
                }

                while (!Stack_empty(opst)) {
                    if (cmp_op(e.exp.op, *(char *)Stack_top(opst)) > 0) {
                        break;
                    }
                    if (*(char *)Stack_top(opst) == '(') {
                        break;
                    }

                    int r = *(int *)Stack_pop(nst);
                    int l = *(int *)Stack_pop(nst);
                    char op = *(char *)Stack_pop(opst);
                    int *ret = Stack_push(nst);
                    *ret = cal(l, op, r);
                } 
                char *c = Stack_push(opst);
                *c = e.exp.op;
                break;
            }
            default: err = 1; goto DONE;
        }
    }
    while (!Stack_empty(opst)) {
        int r = *(int *)Stack_pop(nst);
        int l = *(int *)Stack_pop(nst);
        char op = *(char *)Stack_pop(opst);
        int *ret = Stack_push(nst);
        *ret = cal(l, op, r);
    }

    if (Stack_size(nst) == 1 &&
        Stack_empty(opst)) {
        *result = *(int *)Stack_pop(nst);
        goto DONE;
    }
    err = 2;

DONE:
    Stack_delete(nst);
    Stack_delete(opst);
    return err;
}
