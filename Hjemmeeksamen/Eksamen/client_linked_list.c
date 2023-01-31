#include "client_linked_list.h"

int size = 0;

struct client_linked_list* create_client_list(){
    struct client_linked_list* listOfClients = malloc(sizeof(struct client_linked_list));
    if(listOfClients == NULL){
        perror("Error in create_client_list for malloc, client_Linked_List");
        return NULL; 
    }
    listOfClients->start = NULL; 
    return listOfClients;
}

void free_client(struct client* client){
    free(client->nickname);
    free(client);
}

void free_list_and_clients(struct client_linked_list* listOfClients){
    if(listOfClients == NULL){
        return;
    }
    else if(listOfClients->start == NULL){
        free(listOfClients);
        return;
    }
    struct client *tmp, *tmp2;
    for(tmp = listOfClients->start; tmp != NULL; tmp = tmp2){
        tmp2 = tmp->next;
        free_client(tmp);
    }
    free(listOfClients);
}

// Primarily used for debugging
void print_clients(struct client_linked_list* listOfClients){
    struct client* pointer = listOfClients->start;
    int i = 0;
    while(pointer != NULL){
        printf("\n");
        printf("Client nr: %d \n", i);
        printf("Client name: %s \n", pointer->nickname);
        printf("Client ip-address: %s \n", inet_ntoa(pointer->address.sin_addr));
        printf("Client port: %hu \n", ntohs(pointer->address.sin_port));
        i++;
        pointer = pointer->next;
    }
}

int get_size_of_list(){
    return size;
}

struct client* get_client(char* nickname, struct client_linked_list* listOfClients){
    struct client* pointer = listOfClients->start;
    if(get_size_of_list() >= 1){

        if(strcmp(nickname, pointer->nickname) == 0){
            return pointer; 
        }

        else{

            while(pointer != NULL){

                if(strcmp(pointer->nickname, nickname) == 0){
                    return pointer;
                }
                pointer = pointer->next;

            }
            return NULL;
        }

    }
    return NULL;
}

void push_client(struct client* newClient, struct client_linked_list* listOfClients){
    if(listOfClients->start == NULL){
        listOfClients->start = newClient;
        listOfClients->start->next = NULL;
    }
    else{
        struct client* tmp = listOfClients->start;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = newClient;
        newClient->next = NULL;
    }
    size += 1;
}

bool all_clients_updated(struct client_linked_list* listOfClients){
    struct client* tmp = listOfClients->start;
    int value = 0;
    while(tmp != NULL){
        value = tmp->updated;
        if(value != 1){
            return false;
        }
        tmp = tmp->next;
    }
    return true;
}

void set_client_to_be_removed(struct client_linked_list* listOfClients){
    struct client* tmp = listOfClients->start;
    int value = 0;
    while(tmp != NULL){
        value = tmp->updated;
        if(value == 5){
            tmp->updated = 0;
            fprintf(stderr, "Client %s will be removed \n", tmp->nickname);
            break;
        }
        tmp = tmp->next;
    }
}

void reset_client_update_values(struct client_linked_list* listOfClients){
    struct client* tmp = listOfClients->start;
    while(tmp != NULL){
        tmp->updated = 5;
        tmp = tmp->next;
    }
}

void remove_client(struct client* pointer, struct client_linked_list* listOfClients){
    struct client* tmp = listOfClients->start;
    struct client* previous;
    if(strcmp(pointer->nickname, listOfClients->start->nickname) == 0){
        listOfClients->start = tmp->next;
        free_client(tmp);
        size -= 1;
        return;
    }
    else{
        while(tmp != NULL){
            if(strcmp(pointer->nickname, tmp->nickname) == 0){
                previous->next = tmp->next;
                free_client(tmp);
                size -= 1;
                break;
            }
            previous = tmp; 
            tmp = tmp->next;
        }
    }
}

struct client* create_client(struct client_linked_list* listOfClients, struct sockaddr_in client_addr, char* nickname){
    struct client* newClient = (struct client*) malloc(sizeof(struct client));
    if(newClient == NULL){
        perror("Error in malloc for create_client, client_linked_list");
        return NULL;
    }
    newClient->nickname = strdup(nickname);
    if(newClient->nickname == NULL){
        perror("Error in malloc for create_client, client_linked_list");
        free(newClient);
        return NULL;
    }
    newClient->address = client_addr;
    newClient->updated = 5;
    push_client(newClient, listOfClients);
    return newClient;
}
