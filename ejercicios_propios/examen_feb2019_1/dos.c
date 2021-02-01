#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: %s command arg1 arg2 ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        perror("ERROR fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        pid_t child = setsid();
        int dir = chdir("/tmp");
        if (dir == -1)
        {
            perror("ERROR chdir");
            exit(EXIT_FAILURE);
        }

        printf("PID:    %i\n", getpid());
        printf("PPID:   %i\n", getppid());
        printf("GID:    %i\n", getgid());
        printf("SID:    %i\n", getsid(getpid()));

        if (execvp(argv[1], argv + 1) == -1)
        {
            perror("ERROR execvp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int espera;
        waitpid(pid, &espera, 0);
    }

    return 0;
}