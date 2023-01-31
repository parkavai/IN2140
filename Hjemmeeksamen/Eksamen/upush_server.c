#include "client_linked_list.h"
#include "send_packet.h"
#include "libraries.h"

#define BUFSIZE 125
#define UPPER_PORTNUMBER_RANGE 65535
#define LOWER_PORTNUMBER_RANGE 1024

char* nickname = " ";
char* sequence_number = " ";
char* type_message = " ";
int fd, rc;
struct client_linked_list* server_list;
char buf[BUFSIZE];


/*
    Whenever an error has occured or the program is about to shutdown, then this method will ensure to free() most 
    of the datastructures used. Aswell as closing down som files.  
*/
void do_before_exit(){
    free_list_and_clients(server_list);
    close(fd);
    close(rc);
    exit(EXIT_SUCCESS);
}

/*
    Checks wether or not an error has occured from a system_call such as send_to or recvfrom
*/
void check_error(int ret, char* msg){
    if(ret == -1){
        perror(msg);
        do_before_exit();
    }
}

/*
    Method which returns the package which will be sent to the client
*/
void send_package_to_client(char buf[], char* sequence_number, struct client* c, char* ack_message){

    if(strcmp(ack_message, "OK") == 0){
        sprintf(buf, "ACK %s OK", sequence_number);
    }

    else if(strcmp(ack_message, "NOT FOUND") == 0){
        sprintf(buf, "ACK %s NOT FOUND", sequence_number);
    }

    else if(strcmp(ack_message, "PORT") == 0){
        sprintf(buf, "ACK %s NICK %s IP %s PORT %i", sequence_number, c->nickname, inet_ntoa(c->address.sin_addr), ntohs(c->address.sin_port));
    }
}

/*
    Method which filters out the nickname and sequence number
*/
char* filter_out_nickname_and_sequence_number(char *packet){

    char* fragments = strtok(packet, " ");
    int i = 0;
    int size = 1;
    char** array = malloc(sizeof(char*) * size);

    if(array == NULL){
        fprintf(stderr, "Error in malloc for array, server");
        do_before_exit();
    }

    while(fragments != NULL) {
        array[i] = fragments;
        size += 1;
        array = realloc(array, sizeof(char*) * size);
        i += 1;
        fragments = strtok(NULL, " ");
    }

    if(i > 4 || i < 4){
        free(array);
        return "Whitespace on nickname";
    }

    sequence_number = array[1];
    type_message = array[2];
    nickname = array[3];
    free(array);
    return nickname;
}

// Inspects the client_package
bool check_packet(char *packet){

    char* message = filter_out_nickname_and_sequence_number(packet);

    if(strcmp(message, "Whitespace on nickname") == 0){
        do_before_exit();
        exit(EXIT_SUCCESS);
    }

    return true;
}

/*
    Checks if the client is registered in the servers "registration_list". If it is then it will return 1
    and also sets the clients updated value to "1". This means that it won´t be removed from the server. However,
    should the update value be set to "0", then the server won´t update its client since that client shall be 
    removed from the server. If none of the cases mentioned should not be accepted, then the client will be pushed
    in the servers "registration_list". 
*/
int is_client_registered(char* new_client_nickname, struct sockaddr_in client_addr){

    struct client* pointer = server_list->start;

    while(pointer != NULL){

        if(strcmp(pointer->nickname, new_client_nickname) == 0){

            pointer->address.sin_addr.s_addr = client_addr.sin_addr.s_addr;
            pointer->address.sin_port = client_addr.sin_port;
            pointer->updated = 1;
            fprintf(stderr, "Client %s is updated \n", new_client_nickname);
            return 1;
        }

        pointer = pointer->next;

    }
    return 0;
}

