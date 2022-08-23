#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Trengs for systemkallene for nettverksoperasjon
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define BUFSIZE 325

void check_error(int res, char *msg){
    if(res == -1){
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void get_string(char buf[], int bufsize){
    char c;

    fgets(buf, bufsize, stdin);
    if(buf[strlen(buf - 1)] == '\n'){
        buf[strlen(buf) - 1] = 0;
    }
    else{
        while((c = getchar()) != '\n' && c != EOF);
    }
}

/*                                              SELECT
    “Select” er en funksjon som hjelper oss med å monitorere(holde oversikt) over en rekke fildeskriptorer 
    og gir beskjed over hvilke fildeskriptorer som består av datapakker som er klare til å sende. Denne hjelper 
    oss med å vite hvilke fildeskriptorer med datapakker som fører til at vi kan benytte av recv() funksjonen 
    for å hente en fildeskriptorer med datapakker. Derfor kan man benytte av “Select” hvor funksjonen har som 
    mål å hente ut datapakker fra en socket hvis det er datapakker i den. Funksjonen sjekker altså tilstanden 
    til en socket og hvis det er fildeskriptorer tilgjengelig, så vil “Select” kunne hente ut fildeskriptoren fra funksjonen. 

    Slik ser parameterne for select-funksjonen ut:
    int select(int nfds, fd_set *readfds, fd_set *exceptfds, fd_set *writefds, struct timeval timeout);

    Den første parameteren beskriver antall fildeskriptorer som monitoren/(select) kan maksimalt ta inn. Dvs, om du 
    setter verdien til 3, så kan maksimalt ta imot 3 fildeskriptorer og funksjonen vil da se over disse da. Men hvis
    man er usikker på hvilke antall man skal ha, så kan man bare sette verdien til FD_SETSIZE som er bare en konstant. 

    Den andre, tredje og fjerde parameteren har en type kalt "fd_set" som er en liste av fildeskriptorer. Dvs, at du 
    har en samling av fildeskriptorer som skal benyttes enten for "Lesing" eller "Skriving". 

    Den siste parameteren er en "timeout" som vil si at du kan bestemme hvor lenge monitoren/(select) skal sitte og 
    vente. Hvis du vil at den skal vente i "20 sek" så kan den godt gjøre dette. Og merk at den vil sitte og vente 
    inntill ett av fildeskriptorene er klare med en datapakke så hvis en datapakke ikke er klar til noen av fildeskriptorene
    så vil den vente til f.eks "20 sek" og stoppe. 
*/

int main(int argc, char const* argv[]){
    int msg_fd, rc;

    /*
        Som nevnt i innledningen over, så er "fd_set" essensielt ettersom dette skal være listen som består 
        av alle fildeskriptorene våres. Men merk at denne er tom, vi har ikke lagd listen enda og får å gjøre 
        dette, må vi kalle på funksjonene som er tilknyttet til "fd_set". 
    */
    fd_set fds;
    struct sockaddr_in friends_addr, my_addr;
    struct in_addr ip_addr;
    char buf[BUFSIZE];

    if(argc < 4){
        printf("Usage: %s <my port> <friends port> <friends ip> \n", argv[0]);
        return EXIT_SUCCESS;
    }

    msg_fd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(msg_fd, "Error for call on socket");

    inet_pton(AF_INET, argv[3], &ip_addr);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[1]));
    my_addr.sin_addr = ip_addr;

    friends_addr.sin_family = AF_INET; 
    friends_addr.sin_port = htons(atoi(argv[2]));
    friends_addr.sin_addr = ip_addr;

    rc = bind(msg_fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in));   
    check_error(rc, "Error on binding");
    
    /*
        FD_ZERO er ett av funksjonene som er tilknyttet til "fd_set" som initialiserer selve listen. Som nevnt 
        tidligere så har vi ikke initialisert listen når vi skriver "fd_set fds". Vi har bare lagd selve variabelen, 
        men initialiseringen foregår gjennom FD_ZERO()
    */
    FD_ZERO(&fds);
    printf("Welcome to MSN! Type stuff to your friend! \n");

    buf[0] = '\0';
    while( strcmp(buf, "quit") ){

        printf("[You] ");
        fflush(NULL);

         /*
            Du lurer sikkert på hvorfor vi tar imot "STDIN_FILENO" som fildeskriptor. Vel i CBRA-videoen så var målet å 
            lage en "chatboks" hvor vi skal da chatte med en "venne-endesystem". Vi skal da skrive i terminalen imens 
            "venne-endesystemet" skal kommunisere tilbake til oss gjennom dets terminal. Ved å benytte av "STDIN_FILENO" 
            så vil vi få en fildeskriptor som er tilknyttet til terminalen. Dermed kan vi kommunisere med hverandre. Dette
            vil jeg tro er meget essensielt i hjemmeeksamen siden vi skal gjør det samme også. 
        */
        FD_SET(STDIN_FILENO, &fds);

        /*
            For å legge til fildeskriptoren inn i "fd_set"-listen vår så må vi bruke funksjonen FD_SET. Du kan se på denne 
            funksjonen som "add()" i en arraylist, hvor den legger til fildeskriptoren inn i "fd_set". 
        */
        FD_SET(msg_fd, &fds);

        /*
            Nå kaller vi endelig på funksjonen "select()" ettersom alt er klart. Så det som vil skje nå, er at monitoren 
            vil som sagt vente på fildeskriptorene som ligger inne i "fd_set fds" har datapakker tilgjengelig. Nå siden 
            vi ikke har satt en "timeval" så vil den bare vente i evighet, men i hjemmeeksamen tror jeg man skal sette 
            en tid da. 

            Noe som er viktig å nevne er returverdien til funksjonen. Akkurat som alle andre systemkall, så vil den returnere
            "-1" hvis det oppstod feil ved kallet. Men den kan også returnere "0" hvis og bare hvis man har satt en "timeval". 
            Derfor må vi også sjekke dette, men det behøver vi ikke her da siden vi ikke har satt noe "timeval". Men skulle 
            det skje, ville vi måtte sjekket noe slikt:

            if(rc == 0){
                // Gjør noe//
            }
        */

        rc = select(FD_SETSIZE, &fds, NULL, NULL, NULL);
        check_error(rc, "Error for call on select");
        /*
            Om vi har passert denne delen, så betyr det at det lå en datapakke klar i ett av fildeskriptorene i "fd_set fds". 
            Men hvordan skal vi vite hvilken fildeskriptor som er klar til å sendes? Monitoren/(select) vil fjerne alle 
            fildeskriptorer som ikke har datapakker tilgjengelig, men dette hjelper oss allikevel ikke siden det kan være 
            flere fildeskriptorer med datapakker klare. 

            Løsningen på å finne ut hvilken fildeskriptor som er klare gjøres gjennom FD_ISSET. Denne funksjonen vil sjekke 
            en fildeskriptor fra "fd_set"-listen vår og vurdere om den har en datapakke klar eller ikke. 
        */

        if(FD_ISSET(STDIN_FILENO, &fds)){
            // Vi er klare til å sende melding til vår venn
            get_string(buf, BUFSIZE);
            rc = sendto(msg_fd, buf, strlen(buf), 0, (struct sockaddr*) &friends_addr, sizeof(struct sockaddr_in));
            check_error(rc, "Error on cal for sendto");
        }

        else if(FD_ISSET(msg_fd, &fds)){
            // Melding er blitt sendt fra vår venn
            rc = read(msg_fd, buf, BUFSIZE - 1);
            check_error(rc, "Error for call on read");
            buf[rc] = 0;
            printf("\r%45s [Ano]\n", buf);
            /*
            printf("\r%45s [Friend]\n", buf);
            */

        }
    }
    
    close(msg_fd);

    return EXIT_SUCCESS;

}