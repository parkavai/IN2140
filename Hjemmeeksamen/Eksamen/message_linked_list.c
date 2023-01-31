#include "message_linked_list.h"

struct message_linked_list* create_message_list(){
    struct message_linked_list* message_list = malloc(sizeof(struct message_linked_list));
    if(message_list == NULL){
        perror("Error in create_message_list for malloc, message_linked_list");
        return NULL; 
    }
    message_list->start = NULL; 
    return message_list;
}

void free_message(struct message* pointer){
    free(pointer->sequence_number);
    free(pointer->text);
    free(pointer);
}

void free_message_list(struct message_linked_list* message_list){
    if(message_list == NULL){
        return;
    }
    else if(message_list->start == NULL){
        free(message_list);
        return;
    }
    struct message *tmp, *tmp2;
    for(tmp = message_list->start; tmp != NULL; tmp = tmp2){
        tmp2 = tmp->next;
        free_message(tmp);
    }
    free(message_list);
}

void print_list(struct message_linked_list* message_list){
    struct message* pointer = message_list->start;
    int i = 0;
    while(pointer != NULL){
        printf("\n");
        printf("Message nr: %d \n", i);
        printf("Message ACK: %s \n", pointer->sequence_number);
        printf("Message Text: %s \n", pointer->text);
        i++;
        pointer = pointer->next;
    }
}

struct message* get_message(struct message_linked_list* message_list, char* sequence_number){
    struct message* pointer = message_list->start;
    while(pointer != NULL){

        if(strcmp(pointer->sequence_number, sequence_number) == 0){
            return pointer;
        }
        pointer = pointer->next;

    }
    return NULL;
}

int is_duplicate_message(struct message_linked_list* message_list, char* sequence_number, char* text){
    struct message* tmp = message_list->start;
    int return_value = 0;
    while(tmp != NULL){
        if(strcmp(tmp->sequence_number, sequence_number) == 0){
            if(strcmp(tmp->text, text) == 0){
                return_value = 1;
            }
            else{
                return_value = 2;
            }
        }
        tmp = tmp->next;
    }
    return return_value;
}

void update_message(struct message_linked_list* message_list, char* sequence_number, char* text){
    struct message* tmp = message_list->start;
    while(tmp != NULL){
        if(strcmp(tmp->sequence_number, sequence_number) == 0){
            free(tmp->text);
            tmp->text = strdup(text);
        }
        tmp = tmp->next; 
    }
}

void push_message(struct message_linked_list* message_list, struct message* newMessage){
    if(message_list->start == NULL){
        message_list->start = newMessage;
        message_list->start->next = NULL;
    }
    else{
        struct message* tmp = message_list->start;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = newMessage;
        newMessage->next = NULL;
    }
}

struct message* create_message(struct message_linked_list* message_list, char* sequence_number, char* text){
    struct message* newMessage= (struct message*) malloc(sizeof(struct message));
    if(newMessage == NULL){
        perror("Error in create_message for malloc, message_linked_list");
        return NULL;
    }
    newMessage->sequence_number = strdup(sequence_number);
    if(newMessage->sequence_number == NULL){
        perror("Error in create_message for malloc, message_linked_list");
        return NULL;
    }
    newMessage->text = strdup(text);
    if(newMessage->text == NULL){
        perror("Error in create_message for malloc, message_linked_list");
        return NULL;
    }
    push_message(message_list, newMessage);
    return newMessage;
}
