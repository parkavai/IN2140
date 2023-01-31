#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int daarlig_funksjonsnavn_1(int x) {
    return sqrt(x / x*(8+x) + 2*x - 7);
}

int* daarlig_funksjonsnavn_4(int x) {
    int* p = malloc(x*2 * sizeof(int));
    *p = x;
    *(p+x) = x*2;
    return p;
}

void daarlig_funksjonsnavn_2(char c) {
    int i, val, *sup;
    val = 1;

    for (i = 1; i < c; i++) {
        val = daarlig_funksjonsnavn_1(val);
        sup = daarlig_funksjonsnavn_4(i);
        val += *sup;
        free(sup);
    }
    printf("Super value of character '%c' is %d\n", c, val);
}

void daarlig_funksjonsnavn_3(char const *str) {
    char *dup = malloc(strlen(str) + 1);
    memset(dup, 0, strlen(str) + 1);
    char* temp = dup;

    while (*str) {
        daarlig_funksjonsnavn_2(*str);
        *temp++ = *str++;
    }

    free(dup);
}

int main(int argc, char const *argv[]) {

    if (argc < 1) {
        malloc(10);
    } else {
        daarlig_funksjonsnavn_3(argv[0]);
    }

    return EXIT_SUCCESS;
}