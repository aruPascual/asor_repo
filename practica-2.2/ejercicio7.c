#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    const char PATH[] = "/home/alvaro/Escritorio/created_file.txt";
    mode_t mask1, mask2;

    mask1 = umask(027);
    int fd = open(PATH, O_CREAT | O_RDONLY, 0677);

    if (fd < 0)
    {
        perror("Can't open the file");
    }
    else
    {
        mask2 = umask(mask1);
        printf("File descriptor %i has been opened\n", fd);
    }

    return 0;
}