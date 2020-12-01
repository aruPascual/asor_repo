#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

int main()
{
    if (setuid(0) == -1)
    {
        perror("Wrong uid");
    }
    return 1;
}