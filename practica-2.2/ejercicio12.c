#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
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
    printf("Redirected to %s\n", argv[1]);
    dup2(fd_dup, fd);

    return 0;
}