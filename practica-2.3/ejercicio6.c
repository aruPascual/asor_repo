#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

void printProcessAtributes(char *process)
{
    printf("[%s]PID: %i\n", process, getpid());
    printf("[%s]PPID: %i\n", process, getppid());
    printf("[%s]GID: %i\n", process, getgid());
    printf("[%s]SID: %i\n", process, getsid(getpid()));

    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1)
    {
        printf("ERROR: unable to get limit files");
        exit(EXIT_FAILURE);
    }
    printf("[%s]Max limit of files: %li\n", process, limit.rlim_max);

    char *path = malloc(sizeof(char) * (4096 + 1));
    char *rpath = getcwd(path, (4096 + 1));
    printf("[%s]CWD: %s\n", process, path);
    free(path);
}

int main()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) // child
    {
        pid_t c_pid = setsid();
        chdir("/tmp");
        printProcessAtributes("Child");
    }
    else // parent
    {
        printProcessAtributes("Parent");
    }

    return 0;
}

/*
$ ./ejercicio6 
[Parent]PID: 5793
[Parent]PPID: 3300
[Parent]GID: 1000
[Parent]SID: 3300
[Parent]Max limit of files: 1048576
[Parent]CWD: /home/alvaro/Documentos/GitHub/asor_repo/practica-2.3

[Child]PID: 5794
[Child]PPID: 5793
[Child]GID: 1000
[Child]SID: 5794
[Child]Max limit of files: 1048576
[Child]CWD: /tmp
*/