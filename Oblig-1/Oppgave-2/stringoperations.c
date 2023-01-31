#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


int stringsum(char *s){

    // Bruker ASCII verdiene til de foerste bokstavene i alfabetet 
    // Soekte paa nettet for aa finne ASCII-verdiene for bokstavene,
    // baade smaae og store bokstaver. 
    int lowercase_nedre_grense = 96;
    int lowercase_ovre_grense = 123;
    int uppercase_nedre_grense = 64;
    int uppercase_ovre_grense = 91;


    int lengde = strlen(s);
    int strengSum = 0;
    char mellomrom = ' ';
    char* adresseMellomrom = &mellomrom;

    for(int i = 0; i < lengde; i++){

        // Sjekker om "char" i strengen faktisk er en bokstav(spesifikt stor bokstav) og ikke noe tall
        if(s[i] > uppercase_nedre_grense && s[i] < uppercase_ovre_grense){
                int asciiverdi = s[i];
                strengSum += (asciiverdi - uppercase_nedre_grense);
        }

        // Sjekker om "char" i strengen faktisk er en bokstav(spesifikt smaa bokstav) og ikke noe tall
        else if(s[i] > lowercase_nedre_grense && s[i] < lowercase_ovre_grense){
                int asciiverdi = s[i];
                strengSum += (asciiverdi - lowercase_nedre_grense);
        }

        // Hvis vi har et mellomrom, skal vi ignorere og fortsette over til neste iterasjon
        else if(s[i] == *adresseMellomrom){
            continue;
        }

        // Returnerer -1 siden vi fant et tall
        else{
            return -1;
        }

    }
    return strengSum;
}

void stringsum2(char *s, int *res){
    int strengSum = stringsum(s);
    /*
    Foerst saa tenkte jeg at, vi maa hente minneadressen til "strengSum" ettersom vi jobber med pekere.
    Da lagde jeg en midlertidig peker som jeg kalte "pekerSum" som skal da holde paa "strengSum" sin minneadresse.
    Deretter benyttet jeg av "dereferencing" til aa hente verdien til "strengSum" gjennom "pekerSum". Da setter 
    vi "*res" til aa peke paa verdien til "strengSum" gjennom "*pekerSum" paa grunn av "dereferencing". 
    */
    int* pekerSum = &strengSum;
    *res = *pekerSum;
}

int distance_between(char *s, char c){
    int lengde = strlen(s);
    int low = 0;
    int high = lengde;

    int foerste = 0;
    int siste = 0;
    int forekomster = 0;

    while (low <= high){
        
        if(foerste == 0){

            // Henter den foerste forekomsten av bokstaven sett fra venstre siden
            if(s[low] == c){
                foerste = low;
                forekomster ++;
            }
        }
        if(siste == 0){

            // Henter den foerste forekomsten av bokstaven sett fra hoyre siden
            if(s[high] == c){
                siste = high;
                forekomster ++;
            }
        }

        // Stopper soeket naar vi har funnet foerste og siste forekomst av bokstaven i setningen
        if(foerste != 0 && siste != 0){
            break;
        }
        low ++;
        high --;
    }

    // Foelger oppgaveteksten hvor en forekomst av bokstaven tilsvarer en returverdi paa 0
    if(forekomster == 1){
        return 0;
    }
    // Hvis ingen forekomst saa returnerer vi -1
    else if(forekomster == 0){
        return -1;
    }
    // Ellers returneres distansen mellom siste og foerste forekomst av bokstaven
    else{
        return (siste-foerste);
    }
}

char *string_between(char *s, char c){
    int avstandMellom = distance_between(s, c);
    // Hvis bokstaven kom kun 1 gang i setningen, returneres en tom streng som vi har allokert til heapen
    if(avstandMellom == 0){
        return strdup("");
    }
    else if(avstandMellom == -1){
        return NULL;
    }
    int nyLengde = avstandMellom - 1;

    /*
        Lager et buffer som jeg allokerer minnet til, hvor jeg
        overfoerer bokstavene mellom foerste og siste forekomst 
        av bokstaven "c".
    */
    char* nySetning = malloc(nyLengde+1);
    if(nySetning == NULL){
        fprintf(stderr, "Skjedde feil ved Malloc");
        exit(1);
    }
    // Huske aa sette nullbyte
    nySetning[nyLengde] = '\0';

    // Dette er for aa finne foerste instans av bokstaven igjen. 
    int i = 0;
    while(s[i] != c){
        i++;
    }
    int foerste = i;
    for(int i = 0; i < nyLengde; i++){
        foerste ++;
        nySetning[i] = s[foerste];
    }
    // Siden test-filen frigjoer bufferet som blir returnert, saa slipper jeg aa haandtere dette
    return nySetning;
}  
