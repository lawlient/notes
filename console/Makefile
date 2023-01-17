
test: console.c test.c
	gcc -g -Wall console.c test.c -o test -lpthread

lib:
	gcc -g -Wall -c console.c -o console.o
	gcc -shared -fPIC -o libconsole.so console.o
	ar -cr libconsole.a console.o

install: console.c
	install console.h /usr/include/
	install libconsole.a /lib/x86_64-linux-gnu

clean:
	rm test core


uninstall:
	rm /usr/include/console.h
	rm /lib/x86_64-linux-gnu/libconsole.a
