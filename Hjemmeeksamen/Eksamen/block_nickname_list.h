#ifndef BLOCK_NICKNAME_LIST
#define BLOCK_NICKNAME_LIST

#include "libraries.h"

struct block_client{
    char* nickname; 
    struct block_client* next;
};

struct block_nickname_list{ 
    struct block_client* start; 
};

// Creates the blocked_list
struct block_nickname_list* create_blocked_list();

// frees a blocked_list
void free_blocked_client(struct block_client* blocked_client);

// frees the entire blocked_list
void free_blocked_list(struct block_nickname_list* blocked_list);

// Primarily used for debugging
void print_blocked_clients(struct block_nickname_list* blocked_list);

/*
    If a nickname is already set to be blocked then the client won´t be able to send nor recieve a 
    message from the 'blocked' client
*/
bool check_if_blocked(struct block_nickname_list* blocked_list, char* nickname);

// Pushes nickname in list
void push_blocked_client(struct block_nickname_list* blocked_list, struct block_client* blocked_client);

/*
    If user wants to unblock client, then the method will remove the blocked_client from the blocked_list
*/
void unblock_client(struct block_nickname_list* blocked_list, char* nickname);

// Creates a blocked client
struct block_client* create_blocked_client(struct block_nickname_list* listOfClients, char* nickname);

#endif