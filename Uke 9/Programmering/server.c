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
#define BUFSIZE 300

void check_error(int res, char *msg){
    if(res == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}


/*
                    * recv()*
Det er akkurat det motsatte av sendto() og har noen av de samme parameterne.


                    * bind() *
Denne funksjonen er essensielt for at mottaker skal kunne hente datapakken som ligger 
i endepunktet til en socket. Hva denne gjør, er at den knytter sammen socketen med et navn 
hvor navn er da "adressen" fra senderen som skal sende til mottakeren. "INADDR_ANY" er viktig 
slik at bind() skal lytte på alle interfaces, altså IP-adresser og "INADDR_ANY" vil hente 
senderen sin IP-adresse og dets pakke, som skal tas imot av mottakeren. Altså vi vil hente 
datapakken som er knyttet til mottakerens IP-adresse via "INADDR_ANY".

                    * recvFrom()*
Gjennom denne funksjonen, så kan vi finne ut hvem "senderen" var. Hvem var det som faktisk 
sendte datapakken til oss. Det er da essensen ved denne funksjonen. 

*/



int main(void){

    int fd, rc, wc;
    char buf[BUFSIZE];
    struct sockaddr_in my_addr, src_addr;
    socklen_t addr_len;
    char *reply = "Hei tilbake fra server";

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(fd, "socket");

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY; 

    rc = bind(fd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    // Må sjekke om kallet var korrekt ved recv()
    /*
    rc = recv(fd, buf, BUFSIZE - 1 , 0);
    check_error(rc, "recv");
    */

    addr_len = sizeof(struct sockaddr_in);
    rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&src_addr, &addr_len);
    check_error(rc, "recv");

    buf[rc] = 0;
    
    printf("Vi mottok %c\n", buf[1]);

    // Her skal vi skrive tilbake til "client.c" 
    wc = sendto(fd, reply, strlen(reply), 0, (struct sockaddr*) &src_addr, sizeof(struct sockaddr_in));
    check_error(wc, "sendto Client");
 
    close(fd);
    return EXIT_SUCCESS;
}