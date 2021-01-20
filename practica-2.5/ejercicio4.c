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
    char buf[2] = "";

    if (argc != 3)
    {
        printf("Usage: %s <host> <port>\n", argv[0]);
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
    fd_set set;

    while (buf[0] != 'q')
    {
        FD_ZERO(&set);
        FD_SET(0, &set);
        FD_SET(sfd, &set);

        if (-1 == select(sfd + 1, &set, NULL, NULL, NULL))
        {
            perror("ERROR: waitinf for data");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(sfd, &set))
        {
            nread = recvfrom(sfd, &buf, 2 * sizeof(char), 0, (struct sockaddr *)&peer_addr, &peer_addr_len);
        }
        else if (FD_ISSET(0, &set))
        {
            nread = read(0, &buf, 2 * sizeof(char));
        }

        if (nread == -1)
        {
            perror("ERROR: recieving data");
            close(sfd);
            exit(EXIT_FAILURE);
        }
        else if (nread == 0)
        {
            printf("Saliendo...");
            buf[0] = 'q';
        }
        else
        {
            if (FD_ISSET(sfd, &set))
            {
                char hostname[NI_MAXHOST];
                char port[NI_MAXSERV];

                if (getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, hostname, NI_MAXHOST, port, NI_MAXSERV, 0))
                {
                    perror("ERROR getting name info");
                    exit(EXIT_FAILURE);
                }

                printf("%i byte(s) de %s:%s\n", nread, hostname, port);
            }

            time_t t = time(NULL);
            struct tm *loctime = localtime(&t);
            char buffer[256];

            if (buf[0] == 't')
            {
                ssize_t timeStr = strftime(buffer, 256, "%I:%M:%S %p", loctime);

                if (FD_ISSET(0, &set))
                {
                    printf("%s\n", buffer);
                }
                else if (FD_ISSET(sfd, &set))
                {
                    ssize_t nWrite = sendto(sfd, buffer, strlen(buffer), 0, (struct sockaddr *)&peer_addr, peer_addr_len);
                    if (nWrite == -1)
                    {
                        perror("ERROR while sending back to client");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else if (buf[0] == 'd')
            {
                ssize_t timeStr = strftime(buffer, 256, "%Y-%m-%d", loctime);

                if (FD_ISSET(0, &set))
                {
                    printf("%s\n", buffer);
                }
                else if (FD_ISSET(sfd, &set))
                {
                    ssize_t nWrite = sendto(sfd, buffer, strlen(buffer), 0, (struct sockaddr *)&peer_addr, peer_addr_len);
                    if (nWrite == -1)
                    {
                        perror("ERROR while sending back to client");
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else if (buf[0] == 'q')
            {
                printf("Saliendo...\n");
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("Comando no soportado: %d...\n", buf[0]);
            }
        }
    }

    close(sfd);
    return 0;
}