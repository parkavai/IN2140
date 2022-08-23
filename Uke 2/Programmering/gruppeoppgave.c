#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFSIZE 256


// Ta in {Username, countryCode, Postnumber}
// Verify postnumber is in range 0-9999
// verify countryCode == 2 len
// print it


void debugPrint(char* username, char* countryCode, char* postNumber);
int verifyCountryCode(char* countryCode);
int verifyPostNumber(char* postNumber);
void writeOut(char* username, char* countryCode, char* postNumber, char* writeBuffer);

int main(int argc, char** argv) {

    if(argc != 4) {

        fprintf(stderr, "Arguments illegal. Got %d expected 4\n", argc);
        fprintf(stderr, "Format: Username, CountryCode, Addresscode (0-9999)\n");
        exit(EXIT_FAILURE);

    }

    char* username = argv[1];
    char* countryCode = argv[2];
    char* postNumber = argv[3];

    if(!verifyCountryCode(countryCode)) {

        fprintf(stderr, "Illegal country code, must be two letters\n");
        exit(EXIT_FAILURE);

    }


    if(!verifyPostNumber(postNumber)) {


        fprintf(stderr, "Illegal postnumber, must be greater than -1, less than 10000\n");
        exit(EXIT_FAILURE);

    }


    char writeBuffer[BUFSIZE];

    debugPrint(username, countryCode, postNumber);
    writeOut(username, countryCode, postNumber, writeBuffer);

    return EXIT_SUCCESS;

}

int verifyCountryCode(char* countryCode) {

    if(strlen(countryCode) != 2) {

        return 0;
    }

    return 1;

}


int verifyPostNumber(char* postNumber) {

    int intPostNumber = atoi(postNumber);

    if(intPostNumber > 9999 || intPostNumber < 0) {

        return 0;
    }

    return 1;

}

void writeOut(char* username, char* countryCode, char* postNumber, char* writeBuffer) {

    int writeSize = strlen(username)+strlen(countryCode)+strlen(postNumber)+1;

    if(writeSize > BUFSIZE) {

        fprintf(stderr, "Data too big to write out. Size: %d\n, BUFSIZE: %d\n", writeSize, BUFSIZE);
        fprintf(stderr, "I heard about this malloc thing. I wonder what it is....\n");
        exit(EXIT_FAILURE);

    }

    int writeStart = 0;

    for(int i = 0; i < strlen(username); i++) {

        writeBuffer[writeStart++] = username[i];

    }

    for(int i = 0; i < strlen(countryCode); i++) {

        writeBuffer[writeStart++] = countryCode[i];

    }


    for(int i = 0; i < strlen(postNumber); i++) {

        writeBuffer[writeStart++] = postNumber[i];

    }


    writeBuffer[writeStart] = '\0';

    printf("String is %s", writeBuffer);

}

void debugPrint(char* username, char* countryCode, char* postNumber) {

    printf("\nUsername is %s\n Country code is %s\n Post Number %s\n", username, countryCode, postNumber);

}