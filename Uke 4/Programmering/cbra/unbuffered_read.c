// Nødvendig å legge til for å åpne filer
#include <fcntl.h>
#include <stdio.h>
// Gjør at vi kan returnere EXIT_FAILURE
#include <stdlib.h>
// Gjør at vi kan "close" og "write" filen som vi gjør i andre programmeringsspråk
#include <unistd.h>

// Open, write og close er "system-kall", vikitg å merke

// Makro-verdier, kan sees på det samme som konstanter
#define BUFSIZE 100

int main(void){
    int fd, rc;
    char buf[BUFSIZE];

    /* 
        Tar imot to parametre hvor den ene er filen du skal åpne, den andre derimot 
        beskriver rettighetene du ønsker å gjøre ved filen. Ønsker du å lese filen, 
        skriver du "O_RDONLY". Ved skriving til fil så skriver du "O_WRONLY".
    */
    fd = open("text.txt", O_RDONLY);

    // Må sjekke om fildeskriptoren ikke returnerte -1, det betyr at filen ikke fantes
    if(fd == -1){
        perror("Skjedde feil ved open");
        return EXIT_FAILURE;
    }
    // Ellers vil den returnere antall bytes som ble riktig lest, "0" indikerer at vi er på slutten av filen. 

    /* 
        Tar imot tre parametre hvor den ene er fildeskriptoren, den andre er 
        bufferet du ønsker å skrive til din fil, den siste beskriver antall bytes som skal leses
        fra bufferet. Så hvis vi har strengen "hello world", så vil den lese "hel" siden vi har 
        3 bytes.  
    */
    rc = read(fd, buf, BUFSIZE);
    // Må sjekke om operasjonen var gyldig eller ikke
    if(rc == -1){
        perror("Skjedde feil ved write");
        return EXIT_FAILURE;
    }

    // Må sette nullbyte, kan da bruke readcounter(rc) siden den vil lese igjennom hele filen mot slutten.
    buf[rc] = 0;
    printf("I bufferet ligger %s\n", buf);

    // Hvis operasjonen var gyldig, har vi da endret textfilen vår.
    close(fd);
    return EXIT_SUCCESS;
}

