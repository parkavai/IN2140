CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40 --free-fill=0x23 --leak-check=full --show-leak-kinds=all
BRUKERNAVN = parkavai

all:
	echo "Choose make local or make remote, for valgrind choose: local-valgrind or remote-valgrind"

test:
	echo "Choose make test-local or make test-remote"
		
local: main-home

remote: 
	ssh ${BRUKERNAVN}@login.ifi.uio.no "cd IN2140/oblig2/; make main-ifi"

main-home: 
	gcc -I../ ${CFLAGS} ../allocation.c ../inode.c create_fs.c -o create_fs1

main-ifi: 
	gcc ${CFLAGS} allocation.c inode.c create_fs.c -o create_fs1

test-local:
	./create_fs1

test-remote: 
	ssh ${BRUKERNAVN}@login.ifi.uio.no "cd IN2140/oblig2/; ./create_fs1"

local-valgrind: local 
	valgrind ${WFLAGS} ./create_fs1

remote-valgrind: remote
	ssh ${BRUKERNAVN}@login.ifi.uio.no "cd IN2140/oblig2/; valgrind ${WFLAGS} ./create_fs1"

clean:
	rm -f create_fs1