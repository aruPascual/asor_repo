#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <time.h>

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

    if (lockf(fd, F_LOCK, 0) == -1)
    {
        close(fd);
        printf("Locked file\n");
        return -1;
    }
    else
    {
        time_t curtime;

        time(&curtime);
        printf("Current time = %s", ctime(&curtime));
        sleep(30);

        if (lockf(fd, F_ULOCK, 0) == -1)
        {
            close(fd);
            printf("ERROR AT UNLOCKING\n");
            return -1;
        }
    }

    close(fd);

    return 0;
}