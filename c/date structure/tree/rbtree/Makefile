cc = gcc

cflags += -Wall -g

cflags += -D TEST_MODE

test: rbtree.c
	$(cc) $(cflags) rbtree.c ../memory_pool/xpool.c -o test
	
clean:
	rm test tree.png tree.txt
