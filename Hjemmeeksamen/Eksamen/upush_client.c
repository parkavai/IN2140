#include "send_packet.h"
#include "libraries.h"
#include "client_linked_list.h"
#include "block_nickname_list.h"
#include "message_linked_list.h"

#define BUFSIZE 100
#define UPPER_PORTNUMBER_RANGE 65535
#define LOWER_PORTNUMBER_RANGE 1024

int fd, wc, rc;
int sequence_number = 0;
int seconds;

unsigned short reciever_port = 0;

time_t curr_time;
char send_package[100];
struct sockaddr_in reciever_addr;

char* reciever_nickname;
struct client_linked_list* client_cache; 
struct block_nickname_list* blocked_list;
struct message_linked_list* message_list;


void free_recieved_nickname(){
    free(reciever_nickname);
}

/*
    Whenever an error has occured or the program is about to shutdown, then this method will ensure to free() most 
    of the datastructures used. Aswell as closing down som files.  
*/
void do_before_exit(){
    free_list_and_clients(client_cache);
    free_blocked_list(blocked_list);
    free_message_list(message_list);
    close(fd);
    close(rc);
}

/*
    Checks wether or not an error has occured from a system_call such as send_to or recvfrom
*/
void check_error(int i, char *msg) {
    if(i == -1) {
        perror(msg);
        do_before_exit();
        exit(EXIT_FAILURE);
    }
}

/*
    Checks wether or not an error has occured from malloc
*/
void check_malloc_error(char* stringpointer, char* msg){
    if(stringpointer == NULL){
        perror(msg);
        do_before_exit();
        exit(EXIT_FAILURE);
    }
}

/*
    Generates a random sequence_number assigned to this client. Instead of being limited to either sequence number "1" or "0",
    i wanted have a selection of other options aswell. In this case, it will be a range between 0 and 9. 
*/
void generate_sequence_number(){
    srand(time(NULL));
    sequence_number = (rand() % (10));
}

void assign_new_nickname(char* nickname){
    reciever_nickname = strdup(nickname);
    check_malloc_error(reciever_nickname, "Error in assign_new_nickname for malloc, client ");
}

// Checks wether or not the nickname is in the clients_cache
bool check_nickname_in_cache(char* reciever_nickname){
    struct client* pointer = client_cache->start;

    while(pointer != NULL){

        if(strcmp(pointer->nickname, reciever_nickname) == 0){
            reciever_addr = pointer->address;
            return true;
        }
        pointer = pointer->next;

    }
    return false;
}

void update_reciever_client(char* reciever_nickname, struct sockaddr_in update_addr){
    struct client* pointer = client_cache->start;

    while(pointer != NULL){

        if(strcmp(pointer->nickname, reciever_nickname) == 0){
            pointer->address.sin_addr.s_addr = update_addr.sin_addr.s_addr;
            pointer->address.sin_port = update_addr.sin_port;
            break;
        }

        pointer = pointer->next;
    }
}

/*
    Checks the conditions for a given nickname written by the user to assure that the nickname is correct. If not, then the
    system shuts down. 
*/
char* check_nickname(char* nickname){
    int lengde = strlen(nickname);
    if(lengde > 20){
        fprintf(stderr, "Nickname should only have a length of 20, not: %d \n", lengde);
        return "NOT";
    }

    int low = 0;
    char p = nickname[low];
    lengde = strlen(nickname);

    while(low != lengde){

        if(isalpha(p) == 0){
            fprintf(stderr, "Remember that nickname should only have ASCII-VALUES excluding numbers \n");
            return "NOT";
        }

        if(isspace(p) != 0){
            fprintf(stderr, "Remember that nickname should only have ASCII-VALUES excluding whitespace \n");
            return "NOT";
        }

        if(isdigit(p) != 0){
            fprintf(stderr, "Remember that nickname should only have ASCII-VALUES excluding numbers \n");
            return "NOT";
        }
        low += 1;
        p = nickname[low];

    }
    return "ACCEPTABLE";
}

