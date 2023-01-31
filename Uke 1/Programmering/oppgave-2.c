 #include "shared.h"

/*
Oppgave 2.1
- Skriv en funksjon som tar inn et array (f eks med int'er), og printer ut alle elementene.
- Hvordan vet du når du har kommet til slutten av arrayet? Må funksjonen ta inn flere parametere?

void lagArray(int array[], int lengde){
    for(int i = 0; i < lengde; i++){
        printf("Array-element: %d\n", array[i]);
    }
}


int main(int argc, char const *argv[])
{
    int array[11] = {0,1,2,3,4,5,6,7,8,9,10};
    int lengde = sizeof(array) / 4;
    printf("Lengden til array er:  %d\n", lengde);
    lagArray(array, lengde);
    return 0;
}

*/

/*

Oppgave 2.2
- Opprett et char array med de verdiene du vil.
- Bruk en løkke til å gå gjennom arrayet, men bruk en peker for å aksessere 
  elementene istedenfor vanlig indeksering (altså IKKE array[i]).
- Hva skjer dersom du bruker den opprinnelige pekeren, og ikke oppretter en midlertidig peker? Hint:
  char array[] = "Marie";
  char *tmp = array;

- Her vil både array og tmp peke til 'M' (altså starten på arrayet)
- Hvordan vet løkken at enden på arrayet er nådd? Kan man slippe unna en teller? 
  Hint: hva ligger i slutten av et char-array?

int main(int argc, char const *argv[])
{
    char name[] = "Marie";
    char *ptr = name;

    while (*ptr) {
        printf("%c\n", *ptr);
        ptr++;
    }
}
*/

/*

Oppgave 2.3
- Opprett et char array (eller bruk samme som i 2.2)
- Opprett en char needle = 'a'
- Bruk en løkke til å gå gjennom arrayet, ved hjelp av en peker (ikke indeksering).
- Dersom elementet pekeren peker til er lik needle, print ut "Fant nålen!"

int main(int argc, char const *argv[])
{
    char name[] = "Marie";
    char *ptr = name;
    char needle = 'a';

    while (*ptr) {
        if(*ptr == needle){
            printf("Fant nålen");
            break;
        }
        *ptr++;
    }
}

*/

/*

Oppgave 2.4
- Gitt funksjonene:

void fill_int_array() {
    int array[10];
    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }
}

void print_int_array() {
    int array[10];
    for (int i = 0; i < 10; i++) {
        printf("%d\n", array[i]);
    }
}

int main(int argc, char const *argv[])
{
    fill_int_array();
    print_int_array();
    return 0;
}

Hva skjer om du kaller på disse rett etter hverandre i main? 
Skjedde det noe litt merkelig? Hvorfor skjedde det evt noe litt merkelig? 
Hint: Hva skjer med stacken under/etter funksjonskall

Kaller man fill_int_array og print_int_array rett etter hverandre, vil man få ut tallene 0-9! 
Grunnen til det er at i fill_int_array blir tallene 0-9 lagt på stacken i et array. 
Når funksjonen er ferdig, "poppes" all dataen fra stacken, og nye ting kan bli lagt på etterpå. 
Likevel forsvinner ikke dataen, det er ingenting som blir satt til 0. 
Dermed ligger tallene der fremdeles når vi kaller en ny funksjon, og ettersom den lager et array 
på nøyaktig samme sted (funksjonene gjør det samme), så vil arrayet være fyllt med tallene fra fill_int_array.
*/

