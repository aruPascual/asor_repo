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

    if (execvp(argv[1], argv + 1) == -1)
    {
        printf("ERROR while executing command\n");
        exit(EXIT_FAILURE);
    }

    printf("The command ended its execution\n");
    return 0;
}

/*
Al completar la llamada execvp(), la ejecución no se reanuda por el punto
en el que se quedo, puesto que la función sustituye la imagen del proceso actual
con la nueva.
*/