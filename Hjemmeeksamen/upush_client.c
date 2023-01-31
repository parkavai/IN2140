#include "libraries.h"

#define BUFSIZE 325

int sequence_number = 0;
int fd, wc, rc;
unsigned short reciever_port = 0;
struct sockaddr_in reciever_addr;
#define UPPER_PORTNUMBER_RANGE 65535
#define LOWER_PORTNUMBER_RANGE 1024
unsigned short client_port = 0;

// Dårlig navn, burde endre
struct nickname_cache{
    char* nickname;
    struct sockaddr_in address; 
    struct nickname_cache* next; 
};

struct upush_client{
    char* nickname;
    struct nickname_cache nick_cache;
    // Mulig en struct message også
};

// Denne er cachen til denne terminalens klient. Hvis vi har en terminal klient med nickname "James", så er dette hele hans nick-cache. 
struct nickname_cache* client_cache; 

void free_client(struct nickname_cache* client){
    free(client->nickname);
    free(client);
}

void free_nickname_cache(){
    struct nickname_cache *tmp, *tmp2;
    int i = 1;
    for(tmp = client_cache; tmp != NULL; tmp = tmp2){
        tmp2 = tmp->next;
        printf("Frigjør klient: %d\n", i);
        printf("Navnet var: %s", tmp->nickname);
        free_client(tmp);
        i += 1;
    }
}

void do_before_exit(){
    close(fd);
    close(rc);
    free_nickname_cache();
}

void check_error(int i, char *msg) {
    if(i == -1) {
        perror(msg);
        do_before_exit();
        exit(EXIT_FAILURE);
    }
}

void generate_port(){
    srand(time(NULL));
    client_port = (rand() % (UPPER_PORTNUMBER_RANGE - LOWER_PORTNUMBER_RANGE + 1)) + LOWER_PORTNUMBER_RANGE;
}

void push_nickname_in_cache(struct nickname_cache* newClient){
    if(client_cache == NULL){
        newClient->next = NULL;
        client_cache = newClient;
    }
    else{
        struct nickname_cache* tmp = client_cache;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        newClient->next = NULL;
        tmp->next = newClient;
    }
}

void create_nick(char* nickname, struct sockaddr_in client_addr){
    struct nickname_cache* newClient = (struct nickname_cache*) malloc(sizeof(struct nickname_cache));
    if(newClient == NULL){
        perror("Error on malloc nickname, serverfile");
        free(newClient);
        // Mulig lage en funksjon som fjerner alle klienter og meldinger osv. Akkurat som i serveren med "free_list_and_clients()"
        exit(EXIT_FAILURE);
    }
    newClient->nickname = strdup(nickname);
    if(newClient->nickname == NULL){
        perror("Error on strdup nickname, serverfile");
        free(newClient->nickname);
        free(newClient);
        // Mulig lage en funksjon som fjerner alle klienter og meldinger osv. Akkurat som i serveren med "free_list_and_clients()"
        exit(EXIT_FAILURE);
    }
    newClient->address = client_addr;
    push_nickname_in_cache(newClient);
}

/* 
    Funksjonen sjekker stdinput av brukeren hvor man da sjekker input i henhold tilfeller som blir beskrevet
    i oppgaveteksten
*/
void check_standard_input(char* stdinput){
    /*
        Først sjekker vi om input var "QUIT". Hvis det er tilfelle, så skal man lukke alle sockets, frigjør minne, etc og avslutter
    */
    if(strcmp(stdinput, "QUIT") == 0){
        do_before_exit();
    }
    
}


void get_reciever_information(char* packet){
    // Benytter av strtok() til å splitte opp pakken for å gjøre det enklere å hente ut "nickname" og "sequence_number".
    char* fragments = strtok(packet, " ");
    int i = 0;
    char** array = malloc(sizeof(char*) * 8);
    if(array == NULL){
        exit(EXIT_FAILURE);
    }
    while(fragments != NULL) {
        // printf("Pakken er : %s\n", fragments);
        array[i] = fragments;
        fragments = strtok(NULL, " ");
        i++;
    }
    // fprintf(stderr, "FEIL");
    reciever_addr.sin_family = AF_INET;
    inet_pton(AF_INET, array[5], &reciever_addr.sin_addr.s_addr);
    reciever_port = atoi(array[7]);
    reciever_addr.sin_port = htons(reciever_port);
    printf("Portnummer %i\n", ntohs(reciever_addr.sin_port));
    // Pusher mottaker adressen inn i nick_cache
    create_nick(array[3], reciever_addr);
    free(array);
}

