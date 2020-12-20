#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <time.h>

int main(int argc, char **argv)
{
    struct stat inode;
    char* type;

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

    printf("major and minor asociated with the device: [%lx,%lx]\n", (long)major(inode.st_dev),
           (long)minor(inode.st_dev));
    printf("File i-node: %ld\n", (long)inode.st_ino);
    switch (inode.st_mode & S_IFMT)
    {
        case S_IFBLK: type = "block device";break;
        case S_IFCHR: type = "character device";break;
        case S_IFDIR: type = "directory";break;
        case S_IFIFO: type = "FIFO/pipe";break;
        case S_IFLNK: type = "symlink";break;
        case S_IFREG: type = "regular file";break;
        case S_IFSOCK: type = "socket";break;
        default: type = "unknown?";break;
    }
    printf("File type: %s\n", type);
    printf("Last status change: %s", ctime(&inode.st_ctime));
    printf("Last file modification: %s", ctime(&inode.st_mtime));
    return 0;
}

/*
$ ./ejercicio9 ejercicio7.c 
major and minor asociated with the device: [8,2]
File i-node: 53612819
File type: regular file
Last status change: Fri Dec 18 18:24:11 2020
Last file modification: Fri Dec 18 18:24:11 2020
*/