/* 
    Returns a string based on what the user typed such as "@til_nick text", "BLOCK <nickname>", "UNBLOCK <nickname>" or "QUIT".
    If the input is "@til_nick text" then the function will cover som cases such as checking the 'text' written, if the 
    nickname isn´t blocked or written in the format as a nickname should be written. If the input is blocked, then 
    the nickname will be created and inserted in the blocked_list. If the input is unblocked, then the given nickname 
    will be removed from the blocked_list. Otherwise, than the user will be informed about writing correctly for standard input.
*/
char* check_standard_input(char* stdinput, char* sender_nickname){
    if(strcmp(stdinput, "QUIT") == 0){
        return "quit";
    }
    else if(stdinput[0] == '@' && isalpha(stdinput[1]) != 0){

        char* temporary = strdup(stdinput);
        check_malloc_error(temporary, "Error in check_standard_input for malloc, client ");
        char* fragments = strtok(stdinput, "@");
        int i = 0;
        char buffer[20];

        while(isspace(fragments[i]) == 0 && fragments[i] != 0){
            buffer[i] = fragments[i];
            i++;
        }

        buffer[i] = '\0';
        char* pointer = buffer;
        i += 2;
        int tekstlengde = strlen(temporary) - i;

        if(tekstlengde < 0){
            free(temporary);
            fprintf(stdout, "Remember the format for 'stdin' input is: @til_nick text \n");
            return "";
        }

        char tekst[tekstlengde];
        memcpy(tekst, &temporary[i], tekstlengde);  
        tekst[tekstlengde] = '\0';
        free(temporary);
        assign_new_nickname(pointer);

        if(strcmp(reciever_nickname, sender_nickname) == 0){
            fprintf(stdout, "You can't write a message to yourself, only to other nicknames!!! \n");
            return "";
        }

        char* message = check_nickname(reciever_nickname);
        if(strcmp(message, "ACCEPTABLE") != 0){
            fprintf(stdout, "%s", message);
            free_recieved_nickname();
            return "";
        }

        if(check_if_blocked(blocked_list, reciever_nickname) == true){
            free_recieved_nickname();
            return "BLOCKED";
        }

        if(check_nickname_in_cache(reciever_nickname) != false){
            sprintf(send_package, "PKT %d FROM %s TO %s MSG %s", sequence_number, sender_nickname, reciever_nickname, tekst);
            return "FOUND";
        }

        else{
            printf("NICK %s NOT FOUND. \n SENT 'PKT %d LOOKUP %s' TO SERVER \n", reciever_nickname, sequence_number, reciever_nickname);
            return "NOT FOUND";
        }

    }
    else if(stdinput[0] == 'B' && stdinput[1] == 'L' && stdinput[2] == 'O' && stdinput[3] == 'C' && stdinput[4] == 'K'){
        char* fragments = strtok(stdinput, " ");
        char* block_nickname = "";
        int i = 0;

        while(fragments != NULL) {
            if(i == 1){
                block_nickname = fragments;
            }
            i++;
            fragments = strtok(NULL, " ");
        }

        if(check_if_blocked(blocked_list, block_nickname) == true){
            return "BLOCKED";
        }

        struct block_client* pointer = create_blocked_client(blocked_list, block_nickname);
        if(pointer == NULL){
            do_before_exit();
        }
        // If you want to see all blocked clients then you can so through removing the comments under
        print_blocked_clients(blocked_list);
        fprintf(stderr, "BLOCKED: %s \n", block_nickname);
        return "BLOCKED";
    }

    else if(stdinput[0] == 'U' && stdinput[1] == 'N' && stdinput[2] == 'B' && stdinput[3] == 'L' && stdinput[4] == 'O' && stdinput[5] == 'C' && stdinput[6] == 'K'){
        char* fragments = strtok(stdinput, " ");
        char* unblock_nickname = "";
        int i = 0;

        while(fragments != NULL) {
            if(i == 1){
                unblock_nickname = fragments;
            }
            i++;
            fragments = strtok(NULL, " ");
        }

        if(check_if_blocked(blocked_list, unblock_nickname) == false){
            fprintf(stderr, "Nickname isn't blocked %s \n", unblock_nickname);
            return "UNBLOCKED";
        }
        unblock_client(blocked_list, unblock_nickname);
        // If you want to see if a blocked_client was removed then then you can so through removing the '//' symbols
        print_blocked_clients(blocked_list);
        fprintf(stderr, "UNBLOCKED: %s \n", unblock_nickname);
        return "UNBLOCKED";
    }

    else{
        fprintf(stdout, "Remember the format for 'stdin' input is: @til_nick text \n");
        return "";
    }
}

