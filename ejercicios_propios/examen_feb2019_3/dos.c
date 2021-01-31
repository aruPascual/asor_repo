#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

volatile int intCount = 0;
volatile int tstpCount = 0;

void handler(int signal)
{
    if (signal == SIGINT)
    {
        intCount++;
    }
    if (signal == SIGTSTP)
    {
        tstpCount++;
    }
}

int main()
{
    struct sigaction sig;

    if (sigaction(SIGINT, NULL, &sig) == -1)
    {
        printf("Unable to get SIGINT handler\n");
        exit(EXIT_FAILURE);
    }
    sig.sa_handler = handler;
    if (sigaction(SIGINT, &sig, NULL) == -1)
    {
        printf("Unable to set SIGINT handler\n");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTSTP, NULL, &sig) == -1)
    {
        printf("Unable to get SIGTSTP handler\n");
        exit(EXIT_FAILURE);
    }
    sig.sa_handler = handler;
    if (sigaction(SIGTSTP, &sig, NULL) == -1)
    {
        printf("Unable to set SIGTSTP handler\n");
        exit(EXIT_FAILURE);
    }

    sigset_t set;
    sigemptyset(&set);
    while ((intCount + tstpCount) < 10)
    {
        sigsuspend(&set);
    }
    printf("SIGINT signals recieved: %i\n", intCount);
    printf("SIGTSTP signals recieved: %i\n", tstpCount);
    
    return 0;
}