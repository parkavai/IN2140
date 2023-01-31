CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
BINARIES = server

all: $(BINARIES)

%.o: %.c
	$(CC) $^ -c -o $@ $(CFLAGS)

server: server.o
	$(CC) $^ -o $@ $(CFLAGS)

valgrind: server
	valgrind ./server

clean:
	rm -f $(BINARIES) *.o

debug: CFLAGS += -DDEBUG -g
debug: all
