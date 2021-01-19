#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main (int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = 0;          /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int addr = getaddrinfo(argv[1], NULL, &hints, &result);
    if (addr != 0)
    {
        printf("ERROR getaddrinfo: %s\n", gai_strerror(addr));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        char host[NI_MAXHOST];
        getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST,
            NULL, 0, NI_NUMERICHOST);
        printf("%s\t%i\t%i\n", host, rp->ai_family, rp->ai_socktype);
    }

    freeaddrinfo(result);

    return 0;
}