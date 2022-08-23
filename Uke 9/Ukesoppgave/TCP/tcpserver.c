#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Lengden på køen av avventende forbindelser
#define BACKLOG 3
// Porten serveren lytter på
#define PORT 2021
// Antall bytes til bufferet som det leses inn i.
#define BUFSIZE 256

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

int main(void){
    int listening_so, com_so, rc;
    char buf[BUFSIZE];
    struct sockaddr_in addr;

    /*
      Oppretter en socket som skal brukes til å motta forespørsler å forbindelser
      (altså klienter som kaller 'connect'). Fungerer som et møtepunkt mellom
      klient og server.
     */
    listening_so = socket(AF_INET, SOCK_STREAM, 0);
    check_error(listening_so, "socket");

    // Setter inn verdier i structen addr.
    // addr representerer hele adressen vi skal lytte på. Adressen er en
    // kombinasjon av IP og portnummer.
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    // 'Binder' socketen sammen med adressen.
    rc = bind(listening_so, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    // Bytter 'modus' på socketen. Socketen skal ikke lytte etter meldinger, men
    // etter klienter som ønsker å opprette en forbindelse.
    rc = listen(listening_so, BACKLOG);
    check_error(rc, "listen");

    // Aksepterer forbindelsen til en klient som ønsker å koble til. at andre og
    // tredje argument er 'NULL' gjør at vi aksepterer hvem som helst.
    com_so = accept(listening_so, NULL, NULL);
    check_error(com_so, "accept");

    // Leser melding opp til BUFSIZE - 1 antall bytes.
    rc = read(com_so, buf, BUFSIZE - 1);
    check_error(rc, "read");
    buf[rc] = '\0';
    printf("%s\n", buf);

    close(com_so);
    close(listening_so);

    return EXIT_SUCCESS;

}