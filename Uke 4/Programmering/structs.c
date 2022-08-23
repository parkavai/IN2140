#include <stdio.h>
#include <stdlib.h>

struct bil {
    int mil;
    char* farge = malloc(12);
};

// Når du lager structs, lag en funksjon som oppfører seg som en konstruktør.
// Gjør jobben mye enklere fremfor å gjøre alt i main, samt det sees finere ut. 
void construct_bil(struct bil *b, int mil, char* farge){
    b->mil = mil;
    b->farge = farge;
}

void freeBil(struct bil *b){
    free(b->farge);
    free(b);
}

int main(){

    // Fungerer som ved objekter i python eller java lager instans av 
    // structen og maa huske aa fylle inn verdier for structen som er blitt lagd
    /*
    struct bil minBil;
    minBil.mil = 10;
    minBil.farge = "rød";
    printf("mil: %d\n farge: %s\n", minBil.mil, minBil.farge);
    */

    /* 
    Ved bruk av konstruktør-funksjonen
    Viktig å allokere plass til heapen siden vi har bilpeker
    og denne bilpekeren må peke til en minneplass hvor structen vår ligger
    */
    struct bil* minBil = malloc(sizeof(struct bil));
    construct_bil(&minBil, 15, "rød");

    printf("mil: %d, farge: %s\n", minBil->mil, minBil->farge);

    freeBil(minBil);
    return 0;



}