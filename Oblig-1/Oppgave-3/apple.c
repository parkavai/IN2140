#include "the_apple.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int locateworm(char* apple){
    int i = 0;

    // Kjoerer saa lenge vi ikke har kommet til foerste forekomsten av "worm" i strengen. 
    while(apple[i] != 'w'){

        /*
        Gjoer sjekket under paa grunn av maaten jeg loeste 3 d.), hvor jeg da anvender av denne 
        funksjonen. For naar vi foerst kaller paa removeWorm saa er "worm" fjernet fra strengen
        hvor vi da har overskrevet dette med mellomrom. Derfor returnerer jeg 0 for dette tilfellet
        "worm" ikke er i strengen lenger. 
        */

        if(apple[i] == 0){
            return 0;
        }
        i++;
    }
    return i;
}

int removeworm(char* apple){
    // Henter foerste forekomst av "worm" i strengen "apple" 
    int startPunkt = locateworm(apple);

    // Hvis ormen var fjernet, skal vi returnere 0
    if(startPunkt == 0){
        return 0;
    }
    else{
        int i = strlen(apple);
        /* 
        Itererer bakfra, slik at soeket blir kortere og my enklere. Det blir enklere siden oppgaveteksten 
        skrev at ormen kunne duplisere seg. Da vil vi hente siste instans av "m" i f.eks "wwwwwoooooooorrrrrmmmmmmm". 
        */
        while(apple[i] != 'm'){
            i--;
        }
        int endepunkt = i;
        int lengde = endepunkt - startPunkt;

        // Overskriver "worm" med mellomrom
        for(int i = 0; i <= lengde; i++){
            apple[startPunkt] = ' ';
            startPunkt++;
        }
        // "0"-indekser er ogsaa inkludert i "c" saa derfor oekes lengden med "1".
        return lengde+1;
    }
}

int main()
{  
    int orm = locateworm(apple);
    printf("Ormen ligger i posisjon: %d \n \n", orm);
    /*
    Oppgave 3 d.) var lur ettersom vi hadde med string-literals og jobbe med. Problemet med 
    disse er at vi ikke kan endre verdier gjennom "apple" siden string-literals er read-only. 
    Disse kan altsaa ikke skrives om, saa derfor tenkte jeg aa lage et buffer hvor jeg allokerer plass 
    og bruker denne som en midlertidig-peker. Brukte strlen() for aa tilordne lengde til bufferet,
    gjoer det enklere fremfor aa iterere igjennom hele "apple", for aa finne lengden.
    */
    int lengde = strlen(apple);
    char* nyVersjonEple = malloc(lengde+1);
    if(nyVersjonEple == NULL){
        fprintf(stderr, "Skjedde feil ved Malloc");
        exit(-1);
    }
    nyVersjonEple[lengde] = '\0';

    // Overfoerer char-bokstaver fra strengen "apple" over til dne midlertidige pekeren. 
    for(int i = 0; i < lengde; i++){
        nyVersjonEple[i] = apple[i];
    }

    // Foer ormen er fjernet
    printf("Foer ormen er fjernet, \n%s", nyVersjonEple);
    printf("\n");
    lengde = removeworm(nyVersjonEple);

    // Naa vil "char * apple" peke paa den nye versjonen av strengen som vi nettopp endret. 
    apple = nyVersjonEple;

    // Etter at ormen er fjernet
    printf("Etter at ormen er fjernet, \n %s", nyVersjonEple);
    printf("\n");
    printf("Ormen hadde lengde: %d \n \n", lengde);
    lengde = removeworm(apple);
    
    // Forsikre om at vi skal returnere lengde 0 siden ormen er blitt fjernet i strengen. 
    printf("Kaller paa 'removeworm' igjen,\n");
    printf("Siden ormen er fjernet, skal lengden bli: %d \n", lengde);

    // Frigjoer minnet tilslutt
    free(nyVersjonEple);
    return 0;
}
