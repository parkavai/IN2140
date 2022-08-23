#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define READFD 0
#define WRITEFD 1
#define BUFSIZE 255


int tall = 5;

void check_failure(int res, char *msg){
    if(res == -1){
        fprintf(stderr, msg);
        exit(EXIT_FAILURE);
    }
}

void child_main(int readfd){
    /*
    tall = 500;
    printf("This is the child printing\n");
    */

    char buf[BUFSIZE];
    int rc;

    while (strcmp(buf, "q")){
        rc = read(readfd, buf, BUFSIZE - 1);
        check_failure(rc, "read");
        buf[rc] = '\0';
        printf("Barn mottok: %s\n", buf);
    }

    close(readfd);

}

void parent_main(int writefd){
    /*
    printf("This is the parent printing\n");
    sleep(1);
    printf("tall = %d\n", tall);
    */

    int wc;
    // char *msg = "Hello my child!";
    // sleep(1);
    char msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);

    while(!strcmp(msg, "q")){
        // Lese fra stidin, skrive til child
        fgets(msg, BUFSIZE, stdin);
        msg[strlen(msg) - 1] = 0;
        wc = write(writefd, msg, strlen(msg));
        check_failure(wc, "write");
    }
    wc = write(writefd, msg, strlen(msg));
    check_failure(wc, "write");
    close(writefd);
}

int main(void) {
    /*
    Nødvendig å ha med siden ved kall på fork(), så 
    er det slik at, ved suksess på kall av fork(), så 
    returneres pid til "child"-prosessen altså prosessen som 
    ble duplisert fra parent. I tillegg så vil man returnere 
    "0" ved suksess. Ellers så returneres "-1" hvis kallet ikke 
    ble suksessfullt. Merk at i sjekken, så vil begge
    */
    pid_t is_parent;

    /*
    En pipe tar imot en int-array på "2", hvor disse er da 
    de fildeskriptorene vi sender med hvor en av dem er "read" 
    og andre er da "write". Ved kall på pipe() så må 
    man sjekke om kallet var suksessfult eller ikke, hvor du må forsikre 
    at pipe() ikke er -1. Hvis pipe() er -1, så feilet systemkallet. Hvis 
    ikke så gikk det greit. 
    */
    int fds[2];
    check_failure(pipe(fds), "pipe");
    
    /*
    Husk å alltid sjekke om "is_parent" er korrekt eller ikke, siden 
    det kan hende at forken var suksessful, men det kan hende at 
    is_parent == -1 etter fork(). Derfor sjekker jeg dette under.  
    */
    is_parent = fork();
    if(is_parent == 0){
        check_failure(is_parent, "fork");
        close(fds[READFD]);
        parent_main(WRITEFD);
    }
    else{
        close(fds[WRITEFD]);
        child_main(READFD);
    }

    return 0;
}