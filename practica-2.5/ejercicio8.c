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
    char buf[257] = "";

    if (argc != 3)
    {
        printf("Usage: %s <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0)
    {
        printf("ERROR getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sfd == -1)
    {
        printf("ERROR opening socket\n");
        exit(EXIT_FAILURE);
    }

    if (bind(sfd, result->ai_addr, result->ai_addrlen) != 0)
    {
        perror("ERROR: unable to bind socket");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);
    peer_addr_len = sizeof(peer_addr);

    if (-1 == listen(sfd, 3))
    {
        perror("ERROR listening");
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();
    while (pid)
    {
        int acceptedSocket = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (acceptedSocket == -1)
        {
            perror("ERROR accepting conection");
            exit(EXIT_FAILURE);
        }

        char hostname[NI_MAXHOST];
        char port[NI_MAXSERV];
        if (getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, hostname, NI_MAXHOST, port, NI_MAXSERV, 0))
        {
            perror("ERROR getting name info");
            exit(EXIT_FAILURE);
        }
        printf("Conexion desde %s:%s\n", hostname, port);

        pid = fork();
        if (pid == -1)
        {
            perror("ERROR forking");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            do
            {
                nread = recv(acceptedSocket, buf, 256 * sizeof(char), 0);
                if (nread == -1)
                {
                    perror("ERROR: recieving data");
                    close(sfd);
                    exit(EXIT_FAILURE);
                }
                else if (nread == 0)
                {
                    printf("Conexion terminada\n");
                    buf[0] = 'q';
                }
                else
                {
                    buf[nread] = '\0';
                    ssize_t nWrite = send(acceptedSocket, buf, (nread + 1) * sizeof(char), 0);
                    if (nWrite == -1)
                    {
                        perror("ERROR writing to client");
                        exit(EXIT_FAILURE);
                    }
                }

            } while (nread > 0);
        }

        close(acceptedSocket);
    }

    close(sfd);

    return 0;
}