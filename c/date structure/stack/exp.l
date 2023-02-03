%{
#include <stdio.h>
#include "stack.h"
#include "expression.h"

%}


number [0-9]+[.]?[0-9]*

%%



[  \t|\n]          { /*skip blanks*/ }

{number}        {
    expr[expidx].type = NUMBER;
    expr[expidx].exp.val = atoi(yytext);
    expidx++;
}

.           {  
    expr[expidx].type = OP;
    expr[expidx].exp.op = yytext[0];
    expidx++;
}


%%


int yywrap() { return 1; }
