#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <time.h>

int main()
{
    time_t sec;

    sec = time(NULL);
    printf("Current time (since 1970 Jan 1st) is - %ld:%ld:%ld [%ld seconds have passed]\n",
           (sec / 3600) % 24, (sec / 60) % 60, (sec) % 60, sec);

    return 0;
}
/* Current time (since 1970 Jan 1st) is - 11:31:29 [1607254289 seconds have passed] */