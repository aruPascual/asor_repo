#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

int main()
{
    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    return 0;
}

/*
Real UID: 1000
Effective UID: 1000
*/