#include "block_nickname_list.h"

struct block_nickname_list* create_blocked_list(){
    struct block_nickname_list* blocked_list = malloc(sizeof(struct block_nickname_list));
    if(blocked_list == NULL){
        perror("Error in create_blocked_list for malloc, block_nickname_list");
        return NULL; 
    }
    blocked_list->start = NULL; 
    return blocked_list;
}

void free_blocked_client(struct block_client* pointer){
    free(pointer->nickname);
    free(pointer);
}

void free_blocked_list(struct block_nickname_list* blocked_list){
    if(blocked_list == NULL){
        return;
    }
    else if(blocked_list->start == NULL){
        free(blocked_list);
        return;
    }
    struct block_client *tmp, *tmp2;
    int i = 1;
    for(tmp = blocked_list->start; tmp != NULL; tmp = tmp2){
        tmp2 = tmp->next;
        free_blocked_client(tmp);
        i += 1;
    }
    free(blocked_list);
}

void print_blocked_clients(struct block_nickname_list* blocked_list){
    struct block_client* pointer = blocked_list->start;
    int i = 0;
    while(pointer != NULL){
        printf("\n");
        printf("Client nr: %d \n", i);
        printf("Client name: %s \n", pointer->nickname);
        i++;
        pointer = pointer->next;
    }
}

bool check_if_blocked(struct block_nickname_list* blocked_list, char* nickname){
    struct block_client* pointer = blocked_list->start;
    while(pointer != NULL){
        if(strcmp(pointer->nickname, nickname) == 0){
            return true;
        }
        pointer = pointer->next;
    }
    return false;
}

void push_blocked_client(struct block_nickname_list* blocked_list, struct block_client* blocked_client){
    if(blocked_list->start == NULL){
        blocked_list->start = blocked_client;
        blocked_list->start->next = NULL;
    }
    else{
        struct block_client* tmp = blocked_list->start;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = blocked_client;
        blocked_client->next = NULL;
    }
}

void unblock_client(struct block_nickname_list* blocked_list, char* nickname){
    struct block_client* tmp = blocked_list->start;
    if(strcmp(blocked_list->start->nickname, nickname) == 0){
        blocked_list->start = blocked_list->start->next;
        free_blocked_client(tmp);
        return;
    }
    struct block_client* previous;
    while(tmp != NULL){
        if(strcmp(tmp->nickname, nickname) == 0){
            previous->next = tmp->next;
            free_blocked_client(tmp);
            break;
        }
        previous = tmp; 
        tmp = tmp->next;
    }
}

struct block_client* create_blocked_client(struct block_nickname_list* listOfClients, char* nickname){
    struct block_client* blockClient = malloc(sizeof(struct block_client));
    if(blockClient == NULL){
        perror("Error in malloc for create_blocked_client, block_nickname_list");
        return NULL;
    }
    blockClient->nickname = strdup(nickname);
    if(blockClient->nickname == NULL){
        perror("Error in malloc for create_blocked_client, block_nickname_list");
        free(blockClient);
        return NULL;
    }
    push_blocked_client(listOfClients, blockClient);
    return blockClient;
}