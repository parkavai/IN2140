#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 

FILE* open_file(char *filename, char *mode)
{
	FILE *f = fopen(filename, mode);
	if (!f) {
		perror("fopen");
	}
	return f;
}
struct inode {
	struct inode *next;
	char *name;
	int size;
	int *blocks;
	int num_blocks;
};
struct inode *list_append(struct inode *head, struct inode *new)
{
	if (head == NULL)
		head = new;
	else
		head->next = list_append(head->next, new);
	return head;
}
int list_size(struct inode *head)
{
	return head ? 1 + list_size(head->next): 0;
}


struct inode *create_file(char *name, int size, int *blocks, int num_blocks)
{
	struct inode *new_file = malloc(sizeof(struct inode));
	if (!new_file) {
		fprintf(stderr, "malloc failed");
		return NULL;
	}
	new_file->name = strdup(name);
    free(name);
	new_file->size = size;
	new_file->blocks = malloc(sizeof(*blocks)*num_blocks);
	if (!new_file->blocks) {
		fprintf(stderr, "malloc failed");
		free(new_file);
		return NULL;
	}
	memcpy(new_file->blocks, blocks, num_blocks * sizeof(*blocks));
    free(blocks);
	new_file->num_blocks = num_blocks;
	new_file->next = NULL;
	return new_file;
}

struct inode *create_list_of_random_files(int count)
{
	#define NUM_BLOCKS 500
	struct inode *head = NULL;
	char blocks_used[NUM_BLOCKS];

	memset(blocks_used, 0, NUM_BLOCKS);
	while(count-- > 0) {
		char *name;
		int size;
		int *blocks;
		int num_blocks;

		name = malloc(20);
		if (!name) {
			fprintf(stderr, "malloc failed");
			continue;
		}
		snprintf(name, 10, "Fil %d", count);
		num_blocks = random()%5;
		blocks = malloc(num_blocks * sizeof(int));
		if (!blocks) {
			fprintf(stderr, "malloc failed");
			free(name);
			continue;
		}
		size = num_blocks * 512 + random()%512;
		for(int i = 0; i < num_blocks; ++i) {
			int block;
			do {
				block = random() % NUM_BLOCKS;
			} while (blocks_used[block]);
			blocks_used[block] = 1;

			blocks[i] = block;
		}
		struct inode *new_file = create_file(name, size, blocks, num_blocks);
		head = list_append(head, new_file);
	}
	return head;
}


void print_files(struct inode *head)
{
	if (head == NULL)
		return;
	printf("%s,%d,%d:", head->name, head->size, head->num_blocks);
	for (int i = 0; i < head->num_blocks; ++i)
		printf(" %d", head->blocks[i]);
	printf("\n");
	print_files(head->next);
}



struct inode *read_files(FILE *f)
{
	/* TODO */

	int size;
	int ret = fread(&size, sizeof(int), 1, f);
	printf("Størrelse er %d", size);
	if (ret != 1) {
		perror("fread");
		return NULL;
	}
    
	for (int i = 0; i < size; i++) {
		int index;
		fread(&index, sizeof(index), 1, f);
		/* Read name */

        // Siden vi skrev inn lengden til navn inn i filen, så vet vi hvor mye bufferet skal allokeres av minne, nemlig strenglengden til navn
		int len;
		fread(&len, sizeof(int), 1, f);
		char *buf = malloc(len);
		if (!buf) {
			fprintf(stderr, "malloc in read for name");
			continue;
		}
		fread(buf, 1, len, f);

        /* Read size */

		int fsize; /* str fil */
		fread(&fsize, sizeof(int), 1, f);

        /* Read blocks */

		int num_blocks;
		fread(&num_blocks, sizeof(int), 1, f);

		int *blocks = malloc(num_blocks * sizeof(int));
		fread(blocks, sizeof(int), num_blocks, f);
        free(blocks);
        free(buf);
	}
	return NULL;
}

/*
struct inode {
	struct inode *next;
	char *name;
	int size;
	int *blocks;
	int num_blocks;
};
*/
void write_files(FILE *f, struct inode *file)
{
	/* TODO */

	/* Write metadata */
    
    // Beskriver hvor mange filer/noder vi har i lenkelisten
	int size = list_size(file);
	fwrite(&size, sizeof(size), 1, f);

	int i = 0;
	for (struct inode *cur = file; cur != NULL; cur = cur->next) {	

        // Her skriver vi indeksen til hver fil/noden i lenkelisten som beskriver hvilken fil vi er i, men var ikke nødvendig å inkludere 
		fwrite(&i, sizeof(i), 1, f);
		i++;
		/* Write name */
        /*
            Måten denne funker på, er at man først skriver strenglengden til navn til å vite hvor 
            mye man må allokere til buffer, fordi si at vi har tilfeldige navn. Da blir det vanskelig 
            å vite hvor mye et gitt buffer skal ha av byte siden navnene kan ha tilfeldig størrelse. 
            Derfor sa Joakim at han først skriver ut strenglengden slik at brukeren blir bevisst over 
            hvor lang bufferet skal være. Så det som skjer, er at strenglengden vil bli skrevet inn i 
            filen. Videre når vi skal lese inn filen, så kan vi da bruke denne strenglengden som ble 
            skrevet inn i filen, til å angi som verdi for bufferet vårt. Sjekk read-funksjonen. 
        */
		int len = strlen(cur->name) + 1; 
		fwrite(&len, sizeof(len), 1, f);
		fwrite(cur->name, len, 1, f);

		/* Write size */
		fwrite(&cur->size, sizeof(cur->size), 1, f);

		/* Write blocks */
		fwrite(&cur->num_blocks, sizeof(cur->num_blocks), 1, f);
		fwrite(cur->blocks, sizeof(*cur->blocks), cur->num_blocks, f);
	}
}

void free_files(struct inode *head)
{
	/* TODO */
    struct inode *tmp, *tmp2;
    /*
    Først må vi frigjøre de allokerte plassene for verdiene 
    til de ulike nodene. Dette inkluderer navn og blocks.
    Tilslutt frigjør vi selve noden. 
    */
    for (tmp = head; tmp != NULL; tmp = tmp2){
        tmp2 = tmp->next;
        free(tmp->name);
        free(tmp->blocks);
        free(tmp);
    }
}


int main(int argc, char *argv[])
{

	if (argc < 2) {
		fprintf(stderr, "USAGE: ./%s <filename>\n", argv[0]);
		return 1;
	}

	char *filename = argv[1];

	struct inode *files = create_list_of_random_files(5);
	printf("Number of files: %d\n", list_size(files));
	print_files(files);


	FILE *f = open_file(filename, "w");
	write_files(f, files);
	fclose(f);

	
	f = open_file(filename, "r");
	struct inode *reread_files = read_files(f);
	print_files(reread_files);
    free_files(files);
    fclose(f);
}