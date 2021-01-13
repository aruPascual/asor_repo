#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <pipe>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open("./tuberia", O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open the pipe");
        exit(EXIT_FAILURE);
    }

    size_t length = strlen(argv[1]);
    ssize_t wrPipe = write(fd, argv[1], length);
    if (wrPipe == -1)
    {
        perror("Pipe writing ERROR");
        close(fd);
        exit(EXIT_FAILURE);  
    }
    close(fd);

    return 0;
}