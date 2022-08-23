#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void check_error(int ret, char *msg) {
    if (ret == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

/*
 * Dette programmet tar ikke inn noen argumenter, men sender til en hardkodet
 * port 1234 og adresse (localhost). Dette burde byttes ut til at programmet tar
 * inn port og IP-adresse som argumenter.
 */
int main(void) {
    int so, rc;
    char *msg;
    struct in_addr ipadresse;
    struct sockaddr_in adresse;

    /* setter localhost som IPv4-adresse */
    rc = inet_pton(AF_INET, "127.0.0.1", &ipadresse);
    check_error(rc, "inet_pton");
    if (rc == 0) {
        fprintf(stderr, "IP address not valid\n");
        return EXIT_FAILURE;
    }

    /* skal sende med IPv4, port 1234 og IPadresse localhost */
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(1234);
    adresse.sin_addr = ipadresse;

    /* vil ha en socket som sender datagrammer med IPv4 */
    so = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(so, "socket");

    /* sender en hardkodet melding */
    msg = "Cbra in Cyberspace!";
    rc = sendto(so,                             /* socket */
                msg,                            /* bytes som skal sendes */
                strlen(msg),                    /* antall bytes som sendes */
                0,                              /* flagg (ingen) */
                (struct sockaddr *)&adresse,    /* IP og port å sende til */
                sizeof(struct sockaddr_in));    /* størrelse på adresse-struct*/
    check_error(rc, "sendto");

    printf("Sendte %d bytes!\n", rc);

    close(so);
    return EXIT_SUCCESS;
}