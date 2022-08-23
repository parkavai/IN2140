#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1.1
int strlen_kopi(const char *s) {
    int counter = 0;
    while (*s++) counter++; // så lenge det s peker på ikke er `\0` ...
    return counter;
}

// 1.2

void copy_array(char arr1[], char arr2[]) {
    do {
        *arr2++ = *arr1;
    } while (*arr1++);
}

// 2.1

/*
int main(int argc, char const *argv[]) {
    if (argc < 4) printf("Error: Trenger tre argumenter.\n");
    else printf("arg1: '%s', arg2: '%s', arg3: '%s'\n", argv[1], argv[2], argv[3]);
    return 0;
}
*/

// 2.2

/*
int main(int argc, char const *argv[]) {
    char const *argument;

    if (argc < 2) {
        printf("Error: Trenger ett argument.\n");
        return EXIT_FAILURE;
    }

    argument = argv[1];
    if (strlen(argument) > 1) {
        printf("Error: '%s' er ikke en bokstav\n", argument);
        return EXIT_FAILURE;
    }

    char b = *argument;
    b++;
    printf("%c\n", b);

    return EXIT_SUCCESS;
}
*/

// 3.2

/*
void print_size(char arr[]) {
    printf("%ld\n", sizeof(arr));
}

int main(void) {
    char array[50];
    printf("%ld\n", sizeof(array));
    print_size(array);
    return 0;
}
*/

// 4.3

/*
char *getCopy(char *original){
    char *copy = malloc(strlen(original) + 1); // gjort om
    for(int i = 0; i < strlen(original) + 1; i++){
        copy[i] = original[i];
    }
    return copy;
}

int main() {
    char *string = "Alle Cbraer vil ha kopier!";
    char *stringkopi = getCopy(string);
    free(stringkopi); // lagt til
    return 0;
}
*/

// 4.5

/*
int *gangetabell(int base) {
    int i, *tabell;

    tabell = malloc(10 * sizeof(int));
    for (i = 1; i <= 10; i++) {
        tabell[i-1] = base * i;
    }

    return tabell;
}

int main(void) {
    int i, j, *tabell, baser[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 42, 1337};

    for (i = 0; i < 12; i++) {
        tabell = gangetabell(baser[i]);
        printf("Tabell med base %d:\n", baser[i]);
        for (j = 0; j < 10; j++) {
            printf("%d ", tabell[j]);
        }
        printf("\n");
        free(tabell);
    }
}

*/

// 4.6

/*
int *copy_array(int array[], int length) {
    int *copy = malloc(sizeof(int) * length);
    memcpy(copy, array, length * sizeof(int));
    return copy;
}

int main(int argc, char const *argv[]) {
    int original[] = {1, 2, 3, 4, 5, 6, 7};
    int *copy = copy_array(original, 7);

    for (int i = 0; i < 7; i++) {
        printf("%d\n", copy[i]);
    }

    free(copy);
    return 0;
}
*/
