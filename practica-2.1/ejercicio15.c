#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>

int main()
{
    time_t t;
    t = time(NULL);
    struct tm *tv = localtime(&t);

    char *date;
    strftime(date, 100, "%A, %d of %B of %Y, %T", tv);

    printf("Today is %s\n", date);
    return 0;
}