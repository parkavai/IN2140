CFLAGS = -g -std=gnu11 -Wall -Wextra
BIN = mas msn virus_client watchdog
WFLAGS = --track-origins=yes --malloc-fill=0x40 --free-fill=0x23 --leak-check=full --show-leak-kinds=all

all: $(BIN)

%:%.c
	gcc $(CFLAGS) $^ -o $@

watch:
	valgrind $(WFLAGS) ./watchdog 5132 2132

clean:
	rm -f $(BIN)