#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: %s command1 command2 args2..\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();
    if (pid == -1)
    {
        perror("ERROR forking");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("PID: %i\n", getpid());
        if (execlp(argv[1], argv[1], NULL) == -1)
        {
            perror("ERROR executing");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int returnStatus;
        waitpid(pid, &returnStatus, 0);
        printf("PID: %i\n", getpid());

        for (size_t i = 1; i < argc; i++)
        {
            argv[i] = argv[i + 1];
        }
        argv[argc] = NULL;

        if (execvp(argv[1], argv + 1) == -1)
        {
            perror("ERROR executing");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}