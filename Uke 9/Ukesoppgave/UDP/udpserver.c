#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFSIZE 250

void check_error(int ret, char *msg) {
    if (ret == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

/*
 * Dette programmet tar ikke inn noen argumenter, men lytter på en hardkodet
 * port 1234. Dette burde byttes ut til at programmet tar inn port som argument.
 */
int main(void) {
    int so, rc;
    char buf[BUFSIZE];
    struct in_addr ipadresse;
    struct sockaddr_in adresse;

    /* vil lytte med IPv4 på port 1234 på alle maskinens adresser,
       inkludert localhost */
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(1234);
    adresse.sin_addr.s_addr = INADDR_ANY;

    /* vil ha en socket som tar imot datagrammer med IPv4 */
    so = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(so, "socket");

    /* socketen skal lytte på adressen vi har satt (localhost, port 1234) */
    rc = bind(so, (struct sockaddr *)&adresse, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    /* vent til det kommer noe, så les opp til BUFSIZE - 1 bytes */
    rc = read(so, buf, BUFSIZE - 1);
    check_error(rc, "read");

    /* sett null-byte på slutten av meldingen */
    buf[rc] = '\0';
    printf("Received %d bytes: \n'%s'\n", rc, buf);

    close(so);
    return EXIT_SUCCESS;
}