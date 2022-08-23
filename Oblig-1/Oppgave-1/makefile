CFLAGS = -g -Wall -Wextra -std=gnu11
WFLAGS = --track-origins=yes --malloc-fill=0x40

# Kompilerer filene
compile: vowelshift.c
	gcc ${CFLAGS} vowelshift.c -o vowelshift

# NB!!!, har ikke lagt inn en run-kommando siden bruker skal taste inn argumenter.

# Kjoerer valgrind med flag som gjoer det enklere med error-analysering(Hentet fra CBRA-videoene).
# Har allerede lagt inn et argument. Skal argumentet endres saa er det bare aa endre verdiene mellom anfoerselstegn. 
valgrind: 
	valgrind ${WFLAGS} ./vowelshift "Lorem ipsum dolor sit amet" a

# Fjerner output-filen
clean:
	rm -f ./vowelshift