#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("Maximum length of the arguments: %li\n", sysconf(_SC_ARG_MAX));
    printf("Maximum number of simultaneous processes per user ID: %li\n", sysconf(_SC_CHILD_MAX));
    printf("Maximum number of files that a process can have open: %li\n", sysconf(_SC_OPEN_MAX));
    return 0;
}

/*
Maximum length of the arguments: 2097152
Maximum number of simultaneous processes per user ID: 63359
Maximum number of files that a process can have open: 8192
*/