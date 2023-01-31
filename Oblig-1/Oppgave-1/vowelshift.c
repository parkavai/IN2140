// Nodvendige biblioteker aa inkludere for kjoering av diverse funksjoner
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Main-funksjon hvor vi har to parametere som indikerer inntasting i terminalen 
int main(int argc, char** argv)
{
    // Sjekker for sikkerhetsskyld om bruker ikke har tastet inn mer enn 3 argumenter(./vokalshift er ogsaa et argument)
    if(argc != 3){
        fprintf(stderr, "Husk aa taste inn 2 argumenter, en setning og et bokstav");
        return 1;
    }

    printf("Bruker tastet inn setningen: '%s' og bokstaven: '%s' \n ", argv[1], argv[2]);
    // Henter lengden til setningen som ble skrevet
    int lengde = strlen(argv[1]);

    // Allokerer plass for setningen som ble skrevet
    char *setning = malloc(lengde+1);

    /* 
    Ved bruk av malloc, er det viktig aa sjekke om malloc gav oss minnet eller ikke.
    Da skrives ut en passende melding om minnet ikke var tilgjengelig.  
    */
    if (setning == NULL){
        fprintf(stderr, "Skjedde feil ved Malloc");
        return 1;
    }

    // Setter inn en nullbyte for aa ha et sluttpunkt ved setningen
    setning[lengde] = '\0';

    /* 
    Bruker for-loop til aa sette inn verdier fra setningen bruker tastet inn og sjekker om vi har noen vokaler
    i setningen. Hvis det er vokaler i setningen saa skal disse erstattes med bokstaven som ble oppgitt av bruker. 
    */
    for(int i = 0; i < lengde; i++){
        setning[i] = argv[1][i];
        if(setning[i] == 'a' || setning[i] == 'e' || setning[i] == 'i' || setning[i] == 'o' || setning[i] == 'u' || setning[i] == 'y'){
              setning[i] = *argv[2];
        }
    }
    printf("Den modifiserte versjonen ser slik ut: %s \n", setning);

    // Viktig aa frigjoere minnet naar vi er ferdig med programmet
    free(setning);
    return 0;
}
