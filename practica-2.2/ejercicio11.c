#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct stat inode;
    char *type;

    if (argc != 2)
    {
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int info = stat(argv[1], &inode);

    if (info == -1)
    {
        perror("File do not exist in the directory\n");
        exit(EXIT_FAILURE);
    }

    if (S_ISREG(inode.st_mode))
    {
        char *hard = malloc(sizeof(char) * (5 + strlen(argv[1])));
        char *sym = malloc(sizeof(char) * (5 + strlen(argv[1])));

        strcpy(hard, argv[1]);
        strcpy(sym, argv[1]);
        hard = strcat(hard, ".hard");
        sym = strcat(sym, ".sym");
        if (link(argv[1], hard) == -1)
        {
            printf("Not able to create a hard link\n");
        }
        else
        {
            printf("A hard link has been created: %s\n", hard);
        }

        if (symlink(argv[1], sym) == -1)
        {
            printf("Not able to create a symbolic link\n");
        }
        else
        {
            printf("A symbolic link has been created: %s\n", sym);
        }
        }
    else
    {
        printf("Not a regular file\n");
    }

    return 0;
}