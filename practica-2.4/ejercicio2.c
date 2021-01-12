#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define WRITE 1
#define READ 0

int main(int argc, char **argv)
{
    int pH[2];
    int hP[2];
    if (-1 == pipe(pH) || -1 == pipe(hP))
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
        close(pH[WRITE]);
        close(hP[READ]);

        char msg[256];
        char parentMsg[1] = "l";
        for (int i = 0; i < 10; i++)
        {
            ssize_t childSize = read(pH[READ], msg, 255);
            if (childSize == -1)
            {
                perror("[Child] Pipe reading ERROR");
                exit(EXIT_FAILURE);
            }

            msg[childSize] = '\0';
            printf("[Child] Message: %s", msg);
            sleep(1);

            if (i == 9)
            {
                parentMsg[0] = 'q';
            }
            childSize = write(hP[WRITE], parentMsg, 1);
            if (childSize == -1)
            {
                perror("[Child] Pipe writing ERROR");
                exit(EXIT_FAILURE);
            }
        }
        close(pH[READ]);
        close(hP[WRITE]);
        exit(EXIT_SUCCESS);
    }
    else // parent
    {
        close(pH[READ]);
        close(hP[WRITE]);

        char msg[257];
        char childMsg[1] = "l";
        while (childMsg[0] != 'q')
        {
            printf("[Parent] Enter new message: \n");
            ssize_t parentSize = read(0, msg, 255);
            if (parentSize == -1)
            {
                perror("[Parent] stdin reading ERROR");
                exit(EXIT_FAILURE);
            }

            msg[parentSize] = '\0';
            parentSize = write(pH[WRITE], msg, parentSize + 1);
            if (parentSize == -1)
            {
                perror("[Parent] Pipe writing ERROR");
                exit(EXIT_FAILURE);
            }

            while (childMsg[0] != 'l' && childMsg[0] != 'q')
            {
                parentSize = read(hP[READ], childMsg, 1);
                if (parentSize == -1)
                {
                    perror("[Parent] Pipe reading ERROR");
                    exit(EXIT_FAILURE);
                }
            }
        }
        close(pH[WRITE]);
        close(hP[READ]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}