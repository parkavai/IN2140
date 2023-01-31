#include "allocation.h"
#include "inode.h"

#include <errno.h>

#define BLOCKSIZE 4096

// Benyttes for ID
int teller = -1;

// Hjelpefunksjon for å tilordne id
int hentId(){
    return teller += 1;
}

// Å legge til inode inn i directories
void leggTilInode(struct inode* directory, struct inode* peker, int indeks){
    directory->entries[indeks] = (size_t) peker;
    directory->num_entries += 1;
    directory->entries = (size_t *) realloc(directory->entries, sizeof(struct inode) * directory->num_entries);
}

// Hjelpefunksjon for å lage inode
struct inode* makeFile(char *name, char is_readonly, int filesize){
    struct inode *file = (struct inode *) malloc(sizeof(struct inode));
    if(!file){
        fprintf(stderr, "Oppstod feil ved allokering til fil");
        free(file);
        free(name);
        return NULL;
    }
    /*
        Tilordner verdier for feltene for inode. Vet at is_directory 
        automatisk er NULL, men gjør dette for å holde oversikt.  
    */
    file->id = hentId();
    file->name_len = strlen(name);
    file->name = strdup(name);
    file->is_readonly = is_readonly;
    file->is_directory = 0;
    file->filesize = filesize;

    int antBlokker = (filesize/BLOCKSIZE) + 1;
    int* array = (int *) malloc(sizeof(int *) * antBlokker);
    file->num_entries = antBlokker;
    if(!array){
        fprintf(stderr, "Oppstod feil ved allokering til entries i directory");
        free(file);
        free(array);
        free(name);
        return NULL;
    } 
    file->entries = (size_t *) array;
    return file;
}

// Fyller inn diskblokker for filer
void fyll_inn_blokker(struct inode* fil){
    for(int i = 0; i < fil->num_entries; i++){
        int blokkIndeks = allocate_block();
        if(blokkIndeks == -1){
            break;
        }
        fil->entries[i] = blokkIndeks;
    }
}

/*
Funksjonen tar som parameter en peker til inoden til katalogen som skal inneholde den nye filen. 
Innenfor denne katalogen må navnet være unikt. Dersom det finnes en fil med samme navn der fra 
før, så skal funksjonen returnere NULL uten å gjøre noe.
*/
struct inode* create_file(struct inode* parent, char* name, char readonly, int size_in_bytes )
{   
    struct inode* file = makeFile(name, readonly, size_in_bytes);
    if(file == NULL){
        return NULL; 
    }
    int i = 0; 
    if(parent->num_entries == 0){
        fyll_inn_blokker(file);
        leggTilInode(parent, file, i);
        return file;
    }
    // Iterer gjennom alle filene til katalogen
    while(i != parent->num_entries){
        struct inode *peker = (struct inode*) parent->entries[i];
        // Hvis vi har en fil med det samme navnet, går vi ut av funksjonen
        if(strcmp(peker->name, file->name) == 0){
            free(file->name);
            free(file);
            return NULL;
        }
        i += 1;
    }
    fyll_inn_blokker(file);
    leggTilInode(parent, file, i);
    return file;
}

// Hjelpefunksjon for å lage inode
struct inode* makeDirectory(char *name){
    struct inode* directory = (struct inode *) malloc(sizeof(struct inode));

    if(directory == NULL){
        fprintf(stderr, "Oppstod feil ved allokering til directory");
        free(directory);
        return NULL;
    }

    directory->id = hentId();
    directory->name_len = strlen(name);
    directory->is_readonly = 0;
    directory->is_directory = 1;
    directory->filesize = 0;
    directory->name = strdup(name);
    directory->num_entries = 0;

    struct inode *array = (struct inode *) malloc(sizeof(struct inode));
    if(!array){
        fprintf(stderr, "Oppstod feil ved allokering til entries i directory");
        free(array);
        free(name);
        return NULL;
    }
    directory->entries = (size_t *) array;
    return directory; 
}

