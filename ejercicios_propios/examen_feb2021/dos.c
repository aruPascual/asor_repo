#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define WRITE 1
#define READ 0

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <cmd1> <cmd2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(-1 == mkfifo("tuberia", 0644))
    {
        perror("ERROR mkfifo");
        exit(EXIT_FAILURE);
    }

    int tub = open("tuberia",  O_RDWR | O_NONBLOCK);
    if(tub == -1)
    {
        perror("ERROR open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("ERROR fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) /* ????????????? */
    {
        printf("[Child] PID:\t%i\n", getpid());

        ssize_t wTub = write(tub, argv[1], strlen(argv[1]));
        if(wTub == -1)
        {
            perror("ERROR write");
            close(tub);
            exit(EXIT_FAILURE);
        }

        if(execlp(argv[1], argv[1], NULL) == -1)
        {
            perror("ERROR execl");
            exit(EXIT_FAILURE);
        }
    }
    else
    {

        int espera;
        waitpid(pid, &espera, 0);
    
        printf("[Parent] PID:\t%i\n", getpid());

        char buffer[100];
        ssize_t rTub = read(tub, buffer, 99);
        buffer[rTub] = '\0';
        printf("%s\n", buffer);

        if(execlp(argv[2], argv[2], NULL) == -1)
        {
            perror("ERROR execl");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}