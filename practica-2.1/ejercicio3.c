#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

const int ERR_MAX = 133;

int main()
{
    for (int i = 0; i <= ERR_MAX; i++)
    {
        printf("Error (%d): %s\n", i, strerror(i));
    }
    return 1;
}