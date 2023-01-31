#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum borek_type{
    SPINAT,
    POTET,
    KYLLING,
    SOPP
};

/*
    Foer saa var enum satt i mellom de andre variablene og dette er 
    daarlig med tanke paa alignment. Årsaken er at enum benytter 
    4 bytes og da vil en char og deretter en enum, ville da ført 
    til mis-alignment. Dermed vil det ta lengre aksesstid ettersom 
    enum ikke er alignet riktig. Derfor er enum satt opp helt i 
    toppen. 
*/
typedef struct borek{
    enum borek_type type;
    char pris;
    char b_len;
    char* beskrivelse;
} borek;

borek* make_borek(char pris, enum borek_type type, char* b){
    borek* borek = malloc(sizeof(borek));
    borek->pris = pris;
    borek->type = type;
    borek->b_len = strlen(b);
    borek->beskrivelse = b;
    return borek;
}

void write_borek(borek* b, FILE* fp){
    fwrite(b, sizeof(borek)-sizeof(char*), 1, fp);
    fwrite(b->beskrivelse, b->b_len, 1, fp);
}

int read_borek(borek* b, FILE* fp){
    int antBytesFraFil = fread(b, sizeof(borek)-sizeof(char*), 1, fp);
    if(!antBytesFraFil) return antBytesFraFil; // Returnerer null
    b->beskrivelse = malloc(b->b_len);
    antBytesFraFil = fread(b->beskrivelse, b->b_len, 1, fp);
    return antBytesFraFil;
}

void free_borek(borek* b){
    free(b->beskrivelse);
    free(b);
}

int main(){
    // prosess 1
    FILE* fp = fopen("struct_file.txt", "w");
    if(fp == NULL){
        perror("Kunne ikke åpne fil");
        exit(EXIT_FAILURE);
    }
    char* beskrivelse = malloc(50);
    strcpy(beskrivelse, "Børek med spinat. Veldig god");
    borek* spinat_borek = make_borek(25, SPINAT, beskrivelse);
    write_borek(spinat_borek, fp);
    free_borek(spinat_borek);
    fclose(fp);

    // prosess 2
    FILE* read_fp = fopen("struct_file.txt", "r");
    if(read_fp == NULL){
        perror("Kunne ikke åpne fil");
        exit(EXIT_FAILURE);
    }
    borek lest_borek;
    read_borek(&lest_borek, read_fp);
    printf("Pris: %d, type: %d, b_len: %d\n%s\n", lest_borek.pris, lest_borek.type, lest_borek.b_len, lest_borek.beskrivelse);
    return 0;
}