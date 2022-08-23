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


// Dette programmet fokuserer kun på UDP
void check_error(int res, char *msg){
    if(res == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    
    /* 
                            * sendto()-funksjon * 
    Ved UDP, så trenger man kun denne funksjonen ifølge "Olav", fyren fra CBRA. Når man skal bruke "sendto", så må man vite hvor 
    man skal sende datapakken til. Da må vi benytte oss av ip-adresser for å sende datapakkene til. IPV4 er 4 bytes lange, altså 32-bits. 
    
    Vi kan bruke IP-en "127.0.0.1" som er localhost, dvs at jeg kan sende datapakken til meg selv. 

    Vi må også bruke en adresseport slik at mottakeren som skal motta denne datapakken, skal vite hvor pakken kom fra og at senderen 
    vet hvor datapakken skal sendes til. En port bruker "unsigned short", og er viktig å sjekke returverdien til porten ettersom det 
    kan hende at adresseporten er opptatt og kan derfor ikke brukes. 

    I man-siden for "sendto()", så sier den følgende: sendto() har en del parametere, 
    som er følgende: sszie_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t, addrlen). 
    "sockfd" referer til fildeskriptoren, "buf" referer til et buffer som kan sees som datapakken som skal sendes, 
    "len" beskriver lengden på bufferet, "flags" er bare et flagg du kan sette på, men satt den på 0, "*dest_addr" er ganske 
    selvforklarende som beskriver altså destinasjonen/adressen som pakken skal sendes til, "socklen_t" beskriver lengden på adressen, 
    "addrlen" beskriver lengden på adressen

    // Mer info om *dest_addr
    Så i CBRA-videoen så blir det nevnt at "*dest_addr" kan sees som en superklasse i java hvor i man 7 ip så vil du 
    se de ulike klassene som kan lages av denen superklassen. Blant annet har man tcp_socket, udp_socket og 
    ram_socket. Vi skal bruke udp_socket og den har følgende parametere: socket(AF_INET, SOCK_DGRAM, 0). Men det viktigste 
    du må vite herifra, er dette med "adresse formatet". Hvis du går til man side "7 ip" så vil du se at vi har en 
    "struct sockaddr_in". Denne structen er den vi skal bruke for parameteren i sendto(), altså "* dest_addr". 
    
    Structen ser slik ut:
    struct sockaddr_in {
        sa_family_t     sin_family; // address family: AF_INET //
        in_port_t   sin_port; // port in network byte order //
        struct in_addr  sin_addr; // Internett adress //
    }

    Struct in_addr ser slik utd:
    struct in_addr {
        uint32_t    s_addr; // Adress in network byte order //
    }

                            * socket * 
    I man-siden for "socket" så sier den at man lager et endepunkt for kommunikasjon og returnerer en fildeskriptor som 
    referer til det endepunktet. Så med "endepunkt" så mener vi f.eks at en klient og en server er koblet sammen gjennom 
    et "endepunkt" som gjør det mulig å koble sammen to endesystemer. Så når det gjelder dette med "fildeskriptor" så er 
    det slik at, i en vanlig operasjon hvor du bruker fildeskriptorer, så skriver du til en fil iks. Men for dette tilfellet 
    så skriver vi heller en fil ut på internettet, i dette tilfellet så vil fildeskriptoren skrive til endepunktet som 
    kobler sammen to endesystemer med hverandre. Nå til å se hva slags parametere som tas inn i socket. 

    socket() tar imot følgende parametere: int socket(int domain, int type, int protocol). Protocol skulle ikke 
    vi tenke på foreløpig, men fokusere kun på "domain" og "type". Så ved domene så refereres dette til type 
    tilkoblingsdomene som skal benyttes for kommunikasjon. De viktigste som vi skal forholde oss til, er AF_INET
    og AF_INET6. AF_INET er IPV4 imens AF_INET6 er IPV6. "Type" referer til type data relevant for protokollen som 
    blitt benyttet. F.eks så er "SOCK_DGRAM" typen for UDP imens "SOCK_STREAM" typen for TCP, hvor disse to er de vi 
    trenger å forholde oss til. 

    
    */

    int fd, wc, rc;
    char *msg = "CBRA sender over localhost";
    struct sockaddr_in dest_addr;
    struct in_addr ip_addr;
    char buf[BUFSIZE];

    /*
    Ved kall på socket() så må man sjekke returverdien. Hvis returverdien er "-1" så var det ikke et 
    suksessfult systemkall(), ellers så var det en suksess akkurat som ved fread, fwrite osv. 
    */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(fd, "socket");

    /*
    Denne funksjonen "inet_pton()" gjør det mulig å konvertere en IP-adresse i streng-form, over 
    til en format som brukes i nettverkskommunikasjon så den er must å bruke. Så ved denne funksjonen 
    så må man også sjekke at kallet ble gjort riktig så hvis returverdien ikke er "-1", så er det riktig. 
    Ellers har det skjedd en feil. 
    */
    wc = inet_pton(AF_INET, IP, &ip_addr.s_addr);
    check_error(wc, "inet_pton");
    if (!wc) {
        fprintf(stderr, "Invalid IP adress: %s\n", IP);
        return EXIT_FAILURE;
    }
    dest_addr.sin_family = AF_INET;
    /*
    htons() kommer fra at når det gjelder endian og big-endian så har man da et format for hvordan 
    bytene skal være formatert i nettverket. Noen nettverk er forskjellig, men i denne oppgaven så trenger 
    vi kun å forholde oss til htons(PORT). 
    */
    dest_addr.sin_port = htons(PORT);
    dest_addr.sin_addr = ip_addr;

    wc = sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr_in));
    // Må også sjekke om returkallet var en suksess eller ikke hvor "-1" betyr at kallet gikk galt. 
    check_error(wc, "sendto");

    // Lese innhold fra server.c
    rc = recv(fd, buf, BUFSIZE - 1, 0);
    check_error(rc, "recv");
    buf[rc] = 0;
    printf("Vi fikk som svar %s\n", buf);

    close(fd);
    return EXIT_SUCCESS;
}