#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// servers IP-adresse.
#define IPADDR "127.0.0.1"
// Porten serveren lytter på
#define PORT 2021

/* Brukes til å sjekke returverdien til funksjoner som returnerer -1 ved feil.
 * Printer ut funksjonen som forårsaket feilen, pluss en feilmelding satt av
 * funksjonen (sistnevnte skjer automatisk med perror). Avslutter programmet.
 */
void check_error(int i, char *msg){
    if(i == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    int so, wc;
    struct sockaddr_in addr;
    struct in_addr ip_addr;

    // Formaterer en IP-adresse i streng-format og kopierer den inn i ip_addr
    wc = inet_pton(AF_INET, IPADDR, &ip_addr);
    check_error(wc, "inet_pton");
    if (!wc) {
        fprintf(stderr, "Invalid IP address %s\n", IPADDR);
        return EXIT_FAILURE;
    }

    // Setter inn verdier i structen addr.
    // addr representerer hele adressen til serveren, altså en kombinasjon av
    // IP og portnummer.
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr = ip_addr;

    // Oppretter en socket
    so = socket(AF_INET, SOCK_STREAM, 0);
    check_error(so, "socket");

    // Etablerer en forbindelse (connection) med server
    wc = connect(so, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    check_error(wc, "connect");

    // Sender hardkodet melding til server
    char *msg = "Cbra in Cyberspace!";
    wc = write(so, msg, strlen(msg));
    check_error(wc, "write");

    close(so);

    return EXIT_SUCCESS;
}