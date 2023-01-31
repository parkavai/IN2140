#include <stdlib.h>

struct Person
{
	char *name; /* 64-bit (depending on the machine)*/
	int age; /* 32-bit (usually)*/
	double height_meters; /* 64-bit */
}; /* Total size: 20 Bytes + any padding added by compiler for optimizing access.*/

short add(short arg1, short arg2)
{
	return arg1 + arg2; /*Potential "overflow"*/
}

void change_age(struct Person *person, int new_age)
{
	person -> age = new_age;
}

int main(void) /*Yes, you can ignore the arguments*/
{
	/*In this file we will simply list all sorts of patterns that you might find useful*/

	/* Variables with common size */
	char character = 'a'; /* 8 bit/1 byte */
	short short_number = -2410; /* 16 bit */
	int number = 2410; /* 32 bit/4 bytes*/ 
	long long_number = -2410; /* 64 bit */
	
	unsigned char character_unsigned = 'a'; /* 8 bit */
	unsigned short short_number_non_negative = 2410; /* 16 bit */
	unsigned int number_non_negative = 2410; /* 32 bit */
	unsigned long long_number_non_negative = 2410; /* 64 bit */

	
	float floating_point_number = 3.14; /* 32 bit */
	double floating_point_number_better_precision_and_greater_range = 3.14; /* 64 bit */

	/* Arrays */
	int digits[10] = {0,1,2,3,4,5,6,7,8,9};
	char unfilled_alphabet[29]; /*29, right? Doesn't matter so don't bother correcting it.*/

	/* Control statements */
	/* 0 is false, and everything else is true. No concept of boolean */
	if (character == 'a')
    {
		/*Do something*/
	} else if (number) {
		/*Outdated*/
	} else {
		/*I'm lost*/
	}

	while (number > 2401)
    {
		number--;
	}

	int i;
	for (i = 0; i < number; ++i)
    {
		number++;
	}

	do
    {
		floating_point_number /= 2; /*floating_point_number = floating_point_number/2*/
	}
    while (floating_point_number > 3);

	/* Logical operations */
	unsigned char a = 0b00001111;
	unsigned char b = 0b11110000;

	unsigned char ff = a | b;
	ff = a ^ b;
	unsigned char zero = a & b;
	b = ~a;
	zero = a >> 4;
	a = b >> 4;

	/* Structs instead of objects */
	struct Person person;
	person.name = "Anonymous";
	person.age = 10;
	person.height_meters = 2.14;

	/* Head allocated memory and pointers*/
	int *heap_allocated_digits = (int*) malloc(10 * sizeof(int));
	if (!heap_allocated_digits) /*ALWAYS check that the allocation was successful!*/
    {
		/*Allocation failed, cannot use heap_allocated_digits as a valid pointer*/
	} else {
		heap_allocated_digits[0] = 0;
		heap_allocated_digits[1] = 1;
		/* .. (For-loop is your friend)*/
	}

	struct Person *person_ptr = &person; /* person_ptr is a variable that contains the address of person */
	(*person_ptr).age = 19; /* Change the age by first de-referencing the pointer, then accessing the age */
	person_ptr->age = 19; /* Short hand */
	change_age(person_ptr, 19); /* Or let a function do it */

	/* person_ptr_ptr contains the address of person_ptr, and parson_ptr contains the address of person.*/
	struct Person **person_ptr_ptr = &person_ptr;
	/* (*person_ptr_ptr) gives you the content of person_ptr, so */
	(*person_ptr_ptr)->age = 19; /*Access the persons age*/
}