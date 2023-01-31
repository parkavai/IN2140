/*
 * Copied from https://beej.us/guide/bgnet/html/
** server.c -- a datagram sockets "server" demo
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
#include <fcntl.h>

#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int create_socket(const char *port)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				     p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind socket\n");
        return 2;
    }

    /* From man-pages: 
       Under Linux, select() may report a socket file descriptor as "ready for reading",
       while nevertheless a subsequent read blocks.  This could for example happen  when
       data has arrived but upon examination has wrong checksum and is discarded.  There
       may be other circumstances in which a file descriptor is spuriously  reported  as
       ready.  Thus it may be safer to use O_NONBLOCK on sockets that should not block.
    */
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    freeaddrinfo(servinfo);
    return sockfd;
}

/* Use nc (netcat) instead of using a self-written client
 * nc -u localhost 4041
 * nc -u localhost 4040
 * These do not have to be restarted in-between server restarts because it
 * uses UDP.
 */


int main(void)
{
	/* For data about sender */
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	/* For reading messages */
	char buf[MAXBUFLEN];
	int numbytes;

	/* Sockets for communication */
	int sock_client_fd, sock_admin_fd, sock_admin_fd2;

	/* select related variables */
	fd_set rfds_orig, rfds;
	struct timeval tv;
	int retval;
	int max_fd_value = 0;


	/* Set up two sockets (filedescriptors) */
	sock_client_fd = create_socket("4040");
	sock_admin_fd = create_socket("4041");
	sock_admin_fd2 = create_socket("4042");
	
	/* Set up select */

	/* Specify which sockets/filedescriptors you are waiting
	 * for to become ready to read.
	 * 1. Init rfds_orig, used as second argument to select */
	FD_ZERO(&rfds_orig);
	
	FD_SET(sock_client_fd, &rfds_orig);
	FD_SET(sock_admin_fd, &rfds_orig);
	FD_SET(sock_admin_fd2, &rfds_orig);
	

	/* Select needs the greatest socket/filedescriptor value (+1) 
	 * (poll has to be used if greatest filedescriptor is greater than
	 * FD_SETSIZE (glibc 1024))
	 * 2. Set the maximum filedescriptor value, max_fd_value */
	max_fd_value = max_fd_value < sock_client_fd ? sock_client_fd : max_fd_value;
	max_fd_value = max_fd_value < sock_admin_fd ? sock_admin_fd : max_fd_value;
	max_fd_value = max_fd_value < sock_admin_fd2 ? sock_admin_fd2 : max_fd_value;

	while (1) {
		
		/* Make sure set correctly.
		 * rfds is value-result argument, so it is modified by select. 
		 * (poll does not have this design flaw)*/
		rfds = rfds_orig;

		/* If you want a timeout this has to be set
		 * and pointer passed as last argument to select 
		 * (need to be reinitialized on Linux, see man-pages) */
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		/* Now wait for activity */
		retval = select(max_fd_value + 1, &rfds, NULL, NULL, NULL); //&tv);


		if (retval == -1) {
			perror("select");
			break;
		} else if (retval) { /* Number of fds ready > 0*/
			
			/* Go through all sockets to see which are ready 
			 * 2, find and read. Point out blocking.*/


			for(int i = 0; i <= max_fd_value; ++i) {
				if (FD_ISSET(i, &rfds)) {
					if (i != sock_client_fd && i != sock_admin_fd && i != sock_admin_fd2)
						continue;
					
					addr_len = sizeof(their_addr);
					if ((numbytes = recvfrom(i, buf, MAXBUFLEN-1 , 0,
								 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
						perror("recvfrom");
						continue;
					}
					buf[numbytes] = '\0';
					if (i == sock_client_fd) {
						printf("CLIENT: %s\n", buf);
					} else if (i == sock_admin_fd) {
						printf("ADMIN: %s\n", buf);
					} else if (i == sock_admin_fd2) {
						printf("ADMIN2: %s\n", buf);
					}
				}
			}
		} else {
			printf("Select got a timeout\n");
		}	
	}
	close(sock_client_fd);
	close(sock_admin_fd);
}
