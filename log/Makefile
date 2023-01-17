INC=-I./agent -I./
AGENT_SRC=$(wildcard ./agent/*.c)

TEST_SRC=$(wildcard ./client/*.c)

all: log_agent test

log_agent: $(AGENT_SRC) alog.c logging.c
	gcc -g -Wall $(INC) $(AGENT_SRC) alog.c logging.c -o log_agent -lconsole -lpthread


test: $(TEST_SRC) alog.c logging.c
	gcc -g -Wall $(INC) $(TEST_SRC) alog.c logging.c -o test -lpthread

clean:
	rm core test log_agent log