/*
Funksjonen tar som parameter inoden til katalogen som skal inneholde den nye katalogen. 
Innenfor denne katalogen må navnet være unikt. Dersom det finnes en katalog med samme navn der 
fra før, så skal funksjonen returnere NULL uten å gjøre noe.
*/
struct inode* create_dir(struct inode* parent, char* name)
{
    struct inode* directory = makeDirectory(name);
    int i = 0;
    if(directory == NULL){
        return NULL;
    }

    // Sjekker tilfelle for rotnoden eller om katalogen er tom
    if(parent != NULL){
        if(parent->num_entries == 0){
            leggTilInode(parent, directory, i);
            return directory;
        }
        while(i != parent->num_entries){
            struct inode *peker = (struct inode*) parent->entries[i];
                // Hvis vi har en mappe med det samme navnet, går vi ut av funksjonen
                if(strcmp(peker->name,directory->name) == 0){
                    free(directory);
                    return NULL;
                }
                i += 1;
            }
        leggTilInode(parent, directory, i);
    }
    // print_inode(directory);
    return directory;
}

/*
Funksjonen sjekker alle inoder som refereres direkte fra foreldrenoden. 
Hvis én av dem har navnet name, så returnerer funksjonen dens inodepeker. 
Parent må peke på en kataloginode. Dersom ingen slik inode finnes, så returnerer funksjonen NULL.
*/
struct inode* find_inode_by_name(struct inode* parent, char* name )
{
    for(int i = 0; i < parent->num_entries; i++){
        struct inode* peker = (struct inode*) parent->entries[i];
        if(strcmp(peker->name, name) == 0){
            return peker;
        }
    }
    return NULL;
}

// Iterer gjennom alle noder og henter ut pekeren med korresponderende id som parent skal inneholde
struct inode* find_inode_by_id(int id, struct inode** nodeBeholder){
    int i = 0;
    while(nodeBeholder[i] != NULL){
        if(nodeBeholder[i]->id == id){
                return nodeBeholder[i];
        }
        i++;
    }
}

/*
Tilordne pekere til directories
*/
void tilordnePekere(struct inode* parent, struct inode** beholderAvInoder){
    size_t *entries = parent->entries;
    struct inode* array = (struct inode *) malloc(sizeof(struct inode) * parent->num_entries);
    parent->entries = (size_t *) array;
    int writeIdx = 0;
    for(int i = 0; i < parent->num_entries; i++){
        size_t id = entries[i];
        struct inode* child = find_inode_by_id(id, beholderAvInoder);
        parent->entries[writeIdx] = (size_t) child; 
        writeIdx++;
    }
    free(entries);
}

struct inode* load_inodes()
{
    /*
    Funksjonen leser superblokkfilen og oppretter i minnet en inode for hver tilsvarende oppføring i filen. 
    Funksjonen setter pekere mellom inodene på riktig vis. Superblokkfilen forblir uforandret.
    */
    int ret = 0;

    FILE *fil = fopen("superblock", "r");
    if(fil == 0){
        fprintf(stderr, "Oppstod feil ved lesing av superblokk");
        return NULL;
    }

    struct inode** beholderAvInoder = (struct inode *) malloc(sizeof(struct inode));
    if(beholderAvInoder == NULL){
        free(beholderAvInoder);
        return NULL;
    }

