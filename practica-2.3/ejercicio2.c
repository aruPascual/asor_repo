#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pid = atoi(argv[1]);
    int sched = sched_getscheduler(pid);

    switch (sched)
    {
    case SCHED_OTHER:
        printf("Scheduler: OTHER\n");
        break;
    case SCHED_FIFO:
        printf("Scheduler: FIFO\n");
        break;
    case SCHED_RR:
        printf("Scheduler: RR (Round-robin)\n");
        break;
    default:
        printf("Scheduler: ERROR\n");
        break;
    }

    struct sched_param params;
    sched_getparam(pid, &params);
    printf("Priority: %i\n", params.sched_priority);

    int max = sched_get_priority_max(sched);
    int min = sched_get_priority_min(sched);
    printf("MAX: %i - MIN: %i\n", max, min);

    return 0;
}