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
#define BUFSIZE 300

void check_error(int res, char *msg){
    if(res == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
} 

int main(void)
{
    int moteplass_fd, msg_fd, rc;
    struct sockaddr_in my_addr;
    char buf[BUFSIZE];
    unsigned char msg_len;


    moteplass_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_error(moteplass_fd, "feil ved socket i server");

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    rc = bind(moteplass_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    /*
    Denne funksjonen bytter modus på fildeskriptoren fra å lese/skrive til å bli en møteplass. I denne sammenhengen
    så blir fildeskriptoren endret fra å lese og skrive meldinger mellom to endesystemer, til å bli en møteplass som 
    tar imot tilkoblinger som kobles direkte til denne møteplassen/serveren hvor da klienter kobler seg til. 
    Funksjonen tar imot følgende parametere listen(int sock_fd, int backlog). Backlog er en liste som beskriver hvor 
    mange klienter som kan koble seg opp mot denne møteplassen. Den setter altså en grense for hvor mange som kan koble 
    seg til den.  
    */

    rc = listen(moteplass_fd, 5);
    check_error(rc, "listen");

    /*

    */
    msg_fd = accept(moteplass_fd, NULL, NULL);
    check_error(msg_fd, "accept");

    rc = read(msg_fd, &msg_len, sizeof(char ));
    check_error(rc, "read");

    rc = 0;
    while(rc < msg_len){
        rc = read(msg_fd, &buf[rc], msg_len - rc);
        check_error(rc, "read");
    }
    buf[rc] = 0;
    printf("Vi mottok: %s\n", buf);

    close(msg_fd);
    close(moteplass_fd);
    return EXIT_SUCCESS;
}