int main(int argc, char const *argv[])
{   
    unsigned short server_port;
    float loss; 
    struct sockaddr_in server_addr, sender_addr;
    time_t curr_time;
    struct timeval tv;
    socklen_t addr_len;
    fd_set list_fd;

    if (argc != 3) {
        fprintf(stderr, "Remember to write the in the correct arguments: ./upush_server <port> <tapssannsynlighet> \n");
        return EXIT_FAILURE;
    }

    server_port = atoi(argv[1]);
    if(atoi(argv[1]) < LOWER_PORTNUMBER_RANGE || atoi(argv[1]) > UPPER_PORTNUMBER_RANGE){
        fprintf(stderr, "Illegal port number used %u. Port number must be between 1024 and 65535 \n", server_port);
        return EXIT_FAILURE;
    }

    loss = atof(argv[2]);
    if(loss > 100 || loss < 0){
        fprintf(stderr, "Loss propability must be set between 0 and 100 \n");
        return EXIT_FAILURE;
    }
    set_loss_probability(loss/100);

    srand48(time(NULL));
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(fd, "Error occured in serverfile in main for creating socket, ");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY; 

    rc = bind(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    check_error(rc, "Error occured in serverfile in main for bind, ");

    server_list = create_client_list();
    if(server_list == NULL){
        do_before_exit();
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&list_fd);
    addr_len = sizeof(struct sockaddr_in);
    curr_time = 30;
    tv.tv_sec = curr_time;
    tv.tv_usec = 0;
    int end_server = 0;

    /*
        If the server hasn´t recieved a message from the server within 60 seconds, then the server shuts down
    */
    while(1){
        FD_SET(fd, &list_fd);
        rc = select(FD_SETSIZE, &list_fd, NULL, NULL, &tv);
        check_error(rc, "Error in main for select, server ");
        printf("\n");

        if(rc == 0){
            curr_time = time(NULL);
            fprintf(stdout, "TIMEOUT HAS PASSED \n");

            /*
                If end_value is "2", then it means a whole minute has passed in which the server hasn´t recieved a message from 
                one of the clients.
            */
            end_server += 1;
            if(end_server == 2){
                fprintf(stderr, "Server shuts down \n");
                break;
            }

            if(get_size_of_list() >= 1){
                if(all_clients_updated(server_list) == true){
                    // fprintf(stderr, "All clients have been updated, reset values \n");
                    reset_client_update_values(server_list);
                }
                else{
                    set_client_to_be_removed(server_list);
                }
            }
            curr_time = 30;
            tv.tv_sec = curr_time;
            tv.tv_usec = 0;
            continue;
        }

        else if(FD_ISSET(fd, &list_fd)){
            int message_length;
            end_server = 0;
            rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&sender_addr, &addr_len);
            check_error(rc, "Error in main for recvfrom, server ");
            buf[rc] = '\0';
            fprintf(stdout, "Recieved packet: %s \n", buf);
            
            if(check_packet(buf) == true){

                if(strcmp(type_message, "REG") == 0){
                    int value = is_client_registered(nickname, sender_addr);

                    if(value == 0){

                        struct client* pointer = create_client(server_list, sender_addr, nickname);
                        if(pointer == NULL){
                            do_before_exit();
                            exit(EXIT_FAILURE);
                        }
                        // To see which clients are registered, remove the '//' under
                        // print_clients(server_list);
                        send_package_to_client(buf, sequence_number, NULL, "OK");
                    }

                    else if(value == 1){
                        send_package_to_client(buf, sequence_number, NULL, "OK");
                    }

                    message_length = strlen(buf);
                    rc = send_packet(fd, buf, message_length, 0, (struct sockaddr*)&sender_addr, sizeof(struct sockaddr_in));
                    check_error(rc, "Error in main for send_packet, server ");
                    fprintf(stdout, "Sent packet: %s \n", buf);

                }
                else if(strcmp(type_message, "LOOKUP") == 0){

                    struct client* pointer = get_client(nickname, server_list);
                    if(pointer == NULL){
                        send_package_to_client(buf, sequence_number, NULL, "NOT FOUND");
                    }

                    else{
                        int value = pointer->updated;

                        if(value == 0){
                            remove_client(pointer, server_list);
                            send_package_to_client(buf, sequence_number, NULL, "NOT FOUND");
                        }

                        else{
                            send_package_to_client(buf, sequence_number, pointer, "PORT");
                        }

                    }

                    message_length = strlen(buf);
                    rc = send_packet(fd, buf, message_length, 0, (struct sockaddr*)&sender_addr, sizeof(struct sockaddr_in));
                    check_error(rc, "Error in main for send_packet, server ");
                }

            }
        }

    }
    do_before_exit();
    return EXIT_SUCCESS;
}
