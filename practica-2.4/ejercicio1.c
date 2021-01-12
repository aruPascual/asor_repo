#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define WRITE 1
#define READ 0

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("Usage: %s command1 argument1 command2 argument2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Pipe ERROR");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Fork ERROR");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) //child
    {
        close(fd[WRITE]); // close the one not operating
        dup2(fd[READ], 0); // dup the one in need
        close(fd[READ]);
        execlp(argv[3], argv[3], argv[4], NULL);
    }
    else // parent
    {
        close(fd[READ]); // close the one not operating
        dup2(fd[WRITE], 1); // dup the one in need
        close(fd[WRITE]);
        execlp(argv[1], argv[1], argv[2], NULL);
    }

    return 0;
}