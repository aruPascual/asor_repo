#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>

int main()
{
    struct timeval t;
    int currentT = gettimeofday(&t, NULL);
    long int startT = t.tv_usec;

    for (int i = 0; i < 1000000; i++)
    {
        /* code */
    }

    currentT = gettimeofday(&t, NULL);
    long int endT = t.tv_usec;

    printf("The loop took %ld microseconds to reach 1 million iterations\n", (endT - startT));
    return 0;
}