#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int commandLen = 1;
    for (int i = 1; i < argc; i++)
    {
        commandLen += strlen(argv[i]) + 1;
    }
    char *command = malloc(sizeof(char) * commandLen);
    strcpy(command, "");

    for (int i = 1; i < argc; i++)
    {
        strcat(command, argv[i]);
        strcat(command, " ");
    }
    
    if (system(command) == -1)
    {
        printf("ERROR while executing command\n");
        exit(EXIT_FAILURE);
    }

    printf("The command ended its execution\n");
    return 0;
}

/*
Al completar la llamada system(), la ejecución vuelve a el punto en el que la dejo.
*/