/*
    When the server sends information about the "reciever_client" for a client, then that "reciever_client" shall
    be pushed in the client_cache. 
*/
char* get_reciever_information(char* packet){

    char* fragments = strtok(packet, " ");
    int i = 0;
    char** array = malloc(sizeof(char*) * 8);

    if(array == NULL){
        perror("Error in get_reciever_information for malloc, client \n");
        do_before_exit();
    }

    while(fragments != NULL) {
        array[i] = fragments;
        i++;
        fragments = strtok(NULL, " ");
    }

    reciever_addr.sin_family = AF_INET;
    inet_pton(AF_INET, array[5], &reciever_addr.sin_addr.s_addr);
    reciever_port = atoi(array[7]);
    reciever_addr.sin_port = htons(reciever_port);

    if(get_client(array[3], client_cache) != NULL){
        update_reciever_client(array[3], reciever_addr);
        fprintf(stderr, "UPDATED CLIENT IN CACHE: %s \n", array[3]);
        free(array);
        return "UPDATED";
    }

    else{
        create_client(client_cache, reciever_addr, array[3]);
        fprintf(stderr, "INSERTED CLIENT IN CACHE: %s \n", array[3]);
        free(array);
        return "NICK PUSHED";
    }
}

/*
    Inspects the lookup_message sent from the server and based on how many words there are in the package, 
    then it will return the given message. 
*/
char* check_server_packet(char* packet){

    char* duplicate = strdup(packet);
    check_malloc_error(duplicate, "Error in check_server_packet for malloc, client");
    char* fragments = strtok(duplicate, " ");
    int i = 0;

    while(fragments != NULL) {
        fragments = strtok(NULL, " ");
        i++;
    }

    free(duplicate);
    if(i > 4){
        return get_reciever_information(packet);
    }
    
    return "NOT FOUND";
}

/*
    Returns a package_response depending if a "sender_client" sent the message in the right way which means,
    having the "@til_nick <text>" being written in the right format and that @til_nick is the nickname 
    of this clients nickname. If the message is duplicate then the "sender_client" will recieve this message
    and not print out the message. 
*/
char* message_response(char* msg, char* sequence_number){
    char* message_response = malloc(55);
    check_malloc_error(message_response, "Error in message_response for malloc, client \n");

    if(strcmp(msg, "WRONG NAME") == 0){
        sprintf(message_response, "ACK %s WRONG NAME", sequence_number);
    }

    else if(strcmp(msg, "WRONG FORMAT") == 0){
        sprintf(message_response, "ACK %s WRONG FORMAT", sequence_number);
    }

    else if(strcmp(msg, "OK") == 0){
        sprintf(message_response, "ACK %s OK", sequence_number);
    }

    else{
        sprintf(message_response, "FOUND DUPLICATE MESSAGE");
    }

    return message_response;
}

