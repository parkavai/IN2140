#ifndef PERSON_H
#define PERSON_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h> 

struct person {
	char *name;
	char is_student;
	int age;
};

struct person *create_person(char *name, char is_student, int age) {
	struct person *p = malloc(sizeof(struct person));
	if (!p) {
		fprintf(stderr, "Malloc failed!\n");
		return NULL;
	}
	p->name = strdup(name);
	p->is_student = is_student;
	p->age = age;
	return p;
}

void print_person(struct person *p)
{
	if (p == NULL)
		printf("Person is null!\n");
	else
		printf("Name: %s\nis_student:%d\nage:%d\n",
		       p->name,
		       p->is_student,
		       p->age);
}

char* serialize_person(struct person *p, int *size)
{
	/*namelen(int)|name(variabel)|is_student(char)|age(int)*/
	*size = sizeof(int) + strlen(p->name) +
		1 + sizeof(int);
	char *buf = malloc(*size);
	char *ptr = buf;

	int len = strlen(p->name);
	memcpy(ptr, &len, sizeof(len));
	ptr += sizeof(len);
	
	memcpy(ptr, p->name, len);
	ptr += len;

	*ptr = p->is_student;
	ptr++;

	int age_cpy = htonl(p->age);
	memcpy(ptr, &age_cpy, sizeof(int));
	
	return buf;
}

struct person *deserialize_person(char *buf, int size)
{
	/*namelen(int)|name(variabel)|is_student(char)|age(int)*/
	int len;
	memcpy(&len, buf, sizeof(int));
	buf += sizeof(int);

	char *name = malloc(len + 1);
	memcpy(name, buf, len);
	name[len] = 0;
	buf += len;

	char is_student = *buf;
	buf++;

	int age;
	memcpy(&age, buf, sizeof(int));
	age = ntohl(age);
	
	
	return create_person(name, is_student, age);
}

#endif
