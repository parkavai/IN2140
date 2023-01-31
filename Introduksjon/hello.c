#include <stdio.h>

int main(void){
    //ifi:
    // size of (Bytes) char 1, short 2, int 4, long 8
    // men i c er vi bare garantert min verdier og at char = 1 og char <= short <= int <= long

    printf("size of char = %ld\n", sizeof(char));


    // minne er bare et tall, vi bestemmer hvordan vi tolker det
    char a = 'a'; //ascii verdi 97 så indentisk som 97
    char b = 97; 

    printf("a = %d\n", a); //tolker som tall
    printf("b = %c\n", b); //tolker som ascii

    char x = 0b11111111;  //0b = binært, 0x = hex 
    unsigned char y = x;

    //singed = først bit er 2er kompliment
    //unsigned = bruker hele tallet så ingen negative verdier
    printf("signed: %d\n", x);
    printf("signed: %u\n", y);


    char string2[5];
    char string[] = "Hello";

    string2[0] = 'h';
    string2[1] = 'e';
    string2[2] = 'l';
    string2[3] = 'l';
    string2[4] = 'o';

    printf("%s\n", string2); // vi vet ikke hva som skjer fordi vi ikke har en terminernde null byte så vi printer til vi kommer til 0 (eller kræsjer)
    printf("%s\n", string);

    int ab; //skaper minnefeil når vi bruker den uten å gi den en verdi først
    printf("%d\n", ab);

    return 0;
}