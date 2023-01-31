CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --malloc-fill=0x40

main: main.c
	gcc ${CFLAGS} main.c -o main 

run: main
	./main processes.bin

valgrind: main
	valgrind --leak-check=full --show-leak-kinds=all --malloc-fill=0x40 --track-origins=yes ./main processes.bin

file:
	dd count=100 if=/dev/random of=./large-file 

clean:
	rm -f main