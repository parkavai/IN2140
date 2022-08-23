#include "libraries.h"

#define BUFSIZE 250

void check_error(int ret, char *msg) {
    if (ret == -1) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    int rc;
    char buf[BUFSIZE];
    fd_set set;
    struct timeval tv;

    FD_ZERO(&set);

    buf[0] = '\0';
    while (strcmp(buf, "q")) {
        tv.tv_sec = 4;
        tv.tv_usec = 0;
        FD_SET(STDIN_FILENO, &set);

        rc = select(FD_SETSIZE, &set, NULL, NULL, &tv);
        check_error(rc, "select");

        if (rc == 0) {
            printf("Er du her?\n");
        }
        else if (FD_ISSET(STDIN_FILENO, &set)) {
            rc = read(STDIN_FILENO, buf, BUFSIZE);
            check_error(rc, "read");

            buf[rc-1] = '\0';
            printf("Echo: %s\n", buf);
        }
    }

    return EXIT_SUCCESS;
}