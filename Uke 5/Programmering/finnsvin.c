/*
 * Løsningsforslag til oppgaven FINNSVINN fra Cbra uke 6.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

struct vare {
    int beholdning;
    int registrert_inn_ut;
    char *navn;
    struct vare *neste;
};

/*
 * En enum (enumerate). Her defineres 3 unike konstanter
 */
enum type {
    BEHOLDNING,
    LEVERING,
    SALG
};

// Global lenkeliste av varer.
struct vare *varer = NULL;

/*
 * Free'er minnet til en vare. Merk at rekkefølgen her er viktig. Frigjør vi v
 * først, kan vi ikke lenger følge v til v->navn.
 */
void free_vare(struct vare *v) {
    free(v->navn);
    free(v);
}

/*
 * Frigjør hele den globale listen av varer
 */
void free_varer() {
    struct vare *tmp, *tmp2;

    for (tmp = varer; tmp != NULL; tmp = tmp2) {
        tmp2 = tmp->neste;
        free_vare(tmp);
    }
}

/*
 * Regner ut svinn og printer vare. Siden oppgaven kun ber om at varer med svinn
 * skal printes, printer vi ikke andre varer.
 */
void print_vare(struct vare *v) {
    int svinn = v->registrert_inn_ut - v->beholdning;
    if (svinn)
        printf("svinn: %d : %s\n", svinn, v->navn);
}

/*
 * Itererer gjennom den globale listen av varer. Alle varer med svinn printes.
 */
void print_varer() {
    struct vare *tmp;

    for (tmp = varer; tmp != NULL; tmp = tmp->neste) {
        print_vare(tmp);
    }
}

/*
 * Oppretter en ny vare og legger den til i den globale lenkelisten.
 */
void lag_vare(char *navn, unsigned char antall, enum type t) {
    struct vare *v = malloc(sizeof(struct vare));

    if (v == NULL) {
        fprintf(stderr, "malloc failed. possibly out of memory\n");
        free_varer();
        exit(EXIT_FAILURE);
    }

    v->navn = strdup(navn);
    if (v->navn == NULL) {
        perror("strdup");
        free_varer();
        free(v);
        exit(EXIT_FAILURE);
    }

    v->beholdning = 0;
    v->registrert_inn_ut = 0;

    switch (t) {
        case BEHOLDNING:
            v->beholdning = antall;
            break;
        case LEVERING:
            v->registrert_inn_ut = antall;
            break;
        case SALG:
            v->registrert_inn_ut = -antall;
            break;
    }

    v->neste = varer;
    varer = v;
}

/*
 * Legger til vare i den globale lenkelisten. Dersom varen ikke finnes opprettes
 * en ny vare.
 */
void legg_til_vare(char *navn, unsigned char antall, enum type t) {
    struct vare *tmp;

    tmp = varer;
    while (tmp != NULL) {
        if (strcmp(tmp->navn, navn)) {
            tmp = tmp->neste;
            continue;
        }
        switch (t) {
            case BEHOLDNING:
                tmp->beholdning = antall;
                return;
            case LEVERING:
                tmp->registrert_inn_ut += antall;
                return;
            case SALG:
                tmp->registrert_inn_ut -= antall;
                return;
        }
        tmp = tmp->neste;
    }
    lag_vare(navn, antall, t);
}

/*
 * Leser inn .log-filtypen som er spesifisert i oppgaveteksten. Legger varer inn
 * i den globale lenkelisten.
 */
void les_log(char *filnavn, enum type t) {
    FILE *fil;
    char navn[256];
    unsigned char antall, navn_len;
    int rc;

    fil = fopen(filnavn, "rb");
    if (fil == NULL) {
        perror("fopen");
        free_varer();
        exit(EXIT_FAILURE);
    }


    while ( fread(&antall, sizeof(char), 1, fil) ) {
        if (fread(&navn_len, sizeof(char), 1, fil) == 0) break;
        rc = fread(navn, sizeof(char), navn_len, fil);
        if (rc != navn_len) break;
        navn[navn_len] = 0;
        legg_til_vare(navn, antall, t);
    }
    if (ferror(fil)) {
        fprintf(stderr, "fread failed\n");
        free_varer();
        exit(EXIT_FAILURE);
    }

    fclose(fil);
}


/*
 * Ved å modularisere programmet i funksjoner blir main-funksjonen enkel :-)
 */
int main(void) {
    les_log("salg.log", SALG);
    les_log("levering.log", LEVERING);
    les_log("beholdning.log", BEHOLDNING);
    print_varer();
    free_varer();

    return EXIT_SUCCESS;
}