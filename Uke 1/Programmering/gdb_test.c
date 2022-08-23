#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    char x = 15;
    short y = 1 << ++x;

    if (y) {
        printf("How long is a short?\n");
    }

    x--;
    y = 1 << x;

    if (y > 16000) {
        printf("Did this short need your signature?\n");
    }

    y <<= 5;

    printf("%ld\n", (unsigned long)y * 12345l);

    return EXIT_SUCCESS;
}