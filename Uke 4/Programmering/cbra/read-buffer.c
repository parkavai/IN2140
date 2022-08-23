// Trengs for fopen() og uansett for å skrive til terminalen så er "must" uansett
#include <stdio.h>
// Gjør at vi kan returnere EXIT_FAILURE
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 255

int main(){

    /*
        Akkurat samme som ved "Java" hvor vi har en fil-type 
        men vi har da en peker som peker til en fil vi skal hente.
        I dette tilfelle skulle vi da skrive en fil så pekeren vil 
        peke på den nye filen vi ønsker å skrive til som er da "ny_fil.txt".
        Vi bruker da fopen("ny_fil.txt", "r") hvor vi da har innført leseperasjon
        "r" for den filen. 
    */
    FILE *fil;
    char buf[BUFSIZE];
    size_t rc; // eller unsigned int rc siden vi kan returnere -1;

    fil = fopen("ny_fil.txt", "r");

    // Må sjekke om filen ble initialisert eller ikke.
    if(fil == 0){
        fprintf(stderr, "Oppstod en feil ved filinnlesing");
        return EXIT_FAILURE;
    }

    /*
        Ved fread, så har vi at første parameter benyttes for bufferet som skal samle 
        inn linjer fra tekstfilen. Andre parameter beskriver hvilken størrelse som vi skal 
        lese fra tredje parameter "BUFSIZE". Tilslutt filen vi ønsker å lese fra som er da 
        "fil"-pekeren som peker på "ny_fil.txt". 
    */

    rc = fread(buf, sizeof(char), BUFSIZE - 1, fil);

    if(rc < BUFSIZE && ferror(fil)){
        fprintf(stderr, "Skjedde feil ved writecount, mindre enn forventet");
        fclose(fil);
        return EXIT_FAILURE;
    }
    buf[rc] = 0;
    printf("buf: %s\n", buf);

    char *name = "ssss";
    if(name "ssss"){
        printf("SKOLE");
    }

    fclose(fil);
    return EXIT_SUCCESS;
}