int check_sender_packet(char* packet){
    char* duplicate = strdup(packet);
    char* fragments = strtok(duplicate, " ");
    int i = 0;
    while(fragments != NULL) {
        fragments = strtok(NULL, " ");
        i++;
    }
    if(i == 7){
        get_reciever_information(packet);
        return 1;
    }
    free(duplicate);
    return 0;
}


/*
    Returnerer meldingsformatene som klienten skal sende til serveren. 
*/
char* client_package(char register_packet[], char* from_nick, char* type_message){
    if(strcmp(type_message, "REG") == 0){
        sprintf(register_packet, "PKT %d REG %s", sequence_number, from_nick);
    }
    else if(strcmp(type_message, "LOOKUP") == 0){
        sprintf(register_packet, "PKT %d LOOKUP %s", sequence_number, from_nick);
    }
    sequence_number++;
    return register_packet;
}

void get_string(char buf[], int size) {
    char c;
    fgets(buf, size, stdin);

    /* fjern newline fra slutten av strengen */
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = '\0';
    }
    /* Fjern resten av stdin dersom mer enn BUFSIZE ble lest inn */
    else while ((c = getchar()) != '\n' && c != EOF);
}


/*
    En metode som konkatenerer alle dataen som trengs til server 
*/
char* create_lookup_client(char* nickname){
    return "Ikke fikset foreloepig";
}

