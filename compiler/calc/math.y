%{
    #include <stdio.h>

    extern int yyerror(const char *);

    extern int yylex();
%}


%token        NUMBER
%token        ADD SUB MUL DIV
%token        OP CP
%token        EOL

%%


calclist: /* empty rule */
        |   calclist exp EOL { printf(" = %d\n", $2); }
        ;

exp: factor 
   | exp ADD factor { $$ = $1 + $3; }
   | exp SUB factor { $$ = $1 - $3; }
   ;

factor: term
      | factor MUL term { $$ = $1 * $3; }
      | factor DIV term { $$ = $1 / $3; }
      ;

term: NUMBER { $$ = $1; }
    | OP exp CP { $$ = $2; }
    ;


%%


int main(int argc, char *argv[]) {
    yyparse();
    return 0;
}

int yyerror(const char* msg) {
    printf("error: %s\n", msg);
    return 0;
}


