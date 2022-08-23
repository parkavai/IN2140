#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


/* Jeg (Joakim) har skrevet litt mer kode etter timen siden vi ikke rakk det i timen.
 * Still gjerne spørsmål. */


#define BUF_SIZE 62
/* Definer struct person*/
struct person {
	//char name[BUF_SIZE];	Ulempen med denne er at 90 bytes allokeres uansett hvor lang strengen er.
	char *name; // 8 (64bit maskin, typisk)
	int age; // 4
	int height; // 4
};

int les_inn_streng(char *p, int size, char *what)
{
	if (p == NULL)
		return -1;

	printf("%s:", what);
	fflush(stdout); /* Husk at printf skriver til stdout som buffres. */

	int ret = read(0, p, size-1);
	if (ret == -1) {
		return ret;
	}
	p[ret] = 0;
	p[strcspn(p,"\n")] = 0; /* Man it */

	return ret;
}

int les_inn_tall(int *res, char *what)
{
	/* Les inn streng */
	char buf[BUF_SIZE]; /* Setter av plass på stacken */
	int ret = les_inn_streng(buf, BUF_SIZE, what);

	if (ret == -1) {
		return 1;
	}

	/* Gjør om til tall */
	/* Ta en titt på man atoi, kanskje annen funksjon bør brukes?*/
	*res = atoi(buf);
	return 0;
}

void print_person(struct person *p)
{
	printf("Navn: %s, Alder: %d, Height: %d\n",
	       p->name,
	       p->age,
	       p->height);
}

int main(void)
{
	/* Sett av plass til struct person på stack eller heap */
	struct person *p2 = malloc(sizeof(struct person));
	if (!p2) /* Når p2 er null vil !p2 være true */
		return 1;
	p2->name = "jokim"; //(*p2).name = "joakim";
	p2->age = 27;
	p2->height = 89;
	print_person(p2);
	/* Gjorde feil i forelesning når jeg skrev int *age = p2 + 8;, fordi adressen blir p2 + sizeof(struct person) * 8 */
	int *age = (int*)((char*)p2 + 8); /* Antar ingen padding. */
	printf("Age through int pointer: %d\n", *age);
	free(p2); /* Bruker ikke p2 mer*/

	struct person p1 = {0};
	p1.name = malloc(BUF_SIZE);
	p1.age = 27889;
	p1.height = 2;

	printf("Størrelsen til %lu\n", sizeof(p1)); /* Obs can be padding*/

	/* Les inn navn */
	les_inn_streng(p1.name, BUF_SIZE, "navn");
	/* Les inn alder */
	int ret = les_inn_tall(&p1.age, "alder");
	/* Les inn høyde */
	ret += les_inn_tall(&p1.height, "hight");
	if (ret != 0) {
		fprintf(stderr, "Feil i innlesningen\n");
		return 1;
	}

	print_person(&p1);
	free(p1.name);



	return 0;
}