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

WFLAGS = --track-origins=yes --malloc-fill=0x40

all:
	echo "Choose make local or make remote"

test:
	echo "Choose make test-local or make test-remote"
		
local: main-home

remote: main-ifi
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig/oppgave-3; make main-ifi"

main-home: apple.c
	gcc ${CFLAGS} apple.c the_apple.c -o eplet

main-ifi: apple.c
	gcc ${CFLAGS} apple.c the_apple.c -o eplet

test-local:
	./eplet

test-remote: 
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig/oppgave-3; ./eplet"

valgrind: main-ifi
	ssh parkavai@login.ifi.uio.no "cd IN2140/oblig/oppgave-3; valgrind ${WFLAGS} ./eplet"

clean:
	rm -f eplet