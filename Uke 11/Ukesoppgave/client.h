#ifndef CLIENT_H
#define CLIENT_H

#include "libraries.h"

struct client{
    char *nickname; 
    char *ip_address;
    unsigned char port;
    timeval tv_sec; 
    int loss_probability;
};

struct client *create_client(char *nickname, char *ip_adresse, unsigned char port, timeval timeout, int loss_probability);

#endif