#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <time.h>

#define BUFSIZE 100
#define THRESHOLD 5

void check_error(int ret, char const *msg) {
    if (ret == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const *argv[]) {
    int so, rc;
    unsigned short myport, destport;
    fd_set set;
    time_t last_ack, curr_time;
    char buf[BUFSIZE];
    struct timeval tv;
    struct sockaddr_in myaddr, destaddr;
    struct in_addr ipadresse;

    if (argc < 3) {
        printf("usage: %s port destinasjonsport\n", argv[0]);
        return EXIT_FAILURE;
    }

    myport = atoi(argv[1]);
    destport = atoi(argv[2]);

    /* initialiserer sockaddr-structene */
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(myport);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    inet_pton(AF_INET, "127.0.0.1", &ipadresse);
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(destport);
    destaddr.sin_addr = ipadresse;

    so = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(so, "socket");

    rc = bind(so, (struct sockaddr *)&myaddr, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    /* sett opp fd_set */
    FD_ZERO(&set);
    FD_SET(so, &set);

    /* initialiserer timeval structen */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    /* start timer */
    last_ack = time(NULL);

    while (1) {
        rc = select(FD_SETSIZE, &set, NULL, NULL, &tv);
        check_error(rc, "select");

        /* Vi har mottatt en melding */
        if (rc && FD_ISSET(so, &set)) {
            rc = read(so, buf, BUFSIZE - 1);
            check_error(rc, "read");
            buf[rc] = '\0';

            if (!strcmp(buf, "Hei")) {
                printf("Til stede\n");
                last_ack = time(NULL);
            }
        }
        /* select har timet ut */
        else  {
            rc = sendto(so, "Hei", 3, 0, (struct sockaddr *)&destaddr, sizeof(struct sockaddr_in));
            check_error(rc, "sendto");

            curr_time = time(NULL);
            if (curr_time - last_ack > THRESHOLD)
                printf("Borte\n");

            /* reset structene til select */
            tv.tv_sec = 2;
            tv.tv_usec = 0;
            FD_SET(so, &set);
        }
    }

    return EXIT_SUCCESS;
}