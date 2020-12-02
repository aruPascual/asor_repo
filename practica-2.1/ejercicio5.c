#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <errno.h>
//
#include <sys/types.h>
#include <sys/utsname.h>

int main()
{
    struct utsname sys;
    if (uname(&sys) == -1)
    {
        printf("Error (%d): %s\n", errno, strerror(errno));
    }
    else
    {
        printf("System name: %s\n", sys.sysname);
        printf("Nodename: %s\n", sys.nodename);
        printf("Release: %s\n", sys.release);
        printf("Version: %s\n", sys.version);
        printf("Machine: %s\n", sys.machine);
    }
    return -1;
}