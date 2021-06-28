#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[256] = "";

    if (argc != 3)
    {
        printf("Usage: %s <host> <port>\n", argv[0]);
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

    freeaddrinfo(result);
    peer_addr_len = sizeof(peer_addr);

    if (-1 == listen(sfd, 5))
    {
        perror("ERROR listen");
        exit(EXIT_FAILURE);
    }

    pid_t pid = getpid();

    while (pid)
    {
        int acceptedSocket = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (acceptedSocket == -1)
        {
            perror("ERROR accept");
            exit(EXIT_FAILURE);
        }

        char host[NI_MAXHOST], service[NI_MAXSERV];

        if (getnameinfo((struct sockaddr *)&peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV))
        {
            perror("ERROR getnameinfo");
            exit(EXIT_FAILURE);
        }
        printf("Received bytes from %s:%s\n", host, service);
        printf("[PID] :\t %i\n", pid);

        pid = fork();
        if (pid == -1)
        {
            perror("ERROR fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) /*child*/
        {
            do
            {
                nread = recv(acceptedSocket, buf, 256 * sizeof(char), 0);
                if (nread == -1)
                {
                    perror("ERROR recv");
                    close(sfd);
                    exit(EXIT_FAILURE);
                }
                else if (nread == 0)
                {
                    printf("Conection ended\n");
                }
                else
                {
                    buf[nread] = '\0';
                    ssize_t nwrite = send(acceptedSocket, buf, (nread + 1) * sizeof(char), 0);
                    if (nwrite == -1)
                    {
                        perror("ERROR send");
                        exit(EXIT_FAILURE);
                    }
                }

            } while (nread > 0);
        }
        else /*parent*/
        {
            pid = wait(NULL);
            close(sfd);
            exit(EXIT_FAILURE);
        }

        close(acceptedSocket);
    }

    close(sfd);
    return 0;
}