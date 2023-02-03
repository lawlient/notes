#ifndef EXPRESSION_H__
#define EXPRESSION_H__

enum ExpType { NUMBER, OP };

typedef struct Exp_ {
    enum ExpType type;
    union {
        char op;
        int val;
    } exp;
} Exp;

extern int expidx;
extern Exp expr[128];


#endif
