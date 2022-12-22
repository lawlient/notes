cc = gcc

cflags += -Wall -g

cflags += -D TEST_MODE

test: rbtree.c
	$(cc) $(cflags) rbtree.c -o test
	
clean:
	rm test
