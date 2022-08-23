 #include <stdio.h>
#include <stdlib.h>

struct person {
    unsigned char alder;
    char *navn;
    unsigned long fnr;
};

struct person *kostruerPerson(char *navn, unsigned char alder, unsigned long fnr) {
    struct person *ny = malloc(sizeof(struct person));
    ny->alder = alder;
    ny->navn = navn;
    ny->fnr = fnr;
    return ny;
}

void printPerson(struct person *pers) {
    printf("Navn: %s\nalder: %u\nfodselsnummer: %lu\n",
                pers->navn, pers->alder, pers->fnr);
}

int main() {
    printf("sizeof(struct person) = %ld\n", sizeof(struct person));

    struct person *magnus = kostruerPerson("Magnus", 29, 11019112345);
    printPerson(magnus);
    free(magnus);

    return 0;
}