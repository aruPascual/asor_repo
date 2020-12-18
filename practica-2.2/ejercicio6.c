#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    const char PATH[] = "/home/alvaro/Escritorio/created_file.txt";
    mode_t mask = umask(027);

    int fd = open(PATH, O_CREAT | O_RDONLY, 0777);
    if (fd < 0)
    {
        perror("Error while opening the file");
    }
    else
    {
        printf("File opened succesfully using the mask");
    }

    return 0;
}