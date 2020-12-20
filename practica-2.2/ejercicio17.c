#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>
#include <time.h>

#include <dirent.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dirp;
    struct dirent *dp;

    if ((dirp = opendir(argv[1])) == NULL)
    { // "."
        perror("Could not open directory\n");
        return -1;
    }

    if ((dp = readdir(dirp)) != NULL)
    {
        if (strcmp(dp->d_name, argv[1]) == 0)
        {
            printf("Not found %s directory\n", argv[1]);
            closedir(dirp);
            return -1;
        }

        int totalBytes;
        while ((dp = readdir(dirp)) != NULL)
        {
            struct stat inode;
            char *sym;
            char *buf;
            int info = stat(dp->d_name, &inode);

            switch (inode.st_mode & S_IFMT)
            {
            case S_IFBLK:
                break;
            case S_IFCHR:
                break;
            case S_IFDIR:
                printf("%s\n", strcat(dp->d_name, "/"));
                break;
            case S_IFIFO:;
                break;
            case S_IFLNK:
                strcat(sym, dp->d_name);
                strcat(sym, "->");
                ssize_t s = readlink(dp->d_name, buf, (size_t)100);
                printf("%s\n", strcat(sym, buf));
                break;
            case S_IFREG:
                totalBytes = totalBytes + inode.st_size;
                printf("%s\n", dp->d_name);
                break;
            case S_IFSOCK:;
                break;
            default:
                printf("Unknown file type (%s)\n", dp->d_name);
                break;
            }
        }

        printf("\n");
        printf("Total size of files: %i\n", totalBytes);

        closedir(dirp);
    }

    return 0;
}