/*
    Inspects the package sent from the sender and checks if the package is in the right format(text is ASCII-values only) or
    that the nickname isn´t blocked so that we can send back an "ACK" to the client and so on. It will returns the package 
    which is going to be sent to the "sender_client". Should the package be written in a wrong format and the nickname is 
    wrong, then due to how i have solved this situation, it shall always write back "ACK <number> WRONG NAME".
*/
char* decode_sender_package(char* packet, char* sender_nickname){
    char* fragments = strtok(packet, " ");
    int i = 0;
    int size = 1;
    char** array = malloc(sizeof(char*) * size);

    if(array == NULL){
        perror("Error in get_reciever_information for malloc, client \n");
        do_before_exit();
    }

    while(fragments != NULL) {
        array[i] = fragments;
        size += 1;
        array = realloc(array, sizeof(char*) * size);
        i += 1;
        fragments = strtok(NULL, " ");
    }

    char* sequence_number = array[1];
    char* from_nick = array[3];
    if(check_if_blocked(blocked_list, from_nick) == true){
        free(array);
        fprintf(stderr, "Can´t recieve package since client '%s' is blocked. \n", from_nick);
        return "BLOCKED";
    }

    char* to_nick = array[5];
    if(strcmp(to_nick, sender_nickname) != 0){
        free(array);
        return message_response("WRONG NAME", sequence_number);
    }
    /*
        I realize that the assignment explicitly says that the texts maximum length should be "1400-bytes", but 
        due to heartbeat, then the user may not have time to write that much unless it is copy pasted. Which is 
        why i have set a maximum length of 75. 
    */
    char text[75];
    // The text starts from index 7
    i = 7;
    int s = 0;
    size -= 1;
    while(i != size){
        /* 
            The text may have whitespace which is acceptable, therefore it will iterate as long as we haven´t reached 
            the end of the text. And since strtok will split on "whitespace", then the message will also be split into
            separate "strings". Therefore i "concatenate" the strings alltogether to form the text. 
        */
        if(s == 75){
            break;
        }
        char* tmp = array[i];
        int k = 0;

        while(tmp[k] != 0){
            text[s] = tmp[k];
            s += 1;
            k += 1;
        }

        text[s] = ' ';
        s += 1;
        i += 1;
    }
    text[s] = '\0';
    int high = strlen(text) - 1;
    while(high != 0){

        if(isdigit(text[high]) != 0){
            free(array);
            return message_response("WRONG FORMAT", sequence_number);
        }

        if(isalpha(text[high]) == 0 && isspace(text[high]) == 0){
            fprintf(stderr, "Remember that nickname should only have ASCII-VALUES excluding numbers \n");
            return message_response("WRONG FORMAT", sequence_number);
        }

        high -= 1;
    }
    // Checks if a message is a duplicate or not
    int value = is_duplicate_message(message_list, sequence_number, text);
    char* message;

    // Means the message isn´t in the message_linked_list which means that the message shall be inserted in the linked list
    if(value == 0){

        struct message* pointer = create_message(message_list, sequence_number, text);
        if(pointer == NULL){
            do_before_exit();
        }
    }

    // Message is updated
    else if(value == 2){
        update_message(message_list, sequence_number, text);
    }

    // Message is a duplicate
    else{
        message = message_response("DUPLICATE", sequence_number);
        free(array);
        return message;
    }

    message = message_response("OK", sequence_number);
    free(array);
    return message;
}

/*
    If the sender client did recieve a "Duplicate" packet from a "reciever_client" then that message shall
    not be printed out. 
*/
bool is_duplicate(char* ack_message){
    if(strcmp(ack_message, "FOUND DUPLICATE MESSAGE") == 0){
        return true;
    }
    return false;
}

/*
    Based on the value of type_message(lookup or registration), then the function will format the register_packet
    according to it. 
*/
void client_package(char register_packet[], char* from_nick, char* type_message){
    if(strcmp(type_message, "REG") == 0){

        sprintf(register_packet, "PKT %d REG %s", sequence_number, from_nick);

    }

    else if(strcmp(type_message, "LOOKUP") == 0){
        sprintf(register_packet, "PKT %d LOOKUP %s", sequence_number, reciever_nickname);
    }
}


// Used for standardinput and lets the user type within the size given
void get_string(char buf[], int size) {
    char c;
    fgets(buf, size, stdin);

    /* Removes newline from the end of the string */
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    }
    /* Removes the rest of "stdin" should the user type more than BUFSIZE */
    else while ((c = getchar()) != '\n' && c != EOF);
}

/*
    Checks if the package recieved was from the server or not. Since UDP isn´t connection based protocol then
    every message which the sender recieves, may be from itself or other "Nodes". Therefore, whenever i 
    use recvfrom i must assure that the package was sent from a node which isn´t this client.
*/
bool package_from_server(struct sockaddr_in server, struct sockaddr_in src){
    if(strcmp((char *) &server.sin_addr.s_addr, (char *) &src.sin_addr.s_addr) == 0){
        if(strcmp((char *) &server.sin_port, (char *) &src.sin_port) == 0){
            return true;
        }    
    }
    return false; 
}

