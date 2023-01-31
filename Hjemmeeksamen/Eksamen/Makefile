CC = gcc
CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40 --free-fill=0x23 --leak-check=full --show-leak-kinds=all
BINARIES = upush_client upush_server
CLIENT = message_linked_list.c block_nickname_list.c client_linked_list.c send_packet.c upush_client.c -o upush_client
SERVER = client_linked_list.c send_packet.c upush_server.c -o upush_server

# Prepared some clients and servers in order to test valgrind errors and such
CLIENT1 = Ole 127.0.0.1 4501 15 10
CLIENT2 = Dole 127.0.0.1 4501 15 40
CLIENT3 = Doffen 127.0.0.1 4501 15 20
CLIENT4 = Joakim 127.0.0.1 4501 15 80
CLIENT5 = Carsten 127.0.0.1 4501 15 30

SERVER1 = 4501 30
SERVER2 = 4501 60

all: upush_client upush_server

upush_client: clean
	$(CC) $(CFLAGS) $(CLIENT)

upush_server: clean
	$(CC) $(CFLAGS) $(SERVER) 

client1: upush_client
	valgrind ${WFLAGS} ./upush_client $(CLIENT1)

client2: upush_client
	valgrind ${WFLAGS} ./upush_client $(CLIENT2)

client3: upush_client
	valgrind ${WFLAGS} ./upush_client $(CLIENT3)

client4: upush_client
	valgrind ${WFLAGS} ./upush_client $(CLIENT4)

client5: upush_client
	valgrind ${WFLAGS} ./upush_client $(CLIENT5)

server1: upush_server
	valgrind ${WFLAGS} ./upush_server $(SERVER1)

server2: upush_server
	valgrind ${WFLAGS} ./upush_server $(SERVER2)

clean:
	rm -f $(BINARIES) 


