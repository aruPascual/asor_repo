#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[257];

    if (argc != 4) {
        printf( "Usage: %s <host> <port> <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; 
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        printf("ERROR getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1)
    {
        printf("ERROR opening socket\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    if (-1 == sendto(sfd, argv[3], strlen(argv[3]) + 1, 0, result->ai_addr, result->ai_addrlen)) {
        perror("ERROR while sending to server");
        exit(EXIT_FAILURE);
    }

    peer_addr_len = sizeof(peer_addr);
    nread = recvfrom(sfd, &buf, 256*sizeof(char), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
    if (nread == -1)
    {
        perror("ERROR recieving data");
        close(sfd);
        exit(EXIT_FAILURE);
    }
    buf[nread] = '\0';
    printf("%s\n", buf);

    close(sfd);
    return 0;
}