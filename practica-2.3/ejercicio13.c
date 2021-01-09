#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile int stop = 0;

void handler(int signal)
{
    if (signal == SIGUSR1)
    {
        stop = 1;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <seconds>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    struct sigaction sig;
    if (sigaction(SIGUSR1, NULL, &sig) == -1)
    {
        printf("Unable to get SIGUSR1 handler\n");
        exit(EXIT_FAILURE);
    }
    sig.sa_handler = handler;
    if (sigaction(SIGUSR1, &sig, NULL) == -1)
    {
        printf("Unable to get SIGUSR1 handler\n");
        exit(EXIT_FAILURE);
    }

    int secs = atoi(argv[1]);
    int i = 0;
    while (i < secs && stop == 0)
    {
        i++;
        sleep(1);
    }
    if (stop == 0)
    {
        printf("Executable is going to be erased\n");
        unlink(argv[0]);
    }

    return 0;
}