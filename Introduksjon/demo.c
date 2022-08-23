#include <stdio.h>

int main(void){
	unsigned int b = 2147483647;
	signed int c = 2147483647;
	c++;	// overflow
	b = -1; // underflow

	//printf("b = %u, c = %d\n", b, c);
	
	// sizeof() gir oss størrelsen til en varabel, type eller struct (mer om de i en annen gruppetime) i antall bytes
	// printf("size short %ld, size long %ld\n", sizeof(short), sizeof(long));

	// Det finnes ikke en string type i c. Strings er istedet arrays med chars. Når C leser en array av chars vil den fortsette å lese helt til den treffer-
	// på en NULL byte ('\0')
	char s[10];
	s[0] = 'a';
	s[1] = 'b';
	s[2] = 'c';
	s[3] = '\0'; 	// Her legger jeg inn null byten. Hvis jeg hadde glemt denne og prøvd å printe s ville sannsynligvis helt tilfeldige ting bli printet helt-
			// til c hadde truffet på en null byte tilfeldig.
	
	char string[4] = "abc"; // Her er en enklere måte å definere en string. 
				// C legger til NULL byten automatisk når vi definerer det på denne måten, men husk å sette av nok plass i arrayet!
	
	// printf("s = %s\n", s);
	// Kort forklaring på string formatering. Inni stringen spesifiserer du placeholders med %-tegnet etterfulgt av en eller flere bokstaver. 
	// Bokstavene forteller C hva slags type data placeholderen skal erstattes med. 
	// Når stringen skal brukes vil placeholderene byttes ut med variablene du spesifiserer.
	//
	// Eksempel:
	// int i = 2;
	// long j = 4;
	// char tekst[3] = "ab"; 
	// printf("tall_1: %d tall_2: %ld string: %s \n", i, j, tekst);
	// %d står for digit og vil byttes ut med det første tallet vi spesifiserer (variabelen i), 
	// %ld står for long digit og vil byttes ut med det andre tallet vi spesifiserer (variabelen j),
	// %s står for string og vil byttes ut med char arrayen vi spesifiserte til sist (tekst). Strengen vi får printet blir "tall_1: 2 tall_2: 4 string: ab"
	

	
	int d = 2;
	int* d_ptr = &d;		// Pointers er variabler som lagrer minnelokasjoner. d_ptr vil her lagre minnelokasjonen til d integeren d. Legg merke til-
					// at vi spesifiserer at d_ptr  er en int pointer. Da sier vi til C at det som ligger på denne minnelokasjonen er en integer 
					// Man spesifiserer at man ønsker minnelokasjonen til en variabel ved å bruke &.
	//printf("d = %d, d_ptr = 0x%x\n", d, *d_ptr);  	// %x står for hexadesimal. Dette brukes typisk for minnelokasjoner og prefikses typisk med 0x

	int arr[3] = {10, 20, 30};	
	
	int* arr_ptr = arr;	// Arrays og pointers i C er samme ting! Legg merke til at jeg ikke bruker & for å få minneadressen til arr. 
				// Dette er fordi arr allerede er en pointer!
	
	// printf("arr[1]: %d, arr+4: %d\n", arr[1], *(arr+1));		// Som nevnt er pointers bare variabler som lagrer minneadresser, men du kan bruke dem til-
									// å få tak i det som ligger på den minneadressen. Dette gjør du ved å dereference pointeren.
									// Dette kan du gjøre ved å sette en * forran variabelen slik jeg gjør i printen over. 
									// Når du aksesserer en verdi fra et array gjør du også dereferencing, arr[0] er  det-
									// samme som å skrive *arr
	// printf("arr[0]: %d, *arr: %d\n", arr[0], *arr);
	
	// Når du legger til noe på en pointer vil C se på størrelsen til typen som ligger på minneadressen og sørge for at du får det neste tallet som ligger i-
	// minnet. Prøv gjerne å kommentere ut printene her og prøv å skjønne hva som skjer.
	// printf("int: %d, minneadresse: %d\n", *arr_ptr, arr_ptr);
	arr_ptr++;	
	// printf("int: %d, minneadresse: %d\n", *arr_ptr, arr_ptr);
	return 0;
}