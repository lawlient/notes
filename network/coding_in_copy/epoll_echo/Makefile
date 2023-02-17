all: et lt client

et: et_server.c
	gcc -g -Wall et_server.c -o et

lt: echo_server.c
	gcc -g -Wall echo_server.c -o lt

client: echo_client.c
	gcc -g -Wall echo_client.c -o client

clean:
	rm et lt client core