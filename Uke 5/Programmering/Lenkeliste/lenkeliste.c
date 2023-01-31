#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct lenkeliste {
    struct node* first;
};

struct node {
    int value;
    struct node* next;
};

struct node *constructNode(int data){
    struct node *ny = malloc(sizeof(struct node));
    ny->value = data;
    return ny;
}

void push_front(struct lenkeliste* ll, struct node* new_node){
    if(ll->first == NULL){
        ll->first = new_node;
    }
    else{
        if(ll->first->next == NULL){
            ll->first->next = new_node;
        }
        else{
            struct node *tmp = ll->first;
            while(tmp->next != NULL){
                tmp = tmp->next;
            }
            tmp->next = new_node;
        }
    }
}

struct node pop_front(struct lenkeliste* ll){
    struct node *tmp = ll->first;
    ll->first = tmp->next;
    return *tmp;
}

int number_of_elements(struct lenkeliste* ll){
    int tall = 0;
    struct node *tmp = ll->first;
    while(tmp != NULL){
        tall++;
        tmp = tmp->next;
    }
    return tall;
}

int main(){
    struct lenkeliste *liste = malloc(sizeof(struct lenkeliste));
    struct node* node1 = constructNode(1);
    struct node* node2 = constructNode(2);
    struct node* node3 = constructNode(3);
    push_front(liste, node1);
    push_front(liste, node2);
    push_front(liste, node3);
    printf("Antall noder: %d \n", number_of_elements(liste));
    printf("Node som ble fjernet hadde verdi: %d \n", pop_front(liste).value);
    free(node1);
    free(node2);
    free(node3);
    free(liste);
}