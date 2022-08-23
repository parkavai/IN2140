#ifndef MESSAGE_LINKED_LIST
#define MESSAGE_LINKED_LIST

#include "libraries.h"

struct message{
    char* sequence_number;
    char* text;
    struct message* next; 
};

struct message_linked_list{
    struct message* start;
};

// Creates the message_list
struct message_linked_list* create_message_list();

// Frees the message
void free_message(struct message* pointer);

// Frees the entire message_list
void free_message_list(struct message_linked_list* message_list);

// Primarily used for debugging
void print_list(struct message_linked_list* message_list);

// Primarily used for debugging
struct message* get_message(struct message_linked_list* message_list, char* sequence_number);

/*
    Checks if the message a "sender_client" has recieved, is a duplicate or not. 
    Returns a number which represents wether the message is a duplicate, if it should be updated 
    or if we must create a new message. 
*/
int is_duplicate_message(struct message_linked_list* message_list, char* sequence_number, char* text);

// Updates the message for a specific sequence_number should the message not be a duplicate
void update_message(struct message_linked_list* message_list, char* sequence_number, char* text);

// Inserts a new message to the message_list
void push_message(struct message_linked_list* message_list, struct message* pointer);

// Creates a new message
struct message* create_message(struct message_linked_list* message_list, char* sequence_number, char* text);

#endif