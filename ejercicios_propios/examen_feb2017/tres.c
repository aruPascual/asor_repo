#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#define BUF_SIZE 500

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1)
    {
        perror("ERROR socket");
        exit(EXIT_FAILURE);
    }

    if (bind(sfd, result->ai_addr, result->ai_addrlen) != 0)
    {
        perror("ERROR bind");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    peer_addr_len = sizeof(peer_addr);


    nread = recvfrom(sfd, buf, BUF_SIZE * sizeof(char), 0, (struct sockaddr *) &peer_addr, &peer_addr_len);
    if (nread == -1)
    {
        perror("ERROR recvfrom");
        close(sfd);
        exit(EXIT_FAILURE);
    }
    else if(nread == 0)
    {
        printf("hasta luego...\n");
    }
    else
    {
        char host[NI_MAXHOST], service[NI_MAXSERV];
        s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        if(s == -1)
        {
            perror("ERROR getnameinfo");
            close(sfd);
            exit(EXIT_FAILURE);
        }
        printf("Received %ld bytes from %s:%s\n", (long) nread, host, service);

        ssize_t nwrite = sendto(sfd, host, strlen(host), 0, (struct sockaddr *) &peer_addr, peer_addr_len);
        if(nwrite == -1)
        {
            perror("ERROR getnameinfo");
            close(sfd);
            exit(EXIT_FAILURE);
        }
    }

    close(sfd);
    return 0;
}