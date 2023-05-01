calc: token.l math.y
	bison -d math.y
	flex token.l
	cc -o $@ math.tab.c lex.yy.c -lfl

clean:
	rm -f lex.yy.c math.tab.c math.tab.h calc
