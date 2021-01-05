#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) // child
    {
        pid_t c_pid = setsid();

        int fd = open("/tmp/daemon.out", O_CREAT | O_RDWR, 0777);
        int fderr = open("/tmp/daemon.err", O_CREAT | O_RDWR, 0777);
        int fdin = open("/dev/null", O_CREAT | O_RDWR, 0777);
        int fd2 = dup2(fd, 2);
        int fderr2 = dup2(fderr, 1);
        int fdin2 = dup2(fdin, 0);

        if (execvp(argv[1], argv + 1) == -1)
        {
            printf("ERROR while executing command\n");
            exit(EXIT_FAILURE);
        }
    }
    else // parent
    {
        printf("Parent process printf() debugger\n");
    }

    return 0;
}