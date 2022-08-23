/*
 * Copied from https://beej.us/guide/bgnet/html/
** client.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "person.h"

/*
 * An overview of port-allocation:
 * less /etc/services
*/

#define SERVERPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 100

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char buf[MAXBUFLEN];
    int numbytes;

    if (argc != 3) {
        fprintf(stderr,"usage: ./client <hostname> <message>\n");
        exit(1);
    }

    /* Tell getaddrinfo what kind of address we want
     * Check domain/family and protocol/socktype: man socket
     */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;

    /* Notice that the last argument is a double-pointer 
     * The system call gives us an address to the first element in a
     * linked list as well as a return value indicating success/failure
     * man getaddrinfo */
    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
	
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to create socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("client: sendto");
        exit(1);
    }

    printf("client: sent %d bytes to %s\n", numbytes, argv[1]);
    
    /* This might block forever. Why? Even if server successfully transmits 
       a packet into the network.  */
    printf("Waiting for response\n");
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
        NULL, 0)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    printf("client: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("client: packet contains \"%s\"\n", buf);


    /* TODO plenum ? */

    struct person *me = create_person("gunn", 1, 14);
    print_person(me);
    int buf_len = 0;
    char *serialized = serialize_person(me, &buf_len);
				 
    if ((numbytes = sendto(sockfd, serialized, buf_len, 0,
			   p->ai_addr, p->ai_addrlen)) == -1) {
	    perror("client: sendto");
	    exit(1);
    }
    printf("client: sent %d bytes to %s\n", numbytes, argv[1]);

    free(serialized);
    free(me);
    freeaddrinfo(servinfo);
    
    close(sockfd);

    return 0;
}
