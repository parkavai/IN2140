CFLAGS = -g -Wall -Wextra -std=gnu11 -lm
WFLAGS = --track-origins=yes --leak-check=full

run:lenkeliste
	./lenkeliste

lenkeliste:lenkeliste.c
	gcc lenkeliste.c -o lenkeliste $(CFLAGS)

check:clean lenkeliste
	valgrind $(WFLAGS) ./lenkeliste

clean:
	rm -f lenkeliste