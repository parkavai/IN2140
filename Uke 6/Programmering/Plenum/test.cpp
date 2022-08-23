#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h> 


#define BUFSIZE 256

int main(){
    /*
    int tall = 28000/4096;
    printf("Tall: %d", tall);
    int lengde = tall + 1;
    int *array = (int *) malloc(sizeof(int) * tall);
    array[4] = 1;
    printf("Nummer 10: %d\n", array[3]);
    free(array);
    for(int i = 0; i <= tall; i ++){
        printf("Indeks: %d\n", i);
    }
    */

    FILE *fil = fopen("superblock", "r");
    if(fil == 0){
        fprintf(stderr, "Oppstod feil ved lesing av superblokk");
        return NULL;
    }

    int ret = 0;
    int antNoder = 0;
    int id;
    while(fread(&id, sizeof(int), 1, fil) == 1){
        printf("ID: %d\n", id);
        size_t* entries;

        // Les inn navnets-lengde
        int name_length;
        ret = fread(&name_length, sizeof(int), 1, fil);
        if (ret == -1) {
            perror("Oppstod feil ved lesing av navnelengde");
            return NULL;
        }
        printf("name_len: %d\n", name_length);

        // Les inn navnet
        char* name = (char*) malloc(name_length);
        if(name == NULL){
            free(name);
            fprintf(stderr, "Oppstod feil ved allokering av bytes til navn");
            return NULL;
        }
        ret = fread(name, 1, name_length, fil);
        if (ret == -1) {
            perror("Oppstod feil ved lesing av navn");
            return NULL;
        }
        printf("name: %s\n", name);

        // Les inn is_directory
        char is_directory;
        ret = fread(&is_directory, 1, 1, fil);
        if (ret == -1) {
            perror("Oppstod feil ved lesing av is_directory");
            return NULL;
        }
        printf("is_directory: %d\n", is_directory);
        
        // Les inn is_readonly
        char is_readonly;
        ret = fread(&is_readonly, 1, 1, fil);
        if (ret == -1) {
            perror("Oppstod feil ved lesing av is_readonly");
            return NULL;
        }
        printf("is_read_only: %d\n", is_readonly);
        
        // Les inn filstørrelse
        int filesize;
        ret = fread(&filesize, sizeof(int), 1, fil);
        if (ret == -1) {
            perror("Oppstod feil ved lesing av filsizen");
            return NULL;
        }
        printf("filsize: %d\n", filesize);

        // Les inn num_entries
        int num_entries;
        ret = fread(&num_entries, sizeof(int), 1, fil);
        printf("num_entries: %d\n", num_entries);
        if (ret == -1) {
            perror("Oppstod feil ved lesing av num_entries");
            return NULL;
        }

        entries = (size_t *) malloc(sizeof(size_t) * num_entries);
        
        for(int i = 0; i < num_entries; i++){
                size_t* ider;
                ret = fread(&ider, 8, 1, fil);
                if (ret == -1) {
                    fprintf(stderr, "Har nådd bunnpunkt");
                    return NULL;
                }
                entries[i] = (size_t) ider;
                
        }
        size_t test = entries[0];
        printf("entries-id: %d", test);
        printf("\n");
        antNoder++;
    }
    printf("Antall noder: %d", antNoder);
}
