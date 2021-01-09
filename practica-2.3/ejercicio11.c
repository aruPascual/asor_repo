#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//export SLEEP_SECS=60
int main(int argc, char **argv)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTSTP);

    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    {
        printf("Unable to block signals\n");
        exit(EXIT_FAILURE);
    }
    char *sleepSecs = getenv("SLEEP_SECS");
    if (sleepSecs == NULL)
    {
        printf("Unable to read SLEEP_SECS\n");
        exit(EXIT_FAILURE);
    }
    int secs = atoi(sleepSecs);
    printf("Process sleeping for %d seconds\n", secs);
    sleep(secs);

    sigset_t pendingSet;
    sigpending(&pendingSet);
    if (sigismember(&pendingSet, SIGINT) == 1)
    {
        printf("SIGINT recieved\n");
        sigdelset(&set, SIGINT);
    }
    else
    {
        printf("SIGINT not recieved\n");
    }
    if (sigismember(&pendingSet, SIGTSTP) == 1)
    {
        printf("SIGTSTP recieved\n");
        sigdelset(&set, SIGTSTP);
    }
    else
    {
        printf("SIGTSTP not recieved\n");
    }
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    return 0;
}