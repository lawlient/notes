#include <stdio.h>

typedef struct SM_ SM;
typedef struct State_ State;

#define SMState(sm) (sm)->sp
#define SMParse(sm) (SMState(sm))->parse(SMState(sm))
#define SMChar(sm) (*(sm)->a)
#define SMNextCh(sm) ((sm)->a++)

enum StateName { ST_INIT = 0, ST_PARSE, ST_NUM };

struct State_ {
    SM *sm;
    int (*parse)(State *this);
};

struct SM_ {
    const char* a;
    int i;
    int sign;
    State ss[ST_NUM];
    State *sp;
};


static int parse0(State *this) {
    SM* sm = this->sm;
    char c = SMChar(sm);
    switch (c) {
        case ' ': SMNextCh(sm); return 0;
        case '+': sm->sign =  1; sm->sp = &(sm->ss[ST_PARSE]); SMNextCh(sm); return 0;
        case '-': sm->sign = -1; sm->sp = &(sm->ss[ST_PARSE]); SMNextCh(sm); return 0;
        default:  sm->sp = &(sm->ss[ST_PARSE]); return 0;
    }
    return 1;
}

static int parse1(State *this) {
    SM* sm = this->sm;
    char c = SMChar(sm);
    switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            this->sm->i *= 10;
            this->sm->i += c - '0';
            SMNextCh(sm);
            return 0;
        }
        default: return 1;
    }
    return 1;
}


int atoi(const char* a) {
    if (a == NULL) return 0;
    SM sm = {
        a, 0, 1,
        {
            { &sm, parse0 },
            { &sm, parse1 },
        },
        sm.ss,
    };


    int err = 0;
    while (*(sm.a) != '\0' && !err) {
        err = SMParse(&sm);
    }
    if (err) {
        printf("something wrong at input[%d]:%c\n", (int)(sm.a - a), *(sm.a));
        return 0;
    }
    return sm.i * sm.sign;
}


int main(int argc, char *argv[]) {
    printf("input: %s\n", argv[1]);
    int i = atoi(argv[1]);
    printf("output: %d\n", i);
    return 0;
}
