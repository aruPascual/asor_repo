#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    if (setuid(0) == -1)
    {
        printf("Error (%d): %s\n", errno, strerror(errno));
    }
    return 1;
}