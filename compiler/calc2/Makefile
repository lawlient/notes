calc: lexer.l parser.y calc.*
	bison -d parser.y
	flex lexer.l
	gcc  -o $@ calc.c parser.tab.c lex.yy.c -lfl -lm

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h calc
