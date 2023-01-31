CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40

# Kompilerer filene
compile: tests.c stringoperations.c
	gcc ${CFLAGS} tests.c stringoperations.c -o ferdig-program

# Kjoerer programmet
run: 
	./ferdig-program 

# Kjoerer valgrind med flag som gjoer det enklere med error-analysering(Hentet fra CBRA-videoene)
valgrind: 
	valgrind ${WFLAGS} ./ferdig-program

clean:
	rm -f ./ferdig-program