// Trengs for fopen() og uansett for å skrive til terminalen så er "must" uansett
#include <stdio.h>
// Gjør at vi kan returnere EXIT_FAILURE
#include <stdlib.h>
#include <string.h>

int main(){

    /*
        Akkurat samme som ved "Java" hvor vi har en fil-type 
        men vi har da en peker som peker til en fil vi skal hente.
        I dette tilfelle skulle vi da skrive en fil så pekeren vil 
        peke på den nye filen vi ønsker å skrive til som er da "ny_fil.txt".
        Vi bruker da fopen("ny_fil.txt", "w") hvor vi da har innført skriveoperasjon
        "w" for den filen. 
    */
    FILE *fil;
    char* msg = "Dette er en buffered write";
    size_t wc; // eller unsigned int wc siden vi kan returnere -1;

    fil = fopen("ny_fil.txt", "w");

    // Må sjekke om filen ble initialisert eller ikke.
    if(fil == 0){
        fprintf(stderr, "Oppstod en feil ved filinnlesing");
        return EXIT_FAILURE;
    }

    /*
        fwrite() er akkurat som vi hadde i "unbuffered-write.c" med write-operasjonen
        men har enda en parameter. Første parameter benyttes for å innføre den strengen 
        som skal sendes til filen "ny_fil.txt". Andre parameter henger sammen med 
        tredje parameter i og med at, tredje parameter understreker lengden til det vi 
        ønsker å fylle inn til filen. Andre parameter understreker størrelsen til hva 
        vi skal fylle inn.
    */

    wc = fwrite(msg, sizeof(char), strlen(msg), fil);

    if(wc < strlen(msg)){
        fprintf(stderr, "Skjedde feil ved writecount, mindre enn forventet");
        fclose(fil);
        return EXIT_FAILURE;
    }

    fclose(fil);
    return EXIT_SUCCESS;
}
