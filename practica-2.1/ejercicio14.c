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

    int year = tv->tm_year;
    printf("This year is %i\n", (1900 + year));
    return 0;
}