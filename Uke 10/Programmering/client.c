#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Trengs for systemkallene for nettverksoperasjon
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define PORT 2021
#define IP "127.0.0.1"

void check_error(int res, char *msg){
    if(res == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    int fd, wc;
    struct sockaddr_in dest_addr;
    char *msg = "Dette er en melding fra klient (TCP)";
    char* stream;

    // SOCK_STREAM er det du bruker for TCP
    fd = socket(AF_INET, SOCK_STREAM ,0);
    check_error(fd, "Socket in beginning");

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    wc = inet_pton(AF_INET, IP, &dest_addr.sin_addr.s_addr);
    check_error(fd, "inet_pton");
    if(! wc){
        printf("inet_pton: not right adress format\n");
        exit(EXIT_FAILURE);
    }

    /*
    Denne funksjonen er et kall som benyttes for TCP hvor hensikten er som følger: Systemkallet 
    knytter en socket referert av en fildeskriptor, til en adresse spesifisert av addr. Så denne 
    funksjonen knytter sammen en fildeskriptor og "dest_addr" sammen i vårt tilfelle som er 
    serveren som skal sendes til. 
    */
    wc = connect(fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
    check_error(wc, "connect");

    stream = malloc(strlen(msg) + 1);
    stream[0] = (unsigned char) strlen(msg);
    memcpy(&stream[1], msg, strlen(msg));


    wc = 0;
    while(wc < (int) strlen(msg) + 1){
        wc += write(fd, &msg[wc], strlen(msg) + 1 - wc);
        check_error(wc, "write-loop");
    }

    wc = write(fd, msg, strlen(msg));
    check_error(wc, "write");

    close(fd);
    return EXIT_SUCCESS;
}
