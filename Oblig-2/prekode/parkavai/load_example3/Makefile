CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40 --free-fill=0x23 --leak-check=full --show-leak-kinds=all

all:
	echo "Choose make local or directly to valgrind"
		
local: main-home

main-home: 
	gcc -I../ ${CFLAGS} ../allocation.c ../inode.c load_fs.c -o load_fs3

test-local:
	./load_fs3

valgrind: local
	valgrind ${WFLAGS} ./load_fs3

clean:
	rm -f load_fs3