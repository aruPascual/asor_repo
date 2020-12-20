#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_CREAT | O_RDWR, 0645);
    if (fd == -1)
    {
        printf("Error while creating or opening the file\n");
        return -1;
    }
    
    int fd_dup = dup2(fd, 1);
    int fd_dup_err = dup2(fd, 2);

    printf("Redirected to %s\n", argv[1]);
    char *s;
    if (setuid(0) == -1)
    {
        perror(s);
    }

    dup2(fd_dup, fd);
    dup2(fd_dup_err, fd);
    
    close(fd);
    close(fd_dup);
    close(fd_dup_err);

    return 0;
}