# include <stdio.h>
# include "shared.h"

/*
Oppgave 1.1
- Skriv en main-funksjon som skriver ut "Hello world!" til terminalen.

- Skriv en funksjon print_hello() som gjør det samme. Kall denne funksjonen fra main.

- Er det noen forskjell på om print_hello() ligger over/under main?
Skrives funksjonen under main, så vil ikke programmet kjøre. Derfor skal funksjonsdeklarasjon
foregå alltid over main, aldri under!!!

void print_hello(){
    printf("Hello world!");
}

int main(int argc, char const *argv[])
{
    printf("Hello world! \n");
    print_hello();

    return 0;
}

*/

/*
Oppgave 1.4

- Deklarer et stort array, uten å initialisere det (uten å "fylle" det). 
- Print det ut med en vanlig for-løkke. 
- Ble utskriften som du forventet? 

- Hvorfor/hvorfor ikke?
Det ser ut som at tilfeldige tall blir lagret inni arrayen hvis man ikke tilordner 
verdier inn i arrayen. Noen er null, andre er negative tall, positive tall, 1 osv. 

int main(int argc, char const *argv[])
{
    int array[100];
    
    for (int i = 0; i < 100; i++){
        printf("Array-element: %d\n", array[i]);
    }
    return 0;
}
*/

/*
Oppgave 1.5

- Deklarer et int array a, og initialiser det med verdier fra 0 til 9 (på hvilken måte du vil).
- Endre verdi på plass 5 til 1337.
- Print ut arrayet.

int main(int argc, char const *argv[])
{
    // int array[] = {0,1,2,3,4,5,6,7,8,9}
    int array[9];
    for (int i = 0; i <= 9; i++){
        array[i] = i;
    }
    array[5] = 1337;
    
    for (int i = 0; i <= 9; i++){
        printf("Array-element: %d\n", array[i]);
    }
    return 0;
}

*/

/*
Oppgave 1.6
- Deklarer og initialiser et char-array navn (f eks med navnet ditt).
- Deklarer et annet char-array kopi (initialiser det om du vil, f eks med 0).
- Kopier det som ligger i navn over i kopi på to måter. Hint: man strcpy

int main(int argc, char const *argv[])
{   

//  Kopi-måte 1
    char name[] = "Harry";
    char kopi[5];
    for (int i = 0; i <= strlen(name); i++){
        kopi[i] = name[i]
    }

//  Kopi-måte 2
    strcpy(kopi, navn);
}
*/

/*
Oppgave 1.7
- Opprett en int a = 5
- Opprett en int-peker b som peker på a
- Print ut både a og b (målet er å få ut 5 begge gangene). Hint: hvordan "følge" en peker dit den peker? Dereferencing

int main(int argc, char const *argv[])
{   
    int a = 5;
    int *b = &a;
    printf("b-elementet: %d\n", *b);
}

*/

/*
Oppgave 1.8
- Skriv en funksjon som tar inn en variabel (f eks et tall), og endrer den (f eks legger til 5 på verdien)
- Målet er at variabelen som sendes inn skal bli endret, uten at funksjonen returnerer noe. 
- Hint: Hva slags type må variabelen være? Kan den f eks bare være en int? 

void endreTallVerdi(int *a){
    *a = 8;
}

int main(int argc, char const *argv[])
{   
    int a = 4;
    endreTallVerdi(&a);
    printf("Tallverdi: %d", a);
}

*/

int main(int argc, char const *argv[])
{   
    char* test = "Hei";
    int i = 0;
    char liste[4];
    while(test[i] != 0){
        liste[i] = test[i];
        i++;
    }
    liste[2] = 's';
    printf("%s", liste);
    
    return 0;
}

