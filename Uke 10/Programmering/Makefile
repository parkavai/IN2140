CFLAGS = -g -std=gnu11 -Wall -Wextra
BIN = client server

all: $(BIN)

client: client.c 
	gcc $(CFLAGS) client.c -o client

server: server.c 
	gcc $(CFLAGS) server.c -o server
	
clean:
	rm -f $(BIN)