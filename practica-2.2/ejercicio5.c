#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    const char PATH[] = "/home/alvaro/Escritorio/created_file.txt";

    int fd = open(PATH, O_CREAT, 0645);

    if (fd == -1)
    {
        perror("Can't open the file");
    }
    else
    {
        printf("File descriptor %i has been opened\n", fd);
    }
    
    return 0;
}