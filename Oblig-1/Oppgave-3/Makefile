CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40

# Kompilerer filene
compile: apple.c the_apple.c
	gcc ${CFLAGS} apple.c the_apple.c -o eplet

# Kjoerer programmet
run: 
	./eplet

# Kjoerer valgrind med flag som gjoer det enklere med error-analysering(Hentet fra CBRA-videoene).
valgrind:
	valgrind ${WFLAGS} ./eplet

# Fjerner output-filen
clean:
	rm -f eplet