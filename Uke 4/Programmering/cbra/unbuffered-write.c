// Nødvendig å legge til for å åpne filer
#include <fcntl.h>
#include <stdio.h>
// Gjør at vi kan returnere EXIT_FAILURE
#include <stdlib.h>
// Gjør at vi kan "close" og "write" filen som vi gjør i andre programmeringsspråk
#include <unistd.h>

// Open, write og close er "system-kall", vikitg å merke

int main(void){
    int fd, wc;

    /* 
        Tar imot to parametre hvor den ene er filen du skal åpne, den andre derimot 
        beskriver rettighetene du ønsker å gjøre ved filen. Ønsker du å lese filen, 
        skriver du "O_RDONLY". Ved skriving til fil så skriver du "O_WRONLY".
    */
    fd = open("text.txt", O_WRONLY);
    // Ellers vil den returnere antall bytes fra filen, "0" indikerer at vi er på slutten av filen. 


    /*
    fd = open("text.txt", O_WRONLY | O_TRUNC);

        Forskjellen mellom denne og over, er at vi har inkludert en "| O_TRUNC". 
        Dette er en bitvis-operasjon som vil si at du åpner filen men skriver 
        tekst i henhold til antall bytes du satt inn for skrive-operasjonen. 
        Så hvis jeg skrev "wc = write(fd, "hei", 3)" så vil vi skrive inn "3 bytes"
        som er da "hei" inn i tekstfilen.
    */

   /*
   fd = open("text.txt", O_CREATE);
         For å lage en ny fil, så benyttes kommandoen O_CREATE som da lager en ny fil 
    */

    // Må sjekke om fildeskriptoren ikke returnerte -1, det betyr at filen ikke fantes
    if(fd == -1){
        perror("Skjedde feil ved open");
        return EXIT_FAILURE;
    }

    /* 
        Tar imot tre parametre hvor den ene er fildeskriptoren, den andre er 
        bufferet du ønsker å skrive til din fil, den siste beskriver antall bytes som skal skrives
        fra bufferet. 
    */
    wc = write(fd, "hei", 3);
    // Må sjekke om operasjonen var gyldig eller ikke
    if(wc == -1){
        perror("Skjedde feil ved write");
        return EXIT_FAILURE;
    }
    // Hvis operasjonen var gyldig, har vi da endret textfilen vår.
    close(fd);
    return EXIT_SUCCESS;
}