int main(int argc, char const *argv[])
{
    fd_set list_fd;
    unsigned short port;
    int loss_probability; 
    char* nickname;
    struct sockaddr_in client_addr, server_addr;
    struct in_addr ip_addr;
    socklen_t addr_len;
    int lookup_client = 0;
    // struct timeval tv; 
    char buf[BUFSIZE];

    // Inntill vi ikke har fikset steg "2", dropper vi å inkludere timeout og tapssannsynlighet
    /*
    if(argc < 6){
        printf("Tast inn oppgitte parametere: ./upush_server <nick> <adresse> <port> <timeout> <tapssannsynlighet> \n");
        return EXIT_FAILURE;
    }
    tv.tv_sec = argv[4];
    loss_probability = argv[5]
    */

    // I tillegg har jeg kommentert ut denne biten, ettersom jeg primært skal bruke localhost. Gjør jobben enklere
    /*
    nickname = argv[1];
    inet_pton(AF_INET, argv[2], &ip_addr);
    client_port = atoi(argv[3]);
    */

    nickname = argv[1];
    port = atoi(argv[2]);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(fd, "Error on socket for client");

    inet_pton(AF_INET, "127.0.0.1", &ip_addr);

    client_addr.sin_family = AF_INET;
    generate_port();
    client_addr.sin_port = htons(client_port);
    client_addr.sin_addr = ip_addr;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = ip_addr;

    FD_ZERO(&list_fd);
    client_package(buf, nickname, "REG");
    /*
        "Når UPush-klienten starter, prøver den å registrere nicken sin på serveren umiddelbart" som det står i oppgaveteksten. 
        Dermed sender vi en registreringsmelding med engang parameterne er tastet inn og verdiene er tilordnet. Merk at 
        vi skal sende "sendpacket()", ikke "sendto()". Men dette endres når vi har kommet til "heartbeat" eller skal implementere 
        timeout. 
    */
    rc = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    check_error(rc, "sendto");

    /*
        Må fikse slik at når klienten umiddelbart sender en pakke til serveren, så vil serveren sende tilbake en
        ACK <sekvensnummer> OK og da skal man gå inn i while-løkken under. 
    */

    // Legger inn fildeskriptorer inn i samling av alle fildeskriptorer
    while(strcmp(buf, "quit")){

        printf("[You] ");
        fflush(NULL);
        buf[rc] = '\0';
        FD_SET(STDIN_FILENO, &list_fd);
        FD_SET(fd, &list_fd);
        rc = select(FD_SETSIZE, &list_fd, NULL, NULL, NULL);
        check_error(rc, "select");
        addr_len = sizeof(struct sockaddr_in);

        if(FD_ISSET(STDIN_FILENO, &list_fd)){
            // Hvis mottaker addressen er hentet, så skal man sende direkte til en mottaker fremfor serveren
            if(lookup_client == 1){
                // Vurder å lage en funksjon
                /*
                    Du må uansett implementere slik at en pakke kan gå tapt når en klient sender en melding og håndtere dette.
                    Hvis du husker forelesningen om "retransmission" så må man kunne sende hele pakken tilbake. 
                */
                get_string(buf, BUFSIZE);
                // Sjekk standardinput om det var "@til_nick tekst", "QUIT" eller noe annet
                rc = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&reciever_addr, sizeof(struct sockaddr_in));
                check_error(rc, "Error on sendto for reciever");
                printf("Pakken %s blir sendt til mottaker: %s %i\n", buf, inet_ntoa(reciever_addr.sin_addr), ntohs(reciever_addr.sin_port));
            }
            else{
                // Vurder å lage en funksjon
                get_string(buf, BUFSIZE);
                rc = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
                check_error(rc, "Error on sendto mot server");
                rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&server_addr, &addr_len);
                check_error(rc, "recv");
                buf[rc] = '\0';
                printf("Mottok pakke fra server: %s\n", buf);
                // Hvis serveren sender "ACK <nummer> not found", skal "hentet" fremdeles være 0. Hvis ikke er, er den 1. 
                lookup_client = check_sender_packet(buf);
            }
        }
        // Sjekker hvis man har mottatt en datapakke fra serveren eller klient
        if(FD_ISSET(fd, &list_fd)){
            // Hvis addressen til klienten som sendte pakken er hentet, så skal man hente datapakken som blir sendt fra denne klienten
            if(lookup_client == 1){
                rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&reciever_addr, &addr_len);
                check_error(rc, "recv");
                buf[rc] = '\0';
                printf("Mottok pakke fra mottaker: %s\n", buf);
            }
            // Ellers så leser man inn datapakken fra serveren som er da "LOOKUP"
            /*
                Tilsvarende for å håndtere pakketap når en klient sender melding til mottaker, så må du også håndtere 
                tilfelle når mottakeren ikke bekrefter at de har mottatt en pakke fra klienten. 
            */
            rc = read(fd, buf, BUFSIZE - 1);
            check_error(rc, "Error on read");
            printf("Recieved packet from other client: %s\n", buf);
            buf[rc] = '\0';
            printf("Mottaker %s\n", buf);
        }
        /*
            FOR fd sin fildeskriptor
            rc = read(fd, buf, BUFSIZE - 1);
            check_error(rc, "Error on read");
            printf("Recieved packet from other client: %s\n", buf);
            buf[rc] = '\0';
            printf("Mottaker %s\n", buf);
        */
        /*
        rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
        check_error(rc, "Error on recieve for client");
        printf("Recieved packet: %s\n", buf);
        get_reciever_information(buf, reciever_addr);
        get_string(buf, BUFSIZE);
        rc = sendto(fd, buf, strlen(buf), 0 , (struct sockaddr*)&reciever_addr, sizeof(struct sockaddr_in));

        get_string(buf, BUFSIZE);
        rc = sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&reciever_addr, sizeof(struct sockaddr_in));
        check_error(rc, "Error on sendto");
        rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&reciever_addr, &addr_len);
        check_error(rc, "Error on recieve for client");
        printf("Recieved packet: %s\n", buf);
        get_reciever_information(buf, reciever_addr);
        free(buf);

        rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&server_addr, &addr_len);
        check_error(rc, "Error on recieve for client");
        printf("Recieved packet: %s\n", buf);
        get_reciever_information(buf, reciever_addr);
        free(buf);
        hentet = true;
        */
    }
    do_before_exit();
    return EXIT_SUCCESS;
}
