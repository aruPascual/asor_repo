#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pwd.h>

#include <sys/types.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *dp;
    struct stat statbuf;
    struct passwd *pwd;

    if (argc != 2)
    {
        printf("Usage: %s <dir>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((dir = opendir(argv[1])) == NULL)
    {
        perror("ERROR opendir");
        exit(EXIT_FAILURE);
    }

    if ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, argv[1]) != 0)
        {
            perror("ERROR readdir");
            closedir(dir);
            exit(EXIT_FAILURE);
        }

        while ((dp = readdir(dir)) != NULL)
        {
            int info = stat(dp->d_name, &statbuf);

            printf("File name:\t\t%s\n", dp->d_name);

            if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
            {
                printf("File owner:\t\t%s\n", pwd->pw_name);
            }
            else
            {
                printf("File owner:\t%d\n", statbuf.st_uid);
            }

            printf("File i-node number:\t%li\n", dp->d_ino);

            printf("File size:\t\t%ld\n", statbuf.st_size);

            printf("\t-----\n");
        }

        closedir(dir);
    }

    return 0;
}