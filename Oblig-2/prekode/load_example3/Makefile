CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40 --free-fill=0x23 --leak-check=full --show-leak-kinds=all

all:
	echo "Choose make local or make remote, for valgrind choose: local-valgrind or remote-valgrind"

test:
	echo "Choose make test-local or make test-remote"
		
local: main-home

remote: 
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig2/; make main-ifi"

main-home: 
	gcc -I../ ${CFLAGS} ../allocation.c ../inode.c load_fs.c -o load_fs3

main-ifi: 
	gcc ${CFLAGS} allocation.c inode.c load_fs.c -o load_fs3

test-local:
	./load_fs3

test-remote: 
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig2/; ./load_fs3"

local-valgrind: local
	valgrind ${WFLAGS} ./load_fs3

remote-valgrind: remote
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig2/; valgrind ${WFLAGS} ./load_fs3"

clean:
	rm -f load_fs3