    int antNoder = 0;
    int id;
    while(fread(&id, sizeof(int), 1, fil) == 1){
        struct inode* node = (struct inode *) malloc(sizeof(struct inode));
        size_t* entries;

        node->id = id;

        // Les inn navnets-lengde
        int name_length;
        ret = fread(&name_length, sizeof(int), 1, fil);
        if (ret == -1) {
            fprintf(stderr, "Oppstod feil ved lesing av navnelengde");
            return NULL;
        }
        node->name_len = name_length;

        // Les inn navnet
        char* name = (char*) malloc(name_length);
        if(name == NULL){
            free(name);
            fprintf(stderr, "Oppstod feil ved allokering av bytes til navn");
            return NULL;
        }
        ret = fread(name, 1, name_length, fil);
        if (ret == -1) {
            fprintf(stderr, "Oppstod feil ved lesing av navn");
            return NULL;
        }
        node->name = strdup(name);
        free(name);

        // Les inn is_directory
        char is_directory;
        ret = fread(&is_directory, 1, 1, fil);
        if (ret == -1) {
            fprintf(stderr, "Oppstod feil ved lesing av is_directory");
            return NULL;
        }
        node->is_directory = is_directory;
        
        // Les inn is_readonly
        char is_readonly;
        ret = fread(&is_readonly, 1, 1, fil);
        if (ret == -1) {
            fprintf(stderr, "Oppstod feil ved lesing av is_readonly");
            return NULL;
        }
        node->is_readonly = is_readonly;

        // Les inn filstørrelse
        int filesize;
        ret = fread(&filesize, sizeof(int), 1, fil);
        if (ret == -1) {
            fprintf(stderr, "Oppstod feil ved lesing av filsizen");
            return NULL;
        }
        node->filesize = filesize;

        // Les inn num_entries
        int num_entries;
        ret = fread(&num_entries, sizeof(int), 1, fil);
        if (ret == -1) {
            fprintf(stderr, "Oppstod feil ved lesing av num_entries");
            return NULL;
        }
        node->num_entries = num_entries;
        
        // Får ikke kjørt programmet videre på grunn "hosts" og det store num_entries verdi. 
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
        node->entries = entries;
        beholderAvInoder[antNoder++] = node;
        beholderAvInoder = realloc(beholderAvInoder, sizeof(struct inode) * antNoder);
    }
    int i = 0;
    // Iterer igjennom alle inodene fra beholderen vår
    while (i != antNoder){

        struct inode* peker = beholderAvInoder[i];
        int indeks = i;
        // Hvis det er en mappe, så skal vi sette inn de filene/mappene som tilhører den gitte mappen
        if(peker->is_directory == 1){
            tilordnePekere(peker, beholderAvInoder);
        }
        // Hvis det er en fil så fyller vi inn diskblokkene til filene
        else{

            for(int i = 0; i < peker->num_entries; i++){
                int blokk = allocate_block();
                if(blokk == -1){
                    break;
                }   
                if((int) peker->entries[i] == blokk){
                    peker->entries[i] = blokk;
                }
            }
        }
        i++;
    }
    struct inode* rot = beholderAvInoder[0];
    i = 0;
    free(beholderAvInoder);
    fclose(fil);
    return rot;
}

void fs_shutdown( struct inode* inode )
{
    if(!inode->is_directory){

        for(int i = 0; i < inode->num_entries; i++){
            free_block(inode->entries[i]);
        }

        free(inode->entries);
        free(inode->name);
        free(inode);
        return;

    }
    else{
        if(inode != NULL){

            for(int i = 0; i < inode->num_entries; i++){

                if(inode != NULL){
                    fs_shutdown((struct inode *) inode->entries[i]);
                }

            }

            free(inode->name);
            free(inode->entries);
            free(inode);
            inode = NULL;
        }
    }
}

/* This static variable is used to change the indentation while debug_fs
 * is walking through the tree of inodes and prints information.
 */
static int indent = 0;

void debug_fs( struct inode* node )
{
    if( node == NULL ) return;
    for( int i=0; i<indent; i++ )
        printf("  ");
    if( node->is_directory )
    {
        printf("%s (id %d)\n", node->name, node->id );
        indent++;
        for( int i=0; i<node->num_entries; i++ )
        {
            struct inode* child = (struct inode*)node->entries[i];
            debug_fs( child );
        }
        indent--;
    }
    else
    {
        printf("%s (id %d size %db blocks ", node->name, node->id, node->filesize );
        for( int i=0; i<node->num_entries; i++ )
        {
            printf("%d ", (int)node->entries[i]);
        }
        printf(")\n");
    }
}

