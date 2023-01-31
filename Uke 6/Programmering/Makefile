CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes --malloc-fill=0x40

main: test.c
	gcc ${CFLAGS} test.c -o test

run: main
	./test 

valgrind: main
	valgrind --leak-check=full --show-leak-kinds=all --malloc-fill=0x40 --track-origins=yes ./test

file:
	dd count=100 if=/dev/random of=./large-file 

clean:
	rm -f main