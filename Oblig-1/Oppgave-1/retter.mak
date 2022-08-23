#
# Idea for a Makefile that can be used for local & remote compiling
# and local & remote testing if you use SSHFS
# Works for MAC and Linux
#
# On Mac and Linux, just must first do something like this:
#     cd ~
#     mkdir ifi-home
#     sshfs griff@login.ifi.uio.no: ifi-home
#
CFLAGS = -g -Wall -Wextra -std=gnu11

all:
	echo "Choose make local or make remote"

test:
	echo "Choose make test-local or make test-remote"
		
local: main-home

remote: main-ifi
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig/oppgave-1; make main-ifi"

main-home: vowelshift.c
	gcc ${CFLAGS} vowelshift.c -o vowelshift

main-ifi: vowelshift.c
	gcc ${CFLAGS} vowelshift.c -o vowelshift

test-local:
	./vowelshift "Lorem ipsum dolor sit amet" a

test-remote:
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig/oppgave-1; ./vowelshift "Lorem ipsum dolor sit amet" a"

argumenter: 
	./vowelshift "Lorem ipsum dolor sit amet" a

valgrind: main-ifi
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig/oppgave-1; make valgrind argumenter"

clean:
	rm -f main-home
	rm -f vowelshift
