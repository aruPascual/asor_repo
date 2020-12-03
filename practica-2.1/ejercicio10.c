#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <pwd.h>

int main()
{
    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());

    struct passwd *pw = getpwuid(getuid());
    char* name = pw->pw_name;
    char* dir = pw->pw_dir;
    char* gecos = pw->pw_gecos;
    printf("Username: %s\n", name);
    printf("Home dir: %s\n", dir);
    printf("User description: %s\n", gecos);

    return 0;
}

/*
Real UID: 1000
Effective UID: 1000
Username: alvaro
Home dir: /home/alvaro
User description: alvaro,,,
*/