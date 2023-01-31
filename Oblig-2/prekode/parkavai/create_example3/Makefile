CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40 --free-fill=0x23 --leak-check=full --show-leak-kinds=all

all:
	echo "Choose make local or directly to valgrind"
		
local: main-home

main-home: 
	gcc -I../ ${CFLAGS} ../allocation.c ../inode.c create_fs.c -o create_fs3

test-local:
	./create_fs3

valgrind: local 
	valgrind ${WFLAGS} ./create_fs3

clean:
	rm -f create_fs3