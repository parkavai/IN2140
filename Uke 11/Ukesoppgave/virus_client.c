#include "libraries.h"

/*
Den zoologiske etteretningstjenesten har oppdaget et virus og ønsker i den forbindelse å formidle informasjon om dette. 
De har laget server som gir ut informasjon om viruset. For å hindre at den botaniske etteretningstjenesten får tak i 
graderte opplysninger om viruset, har de valgt å kryptere meldingen. Ved å sende det riktige spørsmålet til serveren vil 
serveren svare med relevant informasjon. Sender man noe annet enn det riktige spørsmålet, vil man få rare meldinger som svar. 
Spørsmålet fungerer altså som et slags passord.

Filen "virus_info" er en UDP-server som lytter på port 2272. Når serveren mottar en melding sender den et svar i retur. 
Målet med denne konkurransen er å få serveren til å gi et svar som gir mening, altså en leselig tekst.

- Lag en UDP-klient som kan sende meldinger til virus_info.
- For å få et meningsfylt svar må du sende strengen "What virus?"" til serveren.
- Null-byten på slutten av strengen skal ikke sendes over nett. Linjeskift skal heller ikke sendes.
- Serveren sender et svar som ikke er nullterminert. Null-byten må legges til på mottakersiden.
- Du kan anta at meldingen ikke er større enn 256 bytes.

Her skal man altså lage en klient som både sender og mottar meldinger.

*/

#define BUFSIZE 325

#define PORT "2272"

void get_string(char buf[], int bufsize){
    char c;
    fgets(buf, bufsize, stdin);

    /* fjern newline fra slutten av strengen */
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    }
    /* Fjern resten av stdin dersom mer enn BUFSIZE ble lest inn */
    else while ((c = getchar()) != '\n' && c != EOF);
}

void check_error(int ret, char *msg) {
    if(ret == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void setup_sockaddr(struct sockaddr_in* addr,const char* port, struct in_addr ip_addr){
    addr->sin_family = AF_INET;
    addr->sin_port = htons(atoi(port));
    addr->sin_addr = ip_addr;
}

int main(void) {
    int msg_fd, wc;
    struct sockaddr_in client_addr; 
    struct in_addr ip_addr;
    char buf[BUFSIZE]; 
    
    inet_pton(AF_INET, "127.0.0.1", &ip_addr);

    setup_sockaddr(&client_addr, PORT, ip_addr);
  
    msg_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(msg_fd, "Error on socket");

    while( 1 ){
        printf("Enter text to send (q to quit):\n> ");
        get_string(buf, BUFSIZE);

        if (!strcmp(buf, "q") || !strcmp(buf, "quit")) break;

        wc = sendto(msg_fd, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, sizeof(struct sockaddr_in));
        check_error(wc, "sendto");

        wc = read(msg_fd, buf, BUFSIZE);
        check_error(wc, "read");

        buf[wc] = '\0';
        printf("%s\n", buf);
    }

    close(msg_fd);

    return EXIT_SUCCESS;
}