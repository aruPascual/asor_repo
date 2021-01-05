#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("PID: %i\n", getpid());
    printf("PPID: %i\n", getppid());
    printf("GID: %i\n", getgid());
    printf("SID: %i\n", getsid(getpid()));

    struct rlimit limit;
    if (getrlimit(RLIMIT_NOFILE, &limit) == -1)
    {
        printf("ERROR: unable to get limit files");
        return -1;
    }
    printf("Max limit of files: %li\n", limit.rlim_max);

    char *path = malloc(sizeof(char) * (4096 + 1));
    char *rpath = getcwd(path, (4096 + 1));
    printf("CWD: %s\n", path);
    free(path);

    return 0;
}

/*
$ ./ejercicio5 
PID: 4830
PPID: 3300
GID: 1000
SID: 3300
Max limit of files: 1048576
CWD: /home/alvaro/Documentos/GitHub/asor_repo/practica-2.3
*/