#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define BUFSIZE 300

void check_error(int i, char *msg) {
    if(i == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void get_string(char buf[], int size) {
    char c;
    fgets(buf, size, stdin);

    /* fjern newline fra slutten av strengen */
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    }
    /* Fjern resten av stdin dersom mer enn BUFSIZE ble lest inn */
    else while ((c = getchar()) != '\n' && c != EOF);
}


int main(int argc, char const *argv[]) {
    int so, rc;
    unsigned short readport, writeport;
    fd_set set;
    struct sockaddr_in my_addr, dest_addr;
    struct in_addr ip_addr;
    char buf[BUFSIZE];

    if(argc < 3) {
        printf("Usage: ./msn readport writeport\n");
        return 0;
    }

    readport = atoi(argv[1]);
    writeport = atoi(argv[2]);

    so = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(so, "socket");

    inet_pton(AF_INET, IP, &ip_addr);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(readport);
    my_addr.sin_addr = ip_addr;

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(writeport);
    dest_addr.sin_addr = ip_addr;

    rc = bind(so, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    FD_ZERO(&set);
    printf("Welcome to msn. Write quit to leave\n\n");
    FD_SET(STDIN_FILENO, &set);
    FD_SET(so, &set);
    buf[0] = '\0';
    while ( strcmp(buf, "quit") ) {
        printf("[You] ");
        fflush(NULL);
        rc = select(FD_SETSIZE, &set, NULL, NULL, NULL);
        check_error(rc, "select");

        if(FD_ISSET(STDIN_FILENO, &set)) {
            get_string(buf, BUFSIZE);
            rc = sendto(so, buf, strlen(buf), 0, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr_in));
            check_error(rc, "sendto");
        } else if (FD_ISSET(so, &set)) {
            rc = read(so, buf, BUFSIZE - 1);
            check_error(rc, "read");
            buf[rc] = '\0';
            printf("\r%45s [Ano]\n", buf);
        }
    }

    /*
    if(FD_ISSET(STDIN_FILENO, &set)) {
            get_string(buf, BUFSIZE);
            rc = sendto(so, buf, strlen(buf), 0, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr_in));
            check_error(rc, "sendto");
            rc = read(STDIN_FILENO, buf, BUFSIZE - 1);
            printf("\r%45s [Ano] skrev \n", buf);
        } else if (FD_ISSET(so, &set)) {
            rc = read(so, buf, BUFSIZE - 1);
            check_error(rc, "read");
            buf[rc] = '\0';
            printf("\r%45s [Ano]\n", buf);
            if(strcmp(buf, "hei") == 0){
                printf("HEI BLE SKREVET");
                char* message = "HALLA";
                rc = sendto(so, message, strlen(message), 0, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in));
            }
        }
    */

    close(so);
    return EXIT_SUCCESS;
}