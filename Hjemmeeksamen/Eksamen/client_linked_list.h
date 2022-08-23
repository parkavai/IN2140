#ifndef CLIENT_LINKED_LIST
#define CLIENT_LINKED_LIST

#include "libraries.h"

/*
    The updated value is primarily used for "heartbeat" which represents wether a client has only 
    been registered, updated or should be removed from the servers registration list. 
    
    The different values which a clients updated value may have
    Registered = 5
    Updated = 1
    Removed = 0

*/
struct client{
    char* nickname;
    int updated;
    struct sockaddr_in address; 
    struct client* next;
};

struct client_linked_list{
    struct client* start;
};

// Creates the client_list
struct client_linked_list* create_client_list();

// Frees the client
void free_client(struct client* client);

// Frees the client_list
void free_list_and_clients(struct client_linked_list* listOfClients);

void print_clients();

// Returns the size of the client_list
int get_size_of_list();

// Returns either a NULL-value or a client based on nickname
struct client* get_client(char* nickname, struct client_linked_list* listOfClients);

// Inserts clients in the client_list
void push_client(struct client* newClient, struct client_linked_list* listOfClients);

/*
    Checks wether or not all clients have been updated to ensure that every clients updated value shall be 
    reset or not
*/
bool all_clients_updated(struct client_linked_list* listOfClients);

/* 
    If the server hasn´t recieved a registration message from a client within 30-seconds, 
    then that clients "updated", value is set to 0 which means it is meant to be removed and not used in server
*/
void set_client_to_be_removed(struct client_linked_list* listOfClients);

/* 
    If all clients have been updated, then we reset all of the clients update values to be 5. 
*/
void reset_client_update_values(struct client_linked_list* listOfClients);

/*
    Removes a client from the servers "registration_list" which is primarily used for heartbeat. If the client´s 
    update value is set to "0" then the client is to be removed whenever a lookup is called towards the current client.
*/
void remove_client(struct client* pointer, struct client_linked_list* listOfClients);

// Creates a client
struct client* create_client(struct client_linked_list* listOfClients, struct sockaddr_in client_addr, char* nickname);

#endif