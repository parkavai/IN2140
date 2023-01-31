#include "client.h"

#define BUFSIZE 125

/*
    Globale variabler som benyttes primært for datastrukturen av registrerte klienter.
    I tillegg til nickname og sekvensnummer for å oppdatere variablene for hver gang en ny klient dukker opp. 
    Tilslutt en "type_message" som brukes for å skille mellom registreringsmeldinger og oppslagsmeldinger.
*/
int indeks = 0;
int list_size = 0;
int fd, rc;
struct client* listOfClients = NULL;
char* nickname = " ";
char* sequence_number = " ";
char* type_message = " ";
char buf[BUFSIZE];


void free_client(struct client* client){
    free(client->nickname);
    free(client);
}

void free_list_and_clients(){
    struct client *tmp, *tmp2;
    int i = 1;
    for(tmp = listOfClients; tmp != NULL; tmp = tmp2){
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
    free_list_and_clients();
}

/*
    Denne funksjonen brukes primært for håndtering av feilmeldinger ved systemkall. Blant annet bind(), socket()
    og andre diverse systemkall hvor ved feil, så vil de returnere "-1". 
*/
void check_error(int ret, char* msg){
    if(ret == -1){
        perror(msg);
        do_before_exit();
        exit(EXIT_SUCCESS);
    }
}

// Må fikse til senere for hva som kan skje hvis det er "NULL", men er foreløpig løsning
struct client* get_client(char* nickname){
    struct client* pointer = listOfClients;
    while(pointer != NULL){
        if(strcmp(pointer->nickname, nickname) == 0){
            return pointer;
        }
        pointer = pointer->next;
    }
}


void reset_buffer(char* buffer){
    memset(buffer, 0, sizeof(buffer));
}

char* print_message(char* sequence_number, struct client* c, char* ack_message){
    char* buf = malloc(100);
    if(buf == NULL){
        free(buf);
        fprintf(stderr, "Oppstod feil ved allokering av bytes til navn");
        return NULL;
    }
    if(strcmp(ack_message, "OK") == 0){
        sprintf(buf, "ACK %s OK", sequence_number);
    }
    else if(strcmp(ack_message, "NOT FOUND") == 0){
        sprintf(buf, "ACK %s NOT FOUND", sequence_number);
    }
    else if(strcmp(ack_message, "PORT") == 0){
        sprintf(buf, "ACK %s NICK %s IP %s PORT %i", sequence_number, c->nickname, inet_ntoa(c->address.sin_addr), ntohs(c->address.sin_port));
    }
    else{
        return "QUIT";
    }
    printf("BUF %s\n", buf);
    return buf;
}

/*
    Metode for å filtrere ut "nickname", "sekvensnummer" og typen melding som ble sendt ut fra klienten.
*/
char* filter_out_nickname_and_sequence_number(char *packet){
    // Benytter av strtok() til å splitte opp pakken for å gjøre det enklere å hente ut "nickname" og "sequence_number".
    char* fragments = strtok(packet, " ");
    int deler = 1;
    // Bedre å lage en array også setter du inn alle verdiene inn i arrayet
    // char array[4];
    while(fragments != NULL) {
        printf("Pakken er : %s\n", fragments);
        if(deler == 2){
            sequence_number = fragments;
        }
        else if(deler == 3){
            type_message = fragments;
        }
        else if(deler == 4){
            nickname = fragments;
        }
        deler += 1;
        fragments = strtok(NULL, " ");
    }
    if(deler > 5){
        return "Whitespace on nickname";
    }
    return nickname;
}

bool check_packet(char *packet){

    if(strcmp(filter_out_nickname_and_sequence_number(packet), "Whitespace on nickname") == 0){
        printf("Whitespace on nickname");
        exit(EXIT_SUCCESS);
    }

    printf("Nickname: %s \n", nickname);
    int lengde = strlen(nickname);
    if(lengde > 20){
        printf("Lengden er for stor");
        return false;
    }
    int low = 0;
    char p = nickname[low];
    while(isspace(p) != 0){

        if(!isalpha(p)){
            printf("Det er ikke ASCII-verdier for bokstaven: %c \n", p);
            return false;
        }

        low++;
        p = nickname[low];
    }
    return true;
}

void print_clients(){
    struct client* pointer = listOfClients;
    int i = 0;
    while(pointer != NULL){
        printf("Client nr: %d \n", i);
        printf("Client navn: %s \n", pointer->nickname);
        printf("Client ip-adresse: %s \n", inet_ntoa(pointer->address.sin_addr));
        printf("Client port: %hu \n", ntohs(pointer->address.sin_port));
        printf("\n");
        i++;
        pointer = pointer->next;
    }
}

/*
    I forbindelse med implementasjonen av "Hjerteslag" så blir det nevnt at, klienter må "sende en registreringsmelding
    hver 10.sekund til serveren". Videre blir det nevnt at "serveren skal ikke bruke noen registrering som ikke har blitt 
    oppdatert i løpet av de siste 30 sekundene til å svare på en klients oppslagsforespørsel. Serveren bør "returnere 
    "ACK <nummer> NOT FOUND" i steder og fjerne !!!!NICK!!! fra serverens liste over registrerte nick" på et passende tidspunkt.
    
    Dvs, skulle en klient ikke sende en melding til serveren innen gitt tid så må vi sørge for at serveren fjerner den klienten
    fra serverens lenkeliste over registrerte klientert.

    Vurder om du skal også sjekke hvis lenkelisten kun har en registrert klient. 
*/
void remove_client(char* nickname){
    struct client* tmp = listOfClients;
    struct client* forrige;
    while(tmp->next != NULL){
        if(strcmp(tmp->nickname, nickname) == 0){
            forrige->next = tmp->next;
            free_client(tmp);
            break;
        }
        forrige = tmp; 
        tmp = tmp->next;
    }
}

char* push_client(struct client* newClient){
    if(listOfClients == NULL){
        newClient->next = NULL;
        listOfClients = newClient;
    }
    else{
        struct client* tmp = listOfClients;
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        newClient->next = NULL;
        tmp->next = newClient;
    }
    return print_message(sequence_number, newClient, "PORT");
}

/*
    Antar at brukeren taster inn korrekt informasjon når det gjelder informasjon om klient som ip-addresse og port for registreringen.
    Denne funksjonen registrerer en ny klient inn i serveren hvor man gjør en del sjekk. Blant annet må man forsikre at nye klienter 
    med en "nickname" som allerede er blitt registrert, så skal serveren kun erstatte den gamle IP-/Portinfromasjonen med den nye
    IP-adressen/porten. 
*/
bool is_client_registered(char* new_client_nickname, struct sockaddr_in client_addr){
    struct client* pointer = listOfClients;
    while(pointer != NULL){
        if(strcmp(pointer->nickname, new_client_nickname) == 0){
            printf("LIKE NAVN FRA PEKERE %s OG NYE KLIENT: %s \n", pointer->nickname, new_client_nickname);
            pointer->address.sin_addr.s_addr = client_addr.sin_addr.s_addr;
            pointer->address.sin_port = client_addr.sin_port;
            printf("ACK %s IS ALREADY REGISTERED, CHANGED PORT AND ADDR \n", sequence_number);
            return true;
        }
        pointer = pointer->next;
    }
    return false;
}

void create_client(struct sockaddr_in client_addr){
    struct client* newClient = (struct client*) malloc(sizeof(struct client));
    if(newClient == NULL){
        perror("Error on malloc nickname, serverfile");
        free(newClient);
        do_before_exit();
        exit(EXIT_FAILURE);
    }
    newClient->nickname = strdup(nickname);
    if(newClient->nickname == NULL){
        perror("Error on strdup nickname, serverfile");
        free(newClient->nickname);
        free(newClient);
        do_before_exit();
        exit(EXIT_FAILURE);
    }
    newClient->address = client_addr;
    push_client(newClient);
}

char* lookup_client(char* nick){
    struct client* pointer = listOfClients;
    int i = 0;
    while(pointer != NULL){
        fprintf(stderr, "Sjekker klient %d\n: ", i);
        fprintf(stderr, "nickname %s\n", pointer->nickname);
        if(strcmp(pointer->nickname, nick) == 0){
            char* pakke = print_message(sequence_number, pointer, "PORT");
            return pakke; 
        }
        pointer = pointer->next;
        i++;
    }
    return print_message(sequence_number, pointer, "NOT FOUND");
}

int main(int argc, char const *argv[])
{   
    unsigned short server_port, client_port;
    int loss_probability; 
    struct sockaddr_in server_addr, sender_addr;
    socklen_t addr_len;

    if (argc < 3) {
        printf("Tast inn oppgitte parametere: ./upush_server <port> <tapssannsynlighet> \n");
        return EXIT_FAILURE;
    }
    server_port = atoi(argv[1]);
    loss_probability = (int) argv[2];

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(fd, "socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY; 

    rc = bind(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    check_error(rc, "bind");

    while(1){

        addr_len = sizeof(struct sockaddr_in);
        rc = recvfrom(fd, buf, BUFSIZE - 1 , 0, (struct sockaddr*)&sender_addr, &addr_len);
        check_error(rc, "recv");
        printf("Portnummer fra client: %i\n", ntohs(sender_addr.sin_port));
        //client_port = ntohs(sender_addr.sin_port);
        //sender_addr.sin_port = client_port;

        buf[rc] = '\0';
        printf("Recieved packet: %s \n", buf);
        if(strcmp(buf, "quit") == 0 || strcmp(buf, "QUIT") == 0){
            break;
        }
        // Må kalle på sendto() for å sende meldingen til Klienten
        else if(check_packet(buf) == true){
            if(strcmp(type_message, "REG") == 0){
                if(is_client_registered(nickname, sender_addr) == false){
                    create_client(sender_addr);
                    print_clients();
                }
                else{
                    printf("ALLEREDE REGISTRERT");
                }
            }
            else if(strcmp(type_message, "LOOKUP") == 0){
                // printf("%s", lookup_client(nickname));
                char* lookup = lookup_client(nickname);
                struct client* hentet = get_client(nickname);
                rc = sendto(fd, lookup, strlen(lookup), 0, (struct sockaddr*)&sender_addr, sizeof(struct sockaddr_in));
                check_error(rc, "Error on sendto for server");
                free(lookup);
            }
            else{
                break;
            }
        }
        type_message = " ";
        nickname = " ";
    }
    do_before_exit();
    return EXIT_SUCCESS;
}
