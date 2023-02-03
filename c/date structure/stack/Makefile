all: test exp



test: test_stack.c stack.c
	gcc -g -Wall stack.c test_stack.c -o teststack


exp: expression.c stack.c exp.l
	flex exp.l
	gcc -g -Wall expression.c lex.yy.c stack.c -o exp


clean:
	rm exp teststack 
	rm lex.yy.c
