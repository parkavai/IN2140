CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40

all:
	echo "Choose make local or make remote"

test:
	echo "Choose make test-local or make test-remote"
		
local: main-home

remote: main-ifi
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig2/; make main-ifi"

main-home: allocation.c inode.c create_fs.c
	gcc ${CFLAGS} allocation.c inode.c create_fs.c -o ferdig-program

main-ifi: allocation.c inode.c create_fs.c
	gcc ${CFLAGS} allocation.c inode.c create_fs.c -o ferdig-program

test-local:
	./ferdig-program 

test-remote: 
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig2/; ./ferdig-program"

valgrind: main-ifi
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig2/; valgrind ${WFLAGS}./ferdig-program"

clean:
	rm -f main-home
	rm -f vowelshift