/*
    This function is responsible for doing the "lookup_procedure" as descriped in the assignment. Such that if 
    the client hasn´t recieved a package from the server after 'x' seconds, then it will send the 'lookup_message'
    up to 3 times. If the client hasn´t recieved lookup within 3 attempts, then the function will return 0 which
    means that the client must shut down. This function is also used by the "send_procedure" and it will return a value
    to inform the "send_procedure" wether or not the client is found, updated or not.
*/
int lookup_procedure(char buffer[], int fd, int rc, struct sockaddr_in server, struct timeval tv, fd_set list_fd, bool client_update){
    char buf[BUFSIZE];
    client_package(buf, reciever_nickname, "LOOKUP");
    socklen_t addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in src_addr;
    int count_lookup_procedure = 0;
    char* message; 
    fprintf(stdout, "LOOKUP PROCEDURE INITIALIZED \n");
    curr_time = time(NULL);
    curr_time = seconds;
    tv.tv_sec = curr_time;
    tv.tv_usec = 0;

    while(count_lookup_procedure != 3){

        FD_SET(fd, &list_fd);

        rc = send_packet(fd, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
        check_error(rc, "Error in lookup_procedure for send_packet, client ");

        rc = select(FD_SETSIZE, &list_fd, NULL, NULL, &tv);
        check_error(rc, "Error in lookup_procedure for select, client");

        if(rc == 0){
            curr_time = time(NULL);
            count_lookup_procedure += 1;
            if(client_update == true){
                return 1;
            }
            int attempts = 3;
            fprintf(stdout, "TIMED OUT, %d ATTEMPTS LEFT \n", (attempts - count_lookup_procedure));

            curr_time = seconds;
            tv.tv_sec = curr_time;
            tv.tv_usec = 0;
            continue;
        }

        else if(FD_ISSET(fd, &list_fd)){

            rc = recvfrom(fd, buffer, BUFSIZE - 1, 0, (struct sockaddr*)&src_addr, &addr_len);
            check_error(rc, "Error in lookup_procedure for recievefrom, client \n");
            buffer[rc] = '\0';
            fprintf(stdout, "Recieved packet: %s\n", buffer);

            if(package_from_server(server, src_addr) == true){
              
                message = check_server_packet(buffer);

                if(strcmp(message, "NOT FOUND") == 0){
                    return 1;
                }

                else if(strcmp(message, "UPDATED") == 0){
                    return 2;
                }

                else{
                    return 3;
                }

            }   
        }
    }
    return 0;
}

/*
    This function is responsible for doing the "send_procedure" as descriped in the assignment. Such that if 
    the client hasn´t recieved an "ACK" from the "recieving_client" within 'x' seconds, then it will send the message again 
    and so on. 
*/
void send_procedure(char* nickname, char buffer[], int fd, int rc, struct sockaddr_in reciever, struct sockaddr_in server, struct timeval tv, fd_set list_fd){
    socklen_t addr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in src_addr;
    int value;
    int count_client_procedure = 0;
    fprintf(stdout, "SEND PROCEDURE INITIALIZED \n");

    curr_time = seconds;
    tv.tv_sec = curr_time;
    tv.tv_usec = 0;

    while(count_client_procedure != 5){

        FD_SET(fd, &list_fd);

        if(count_client_procedure < 2){
            fprintf(stdout, "Sent packet: %s \n", send_package);
            rc = send_packet(fd, send_package, strlen(send_package), 0, (struct sockaddr*)&reciever, sizeof(struct sockaddr_in));
            check_error(rc, "Error in send_procedure for send_packet, client ");
        }

        else if(count_client_procedure == 2){
        
            value = lookup_procedure(buffer, fd, rc, server, tv, list_fd, true);

            // If lookup also failed, then it will write out that client is not registered
            if(value == 1){
                fprintf(stderr, "NICK %s NOT REGISTERED \n",nickname);
                return;
            }

            // Client is updated 
            else if(value == 2){ 
                reciever = get_client(nickname, client_cache)->address;
                count_client_procedure += 1;
                continue; 
            }
        }
        else if(count_client_procedure > 2){
            rc = send_packet(fd, send_package, strlen(send_package), 0, (struct sockaddr*)&reciever, sizeof(struct sockaddr_in));
            check_error(rc, "Error in send_procedure for send_packet, client ");
        }

        rc = select(FD_SETSIZE, &list_fd, NULL, NULL, &tv);
        check_error(rc, "Error in send_procedure for select, client");
    
        if(rc == 0){
            curr_time = time(NULL);
            count_client_procedure += 1;
            int count_attempts = 5;
            fprintf(stdout, "TIMEOUT HAS OCCURRED, %i ATTEMPTS LEFT \n", (count_attempts-count_client_procedure));
            curr_time = seconds;
            tv.tv_sec = curr_time;
            tv.tv_usec = 0;
            continue;
        }

        else if(FD_ISSET(fd, &list_fd)){

            rc = recvfrom(fd, buffer, BUFSIZE - 1, 0, (struct sockaddr*)&src_addr, &addr_len);
            check_error(rc, "Error in send_procedure for recievefrom, client ");
            buffer[rc] = '\0';

            if(strcmp((char *) &reciever_addr.sin_addr.s_addr,(char *) &src_addr.sin_addr.s_addr) == 0){
                if(strcmp((char *) &reciever_addr.sin_port, (char *) &src_addr.sin_port) == 0){
                    if(is_duplicate(buffer) != true){
                        fprintf(stdout, "Recieved packet: %s \n", buffer);
                    }
                    return;
                }
            }
        }

    }
    fprintf(stderr, "NICK %s NOT REACHABLE \n", nickname);
}

// Sends registration message to server. Primariliy used for the heartbeat-part of the assignment. 
void registration_packet_to_server(char* sender_nickname, char buf[], int fd, int rc, struct sockaddr_in server){
    client_package(buf, sender_nickname, "REG");
    rc = send_packet(fd, buf, strlen(buf), 0, (struct sockaddr*)&server, sizeof(struct sockaddr_in));
    check_error(rc, "Error on registration message to server, client ");
    buf[rc] = '\0';
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in client_addr, server_addr, src_addr;
    struct in_addr ip_addr;
    struct timeval tv; 
    time_t heartbeat_time;
    socklen_t addr_len;
    char buf[BUFSIZE];

    unsigned short server_port;

    float packet_loss_rate; 
    fd_set list_fd;

    if(argc != 6){
        printf("These command-line arguments must be filled: ./upush_server <nick> <adresse> <port> <timeout> <tapssannsynlighet> \n");
        return EXIT_FAILURE;
    }

    char* sender_nickname = (char *) argv[1];
    if(strcmp(check_nickname(sender_nickname), "NOT") ==  0){
        return EXIT_FAILURE;
    }

    inet_pton(AF_INET, argv[2], &ip_addr);
    check_error(fd, "Error in main for address format, client ");

    server_port = atoi(argv[3]);
    if( atoi(argv[3]) < LOWER_PORTNUMBER_RANGE || atoi(argv[3]) > UPPER_PORTNUMBER_RANGE){
        fprintf(stderr, "Illegal port number used %u. Port number must be the same as server, client \n", server_port);
        return EXIT_FAILURE;
    }

    seconds = atoi(argv[4]);

    packet_loss_rate = atof(argv[5]);
    if(packet_loss_rate > 100 || packet_loss_rate < 0){
        fprintf(stderr, "Loss propability must be set between 0 and 100, client \n");
        return EXIT_FAILURE;
    }
    set_loss_probability(packet_loss_rate/100);
    srand48(time(NULL));

    generate_sequence_number();
    char ack_message[40];
    sprintf(ack_message, "ACK %d OK", sequence_number);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(fd, "Error in main for creating socket, client ");

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(0);
    client_addr.sin_addr = ip_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr = ip_addr;

    client_cache = create_client_list();
    if(client_cache == NULL){
        close(fd);
        return EXIT_FAILURE;
    }
    blocked_list = create_blocked_list();
    if(blocked_list == NULL){
        free_list_and_clients(client_cache);
        close(fd);
        return EXIT_FAILURE;
    }

    message_list = create_message_list();
    if(message_list == NULL){
        free_list_and_clients(client_cache);
        free_blocked_list(blocked_list);
        close(fd);
        return EXIT_FAILURE;
    }

    FD_ZERO(&list_fd);
    FD_SET(fd, &list_fd);
    addr_len = sizeof(struct sockaddr_in);
    curr_time = seconds;
    tv.tv_sec = curr_time;
    tv.tv_usec = 0;
    /*
        If client hasn´t recieved an ACK for the first time, then the system shuts down. Otherwise the program will proceed towards
        the stop-and-wait loop. 
    */
    while(1){

        registration_packet_to_server(sender_nickname, buf, fd, rc, server_addr);
        rc = select(FD_SETSIZE, &list_fd, NULL, NULL, &tv);
        check_error(rc, "Error in main for select, client ");

        if(rc == 0){
            fprintf(stdout, "Server hasn't sent package within time, system shutdown on client \n");
            do_before_exit();
            return EXIT_FAILURE;
        }

        else if(FD_ISSET(fd, &list_fd)){

            rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&src_addr, &addr_len);
            check_error(rc, "Error in main for recvfrom, client ");
            buf[rc] = '\0';

            if(package_from_server(server_addr, src_addr) == true){
                if(strcmp(buf, ack_message) == 0){
                        break;
                }
            }
        }
    }

    heartbeat_time = 10;
    tv.tv_sec = heartbeat_time;
    tv.tv_usec = 0;
    FD_ZERO(&list_fd);

    while(strcmp(buf, "quit")){
        printf("\n [%s] ", sender_nickname);
        fflush(NULL);

        FD_SET(STDIN_FILENO, &list_fd);
        FD_SET(fd, &list_fd);
        rc = select(FD_SETSIZE, &list_fd, NULL, NULL, &tv);
        check_error(rc, "Error in main for select, client: ");

        /*
            If timeout has occurred, then the program wil send a registration message. I assume that the server won´t 
            send an ACK back to the client since it isn´t mentioned in the assignment. Also, should the user write a message 
            in the meantime and registration_message is sent, then the user can press enter and the message is then sent. 
            Wasn´t able to fix the issue with being able to reset the output so that it is more "user-friendly". 
        */
        if(rc == 0){
            heartbeat_time = time(NULL);
            registration_packet_to_server(sender_nickname, buf, fd, rc, server_addr);
            // fprintf(stdout, "Sent packet %s", buf);
            heartbeat_time = 10;
            tv.tv_sec = heartbeat_time;
            tv.tv_usec = 0;
        }

        else if(FD_ISSET(STDIN_FILENO, &list_fd)){
         
            get_string(buf, BUFSIZE);
            if(strcmp(buf, "QUIT") == 0){
                break;
            }

            char* state = check_standard_input(buf, sender_nickname);
            if(strcmp(state, "NOT FOUND") == 0){
                
                int check = lookup_procedure(buf, fd, rc, server_addr, tv, list_fd, false);
                free_recieved_nickname();

                // Initializes QUIT-procedure since the server didn´t send an "ACK" after 3 attempts
                if(check == 0){
                    break;
                }
            }

            else if(strcmp(state, "FOUND") == 0){

                if(check_if_blocked(blocked_list, reciever_nickname) == true){
                    continue;
                }
                int i = 0;
                int lengde = strlen(send_package);
                /*
                    Send_package is a global variable which will contain a string in the format which is meant to be 
                    sent to the recieving client. The package will only be overwritten whenever the "reciever_client"
                    is known to this client. 
                */
                while(i != lengde){
                    buf[i] = send_package[i];
                    i++;
                }

                buf[lengde] = '\0';
                // Må fjerne send_procedure funksjonen siden, 
                struct client* reciever = get_client(reciever_nickname, client_cache);
                send_procedure(reciever->nickname, buf, fd, rc, reciever->address, server_addr, tv, list_fd);
                free_recieved_nickname();
                check_error(rc, "Error in main for send_procedure, client \n");

            }  

            else if(strcmp(state, "BLOCKED") == 0){
                continue;
            }

            else if(strcmp(state, "UNBLOCKED") == 0){
                continue;
            }

            else{
                continue;
            }
        }
        else if(FD_ISSET(fd, &list_fd)){

            rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&src_addr, &addr_len);
            check_error(rc, "Error in main for recvfrom, client \n");
            buf[rc] = '\0';

            // Assures that the message is sent from someone else
            if(strcmp((char *) &client_addr.sin_addr.s_addr, (char *) &src_addr.sin_addr.s_addr) == 0){

                if(strcmp((char *) &client_addr.sin_port, (char *) &src_addr.sin_port) != 0){

                    // Also assures that should the message be from the server which is referring to heartbeat, then we just continue
                    if(package_from_server(server_addr, src_addr) == true){
                        continue;
                    }

                    else{
                        char* to_sender = decode_sender_package(buf, sender_nickname);
                        // Whenever the client blocks a nickname, we must assure wether or not the nickname is blocked or not
                        if(strcmp(to_sender, "BLOCKED") == 0){
                            continue;
                        }

                        int i = 0;
                        int length = strlen(to_sender);

                        while(i != length){
                            buf[i] = to_sender[i];
                            i++;
                        }

                        buf[length] = '\0';
                        free(to_sender);
                        rc = send_packet(fd, buf, length, 0, (struct sockaddr*)&src_addr, sizeof(struct sockaddr_in));
                        check_error(rc, "Error in send_packet for reciever, client \n");
                        fprintf(stdout, "Sent packet: %s \n", buf);
                    }

                }
                continue;
            }
        }
    }
    do_before_exit();
    return EXIT_SUCCESS;
}