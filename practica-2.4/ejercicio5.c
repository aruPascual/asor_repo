#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
    if (-1 == mkfifo("pipe1", 0644))
    {
        perror("Pipe1 creating ERROR");
        exit(EXIT_FAILURE);
    }
    if (-1 == mkfifo("pipe2", 0644))
    {
        perror("Pipe2 creating ERROR");
        exit(EXIT_FAILURE);
    }

    int pipe1 = open("pipe1", O_RDONLY | O_NONBLOCK);
    if (pipe1 == -1)
    {
        perror("Pipe1 opening ERROR");
        close(pipe1);
        exit(EXIT_FAILURE);
    }
    int pipe2 = open("pipe2", O_RDONLY | O_NONBLOCK);
    if (pipe2 == -1)
    {
        perror("Pipe2 opening ERROR");
        close(pipe1);
        close(pipe2);
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    int bufLen = 0;
    int selection, pipeSelected;
    do
    {
        fd_set pipes;
        FD_ZERO(&pipes);
        FD_SET(pipe1, &pipes);
        FD_SET(pipe2, &pipes);

        selection = select((pipe1 < pipe2) ? pipe2 + 1 : pipe1 + 1, &pipes, NULL, NULL, NULL);
        if (selection > 0)
        {
            int numPipe = 0;
            if (FD_ISSET(pipe1, &pipes))
            {
                numPipe = 1;
                pipeSelected = pipe1;
            }
            else if (FD_ISSET(pipe2, &pipes))
            {
                numPipe = 2;
                pipeSelected = pipe2;
            }

            ssize_t bufferSize = 256;
            while (bufferSize == 256)
            {
                bufferSize = read(pipeSelected, buffer, 256);
                if (bufferSize == -1)
                {
                    perror("Pipe buffer reading error");
                    close(pipe1);
                    close(pipe2);
                    exit(EXIT_FAILURE);
                }
                buffer[bufferSize] = '\0';
                printf("[Pipe%i]: %s", numPipe, buffer);
            }

            if (bufferSize != 256 && numPipe == 1)
            {
                close(pipe1);
                pipe1 = open("pipe1", O_RDONLY | O_NONBLOCK);
                if (pipe1 == -1)
                {
                    perror("Pipe1 opening ERROR");
                    close(pipe1);
                    close(pipe2);
                    exit(EXIT_FAILURE);
                }
            }

            if (bufferSize != 256 && numPipe == 2)
            {
                close(pipe2);
                pipe2 = open("pipe2", O_RDONLY | O_NONBLOCK);
                if (pipe2 == -1)
                {
                    perror("Pipe2 opening ERROR");
                    close(pipe1);
                    close(pipe2);
                    exit(EXIT_FAILURE);
                }
            }
        }
    } while (selection != -1);

    perror("Selecting file descriptor ERROR");
    close(pipe1);
    close(pipe2);

    return -1;
}