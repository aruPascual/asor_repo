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

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
    hints.ai_protocol = 0;           /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
        Try each address until we successfully bind(2).
        If socket(2) (or bind(2)) fails, we (close the socket
        and) try the next address. */

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

    freeaddrinfo(result); /* No longer needed */
    peer_addr_len = sizeof(peer_addr);

    if (-1 == listen(sfd, 5))
    {
        perror("ERROR listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int acceptedSocket = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (acceptedSocket == -1)
        {
            perror("ERROR accept");
            exit(EXIT_FAILURE);
        }

        char host[NI_MAXHOST], service[NI_MAXSERV];

        s = getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        if (s == 0)
            printf("Received %zd bytes from %s:%s\n", nread, host, service);
        else
        {
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
            exit(EXIT_FAILURE);
        }

        do
        {
            nread = recv(acceptedSocket, buf, BUF_SIZE * sizeof(char), 0);
            if (nread == -1)
            {
                perror("ERROR recv");
                close(sfd);
                exit(EXIT_FAILURE);
            }
            else if (nread == 0)
            {
                printf("%s:%s abandono la conexion\n", host, service);
            }
            else
            {
                buf[nread] = '\0';
                ssize_t nwrite = send(acceptedSocket, buf, (nread + 1) * sizeof(char), 0);
                if (nwrite == -1)
                {
                    perror("ERROR send");
                    close(sfd);
                    exit(EXIT_FAILURE);
                }
            }
        } while (nread > 0);

        close(acceptedSocket);
    }

    close(sfd);
    return 0;
}